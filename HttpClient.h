// DN: Added the provided HTTP client header to the repo 
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#pragma comment (lib, "Wininet.lib")

class HttpClient
{
	HINTERNET hSession {};
	HINTERNET hConnect {};
	std::string authToken;
	const std::string userAgent = "itcs2550/app";
	const std::string acceptType = "application/json";
protected:
	virtual void Data(const char* data, const unsigned int size) {}
	virtual void StartOfData() {}
	virtual void EndOfData() {}
	int nPort = INTERNET_DEFAULT_HTTPS_PORT;
public:
	HttpClient()
	{
		const std::string acceptType = "application/json";
		hSession = InternetOpenA(userAgent.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (hSession == NULL)
			std::cerr << "InternetOpen error : " << GetLastError() << std::endl;
	}

	~HttpClient()
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
	}

	void SetAuthToken(const std::string token = "") { authToken = token; }

	const std::string AddQueryParameters(const std::string uri, const std::map<std::string, std::string>& qp = {})
	{
		if (!qp.size())
			return uri;

		std::string path(uri + '?');
		for (auto& p : qp)
		{
			path += p.first + "=" + p.second + '&';
		}
		path.pop_back();
		return path;
	}

	bool Connect(const std::string server_name, const int p = INTERNET_DEFAULT_HTTPS_PORT)
	{
		nPort = p;
		if (hSession)
		{
			hConnect = InternetConnectA(hSession, server_name.c_str(), nPort, "", "", INTERNET_SERVICE_HTTP, 0, 0);
			if (hConnect == NULL)
				std::cerr << "InternetConnect error : " << GetLastError() << std::endl;
		}
		return hConnect != NULL;
	}

	bool Get(const std::string uri, const std::map<std::string, std::string> qp = {})
	{
		std::string u = AddQueryParameters(uri, qp);
		DWORD dwFlags = (nPort == INTERNET_DEFAULT_HTTPS_PORT) ? INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_SECURE : INTERNET_FLAG_DONT_CACHE;
		PCSTR rgpszAcceptTypes[] = { acceptType.c_str(), NULL };
		HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", u.c_str(), NULL, NULL, rgpszAcceptTypes, dwFlags, 0);
		if (hRequest == NULL)
		{
			std::cerr << "HttpOpenRequest error : " << GetLastError() << std::endl;
			return false;
		}

		std::string connHeader("Connection: keep-alive");
		HttpAddRequestHeadersA(hRequest, connHeader.c_str(), static_cast<DWORD>(connHeader.length()), 0);
		if (!authToken.empty())
		{
			std::string authHeader("Authorization: token " + authToken + "\r\n");
			HttpAddRequestHeadersA(hRequest, authHeader.c_str(), static_cast<DWORD>(authHeader.length()), 0);
		}

		while (!HttpSendRequest(hRequest, NULL, 0, 0, 0))
		{
			std::cerr << "HttpOpenRequest error : " << GetLastError() << std::endl;
			return false;
		}

		const int BUFFER_SIZE = 4096;
		char buffer[BUFFER_SIZE] {};
		StartOfData();
		while (true)
		{
			DWORD dwBytesRead;
			BOOL bRead;
			bRead = InternetReadFile(hRequest, buffer, BUFFER_SIZE, &dwBytesRead);
			if (!bRead)
			{
				std::cerr << "InternetReadFile error : " << GetLastError() << std::endl;
			}
			else
			{
				if (dwBytesRead == 0)
					break;
				Data(buffer, dwBytesRead);
			}
		}
		EndOfData();
		InternetCloseHandle(hRequest);
		return true;
	}

	bool Post(const std::string uri, const std::string& body)
	{
		DWORD dwFlags = (nPort == INTERNET_DEFAULT_HTTPS_PORT) ? INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_SECURE : INTERNET_FLAG_DONT_CACHE;
		PCSTR rgpszAcceptTypes[] = { acceptType.c_str(), NULL };
		HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", uri.c_str(), NULL, NULL, rgpszAcceptTypes, dwFlags, 0);
		if (hRequest == NULL)
		{
			std::cerr << "HttpOpenRequest error : " << GetLastError() << std::endl;
			return false;
		}

		std::string headers = "Content-Type: application/json\r\nConnection: keep-alive\r\n";
		HttpAddRequestHeadersA(hRequest, headers.c_str(), static_cast<DWORD>(headers.length()), HTTP_ADDREQ_FLAG_ADD);

		if (!authToken.empty())
		{
			std::string authHeader("Authorization: token " + authToken + "\r\n");
			HttpAddRequestHeadersA(hRequest, authHeader.c_str(), static_cast<DWORD>(authHeader.length()), 0);
		}

		if (!HttpSendRequestA(hRequest, NULL, 0, (LPVOID)body.c_str(), static_cast<DWORD>(body.length())))
		{
			std::cerr << "HttpSendRequest error : " << GetLastError() << std::endl;
			InternetCloseHandle(hRequest);
			return false;
		}

		const int BUFFER_SIZE = 4096;
		char buffer[BUFFER_SIZE] {};
		StartOfData();
		while (true)
		{
			DWORD dwBytesRead;
			BOOL bRead;
			bRead = InternetReadFile(hRequest, buffer, BUFFER_SIZE, &dwBytesRead);
			if (!bRead)
			{
				std::cerr << "InternetReadFile error : " << GetLastError() << std::endl;
				break;
			}
			if (dwBytesRead == 0)
				break;
			Data(buffer, dwBytesRead);
		}
		EndOfData();
		InternetCloseHandle(hRequest);
		return true;
	}
};
