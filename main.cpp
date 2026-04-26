// Uncomment this line to run unit tests
// DN: Let the Visual Studio Test configuration control RUN_TESTS so the normal build can still execute main().
// #define RUN_TESTS

#ifdef RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

// ===========================
// CRT MEMORY LEAK DETECTION
// ===========================
#ifndef RUN_TESTS
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#endif

#include <iostream>
#include <string>
#include <stdexcept>
#include <map>
#include <vector>

// File Handling
#include <fstream>

// For Using JSON
#include "HttpClient.h"
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

/* ===========================
   CUSTOM EXCEPTION
   =========================== */
class PuzzleException : public runtime_error
{
public:
    PuzzleException(const string& message)
        : runtime_error(message) {
    }
};

/* ===========================
   FUNCTION TEMPLATE
   =========================== */
template<typename T>
T getMax(T a, T b)
{
    return (a > b) ? a : b;
}

/* ===========================
   ENUM
   =========================== */
enum Difficulty
{
    EASY = 1,
    MEDIUM,
    HARD
};

/* ===========================
   ABSTRACT BASE CLASS
   =========================== */
class Puzzle
{
protected:

    string name;
    int duration;
    Difficulty difficulty;

public:

    Puzzle(string n = "Unknown", int d = 0, Difficulty diff = EASY)
        : name(n), duration(d), difficulty(diff) {
    }

    virtual ~Puzzle() {}

    string getName() const
    {
        return name;
    }

    virtual string getCategory() const = 0;

    virtual void toStream(ostream& os) const
    {
        os << name << " (" << duration << " min)";
    }
};

/* ===========================
   STREAM OPERATOR
   =========================== */
ostream& operator<<(ostream& os, const Puzzle& p)
{
    p.toStream(os);
    return os;
}

/* ===========================
   HTTP CLIENT DERIVED CLASS
   =========================== */
class PuzzleApiClient : public HttpClient
{
private:
    string responseBody;

protected:
    // DN: Reset the saved response before each request so GET and POST do not mix old data with new server data.
    void StartOfData() override
    {
        responseBody.clear();
    }

    // DN: Append every chunk WinINet gives us because HttpClient streams the body in pieces instead of one complete string.
    void Data(const char* data, const unsigned int size) override
    {
        responseBody.append(data, size);
    }

    // DN: Keep this override to show where the streamed response finishes even though no extra cleanup is needed here.
    void EndOfData() override
    {
    }

public:
    // DN: Expose the accumulated body so PuzzleManager can parse JSON without touching the transport details.
    string GetResponse() const
    {
        return responseBody;
    }
};

/* ===========================
   DERIVED CLASS 1
   =========================== */
class LogicPuzzle : public Puzzle
{
private:
    int cluesUsed;

public:

    LogicPuzzle(string n, int d, Difficulty diff, int clues)
        : Puzzle(n, d, diff), cluesUsed(clues) {
    }

    string getCategory() const override
    {
        return "Logic";
    }

    void toStream(ostream& os) const override
    {
        os << name << " clues:" << cluesUsed;
    }

    /* operator== */
    bool operator==(const LogicPuzzle& other) const
    {
        return name == other.name &&
            duration == other.duration &&
            difficulty == other.difficulty &&
            cluesUsed == other.cluesUsed;
    }
};

/* ===========================
   DERIVED CLASS 2
   =========================== */
class WordPuzzle : public Puzzle
{
private:
    int wordsFound;

public:

    WordPuzzle(string n, int d, Difficulty diff, int words)
        : Puzzle(n, d, diff), wordsFound(words) {
    }

    string getCategory() const override
    {
        return "Word";
    }

    void toStream(ostream& os) const override
    {
        os << name << " words:" << wordsFound;
    }
};

/* ===========================
   DERIVED CLASS 3
   =========================== */
class JokePuzzle : public Puzzle
{
private:
    int jokeId;
    string jokeCategory;
    string punchline;

    // DN: Translate the joke category into the existing Difficulty enum so API data fits the current class model cleanly.
    static Difficulty categoryToDifficulty(const string& category)
    {
        if (category == "programming")
            return HARD;

        if (category == "math")
            return MEDIUM;

        return EASY;
    }

public:
    JokePuzzle(int id, const string& category, const string& setupText, const string& punchlineText)
        : Puzzle(setupText, 1, categoryToDifficulty(category)),
        jokeId(id),
        jokeCategory(category),
        punchline(punchlineText) {
    }

    string getCategory() const override
    {
        return "Joke";
    }

    void toStream(ostream& os) const override
    {
        os << "Joke #" << jokeId << " [" << jokeCategory << "] " << name << " -> " << punchline;
    }
};

/* ===========================
   LINKED LIST ADT
   =========================== */
template<typename T>
class LinkedList
{
private:

    struct Node
    {
        T data;
        Node* next;

        Node(T d)
        {
            data = d;
            next = nullptr;
        }
    };

    Node* head;

public:

    /* ===========================
       ITERATOR CLASS
       =========================== */
    class Iterator
    {
    private:
        Node* current;

    public:

        Iterator(Node* start)
        {
            current = start;
        }

        bool hasNext() const
        {
            return current != nullptr;
        }

        void next()
        {
            if (current)
                current = current->next;
        }

        T getData() const
        {
            if (!current)
                throw PuzzleException("Iterator out of bounds");

            return current->data;
        }
    };

    /* ===========================
       CONSTRUCTOR / DESTRUCTOR
       =========================== */
    LinkedList()
    {
        head = nullptr;
    }

    void clear()
    {
        Node* current = head;

        while (current)
        {
            Node* temp = current;
            current = current->next;
            delete temp;
        }

        head = nullptr;
    }

    ~LinkedList()
    {
        clear();
    }

    /* ===========================
       INSERT OPERATIONS
       =========================== */
    void insertFront(T value)
    {
        Node* node = new Node(value);
        node->next = head;
        head = node;
    }

    void insertBack(T value)
    {
        Node* node = new Node(value);
        if (!head)
        {
            head = node;
            return;
        }

        Node* current = head;
        while (current->next)
            current = current->next;

        current->next = node;
    }

    /* ===========================
       DELETE OPERATION
       =========================== */
    bool deleteNode(T value)
    {
        if (!head)
            return false;

        if (head->data == value)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
            return true;
        }

        Node* current = head;

        while (current->next)
        {
            if (current->next->data == value)
            {
                Node* temp = current->next;
                current->next = temp->next;
                delete temp;
                return true;
            }

            current = current->next;
        }

        return false;
    }

    /* ===========================
       SEARCH OPERATION
       =========================== */
    bool search(T value)
    {
        Node* current = head;

        while (current)
        {
            if (current->data == value)
                return true;

            current = current->next;
        }

        return false;
    }

    /* ===========================
       PRINT / TRAVERSE OPERATION
       =========================== */
    void print()
    {
        Node* current = head;

        while (current)
        {
            cout << current->data << " ";
            current = current->next;
        }

        cout << endl;
    }

    /* ===========================
       UTILITY FUNCTIONS
       =========================== */
    int size() const
    {
        int count = 0;
        Node* current = head;

        while (current)
        {
            count++;
            current = current->next;
        }

        return count;
    }

    T getAtIndex(int index) const
    {
        if (index < 0)
            throw PuzzleException("Index out of bounds");

        Node* current = head;
        int i = 0;

        while (current)
        {
            if (i == index)
                return current->data;

            current = current->next;
            i++;
        }

        throw PuzzleException("Index out of bounds");
    }

    /* ===========================
       SWAP NODES
       =========================== */
    void swapAtIndex(int a, int b)
    {
        if (a == b)
            return;

        Node* nodeA = head;
        Node* nodeB = head;

        int index = 0;

        while (nodeA && index < a)
        {
            nodeA = nodeA->next;
            index++;
        }

        index = 0;

        while (nodeB && index < b)
        {
            nodeB = nodeB->next;
            index++;
        }

        if (nodeA && nodeB)
        {
            T temp = nodeA->data;
            nodeA->data = nodeB->data;
            nodeB->data = temp;
        }
    }

    Node* getHead() const
    {
        return head;
    }
};

/* ===========================
   CLASS TEMPLATE
   =========================== */
template<typename T>
class DynamicArray
{
private:

    T* data;
    int sz;
    int cap;

    void resize()
    {
        int newCap = cap * 2;
        T* temp = new T[newCap];

        for (int i = 0; i < sz; i++)
            temp[i] = data[i];

        delete[] data;
        data = temp;
        cap = newCap;
    }

public:

    DynamicArray(int c = 5)
    {
        sz = 0;
        cap = c;
        data = new T[cap];
    }

    ~DynamicArray()
    {
        delete[] data;
    }

    void add(T item)
    {
        if (sz >= cap)
            resize();

        data[sz++] = item;
    }

    T operator[](int index) const
    {
        if (index < 0 || index >= sz)
            throw PuzzleException("DynamicArray index out of bounds");

        return data[index];
    }

    int size() const
    {
        return sz;
    }
};


/* ===========================
   MANAGER CLASS
   =========================== */
class PuzzleManager
{
private:

    LinkedList<Puzzle*> items;

    //parallel map for faster puzzle searching 
    map<string, Puzzle*> puzzleMap;

    /* ===========================
       RECURSIVE HELPER FUNCTION
       =========================== */
    int countRecursiveHelper(int index) const
    {
        // Base case
        if (index >= items.size())
            return 0;

        // Recursive case
        return 1 + countRecursiveHelper(index + 1);
    }

public:

    void add(Puzzle* p)
    {
        // preserve insertion order expected by tests: append to list
        items.insertBack(p);

        puzzleMap[p->getName()] = p;
    }

    /* operator[] */
    Puzzle* operator[](int index) const
    {
        return items.getAtIndex(index);
    }

    /* operator+= */
    PuzzleManager& operator+=(Puzzle* p)
    {
        add(p);
        return *this;
    }

    /* operator-= */
    PuzzleManager& operator-=(int index)
    {
        Puzzle* p = items.getAtIndex(index);

        if (!p)
            throw PuzzleException("Invalid removal index");

        items.deleteNode(p);   // remove node from linked list
        puzzleMap.erase(p->getName());
        delete p;              // free the puzzle 

        

        return *this;
    }

    int getSize() const
    {
        return items.size();
    }

    int countPuzzlesRecursive() const
    {
        return countRecursiveHelper(0);
    }

    Puzzle* mapLookup(const string& name) {
        auto it = puzzleMap.find(name);

        if (it != puzzleMap.end())
            return it->second;

        return nullptr;
    }
    void printMap() {
        for (auto& pair : puzzleMap) {
            cout << pair.first << " -> " << pair.second->getCategory() << endl;
        }
    }

    // DN: Added a formatted traversal helper so both local puzzles and API jokes can be displayed through existing class behavior.
    void printAllPuzzles() const
    {
        typename LinkedList<Puzzle*>::Iterator it(items.getHead());

        while (it.hasNext())
        {
            Puzzle* p = it.getData();

            if (p)
                cout << *p << endl;

            it.next();
        }
    }

    /* ===========================
       SEQUENTIAL SEARCH
       =========================== */
    int sequentialSearch(const string& target) const
    {
        typename LinkedList<Puzzle*>::Iterator it(items.getHead());

        int index = 0;

        while (it.hasNext())
        {
            Puzzle* p = it.getData();

            if (p && p->getName() == target)
                return index;

            it.next();
            index++;
        }

        return -1;
    }

    /* ===========================
       BUBBLE SORT
       =========================== */
    void bubbleSort()
    {
        int n = items.size();

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                Puzzle* a = items.getAtIndex(j);
                Puzzle* b = items.getAtIndex(j + 1);

                if (a && b && a->getName() > b->getName())
                    items.swapAtIndex(j, j + 1);
            }
        }
    }

    /* ===========================
       BINARY SEARCH
       =========================== */
    int binarySearch(const string& target)
    {
        int low = 0;
        int high = items.size() - 1;

        while (low <= high)
        {
            int mid = (low + high) / 2;

            Puzzle* p = items.getAtIndex(mid);

            if (!p)
                return -1;

            if (p->getName() == target)
                return mid;
            else if (p->getName() < target)
                low = mid + 1;
            else
                high = mid - 1;
        }

        return -1;
    }

    ~PuzzleManager()
    {
        typename LinkedList<Puzzle*>::Iterator it(items.getHead());

        while (it.hasNext())
        {
            delete it.getData();
            it.next();
        }
    }

    // JSON Loading Function
    // Reads from the puzzles.json, parses each object and modify the current structures

    void loadPuzzlesFromJSON(const string& fileName)
    {
        try{
            ifstream file(fileName);

            if (!file.is_open()){
                throw PuzzleException("JSON file not found: " + fileName);
            }

            json data;
            file >> data;

            // Iterate through the JSON array with enhanced for loop
            for (const auto& item: data){
                string type = item.at("type");
                string name = item.at("name");
                int duration = item.at("duration");
                int difficultyAsInt = item.at("difficulty");

                // Converting the difficulty
                Difficulty diff = static_cast<Difficulty>(difficultyAsInt);

                // Creating objects based on derived classes and insert into structures
                if (type == "logic"){
                    int clues = item.at("cluesUsed");
                    *this += new LogicPuzzle(name, duration, diff, clues);
                } else if(type == "word"){
                    int words = item.at("wordsUsed");
                    *this += new WordPuzzle(name, duration, diff, words);
                }
            }
        } catch(const json::exception&){
            throw PuzzleException("Malformed JSON");
        }
        
    }

    // DN: Parse the jokes API response and load each joke into the existing linked list and map through JokePuzzle objects.
    void loadJokesFromApiResponse(const string& responseBody)
    {
        try
        {
            json data = json::parse(responseBody);

            for (const auto& jokeItem : data.at("jokes"))
            {
                int id = jokeItem.at("id");
                string category = jokeItem.at("category");
                string setup = jokeItem.at("setup");
                string punchline = jokeItem.at("punchline");

                *this += new JokePuzzle(id, category, setup, punchline);
            }
        }
        catch (const json::exception&)
        {
            throw PuzzleException("Malformed jokes API response");
        }
    }

    // DN: Parse the POST confirmation so the main program can show the server-assigned joke ID instead of raw JSON.
    int parsePostedJokeId(const string& responseBody) const
    {
        try
        {
            json data = json::parse(responseBody);
            return data.at("joke").at("id");
        }
        catch (const json::exception&)
        {
            throw PuzzleException("Malformed joke POST response");
        }
    }
};

/* ===========================
   MAIN PROGRAM
   =========================== */

#ifndef RUN_TESTS

int main()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    PuzzleManager manager;
    // DN: Added a dedicated API client instance so the REST work stays separate from PuzzleManager ownership logic.
    PuzzleApiClient apiClient;

    // Loading the Data from JSON file
    manager.loadPuzzlesFromJSON("puzzles.json");

    // DN: Fetch programming jokes from the live API and load them into the same manager used by local puzzle data.
    if (apiClient.Connect("api.macomb.io", INTERNET_DEFAULT_HTTP_PORT) &&
        apiClient.Get("/jokes", { {"count", "3"}, {"category", "programming"} }))
    {
        manager.loadJokesFromApiResponse(apiClient.GetResponse());
    }

    // DN: Build the POST body with nlohmann::json so the request sent to the API matches the assignment requirements.
    json newJoke = {
        {"category", "programming"},
        {"setup", "Why did the debugger bring a flashlight?"},
        {"punchline", "Because the bug was hiding in the dark."}
    };

    // DN: Parse the POST confirmation and show the assigned ID so the user sees that the server accepted the new joke.
    if (apiClient.Post("/jokes", newJoke.dump()))
    {
        cout << "Added API joke with ID: " << manager.parsePostedJokeId(apiClient.GetResponse()) << endl;
    }

    //manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);
    //manager += new WordPuzzle("Crossword", 20, EASY, 10);

    cout << "Total puzzles: " << manager.getSize() << endl;

    manager.printMap(); // structure modified
    manager.printAllPuzzles();

    return 0;
}

#else

/* ===========================
   TEST MODE
   =========================== */

TEST_CASE("operator[] throws on invalid index")
{
    PuzzleManager manager;
    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    CHECK(manager[0] != nullptr);
    CHECK_THROWS_AS(manager[5], PuzzleException);
}

TEST_CASE("operator-= throws on invalid removal")
{
    PuzzleManager manager;
    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    CHECK_THROWS_AS(manager -= 5, PuzzleException);
}

TEST_CASE("Template class throws on invalid access")
{
    DynamicArray<int> arr;
    arr.add(10);

    CHECK(arr[0] == 10);
    CHECK_THROWS_AS(arr[2], PuzzleException);
}

TEST_CASE("Custom exception what() works")
{
    try
    {
        throw PuzzleException("Test message");
    }
    catch (const PuzzleException& ex)
    {
        CHECK(string(ex.what()) == "Test message");
    }
}

TEST_CASE("Function template works")
{
    CHECK(getMax(3, 5) == 5);
}

TEST_CASE("Equality operator works")
{
    LogicPuzzle a("Sudoku", 30, MEDIUM, 3);
    LogicPuzzle b("Sudoku", 30, MEDIUM, 3);

    CHECK(a == b);
}

TEST_CASE("Recursive puzzle count works")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);
    manager += new WordPuzzle("Crossword", 20, EASY, 10);
    manager += new LogicPuzzle("KenKen", 25, HARD, 2);

    CHECK(manager.countPuzzlesRecursive() == 3);
}

TEST_CASE("Sequential search works")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);
    manager += new WordPuzzle("Crossword", 20, EASY, 10);

    CHECK(manager.sequentialSearch("Sudoku") == 0);
    CHECK(manager.sequentialSearch("Missing") == -1);
}

TEST_CASE("Bubble sort works")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("ZPuzzle", 30, MEDIUM, 3);
    manager += new LogicPuzzle("APuzzle", 20, EASY, 2);

    manager.bubbleSort();

    CHECK(manager[0]->getName() == "APuzzle");
}

TEST_CASE("Binary search works")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Alpha", 30, MEDIUM, 3);
    manager += new LogicPuzzle("Beta", 20, EASY, 2);
    manager += new LogicPuzzle("Gamma", 40, HARD, 1);

    CHECK(manager.binarySearch("Beta") != -1);
    CHECK(manager.binarySearch("Missing") == -1);
}

TEST_CASE("Insert into empty list")
{
    LinkedList<int> list;

    list.insertFront(5);

    CHECK(list.search(5) == true);
}

TEST_CASE("Delete missing node")
{
    LinkedList<int> list;

    list.insertFront(5);

    CHECK(list.deleteNode(10) == false);
}

TEST_CASE("Traverse empty list")
{
    LinkedList<int> list;

    list.print();

    CHECK(true);
}
TEST_CASE("Map lookup")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    CHECK(manager.mapLookup("Sudoku") != nullptr);
    CHECK(manager.mapLookup("Missing") == nullptr);
}

TEST_CASE("Map delete")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    manager -= 0;

    CHECK(manager.mapLookup("Sudoku") == nullptr);
}

TEST_CASE("Load JSON Correctly"){
    PuzzleManager manager;
    manager.loadPuzzlesFromJSON("puzzles.json");

    CHECK(manager.getSize()== 5);
    CHECK(manager.mapLookup("Sudoku") != nullptr);
}

TEST_CASE("Missing file throws exception"){
    PuzzleManager manager;

    CHECK_THROWS_AS(manager.loadPuzzlesFromJSON("missing.json"), PuzzleException);
}

TEST_CASE("Jokes API response loads into existing manager structures")
{
    PuzzleManager manager;
    string response = R"({"count":2,"jokes":[{"id":21,"category":"programming","setup":"Why do programmers confuse Halloween and Christmas?","punchline":"Because OCT 31 == DEC 25."},{"id":22,"category":"math","setup":"Why was the equal sign so humble?","punchline":"Because it knew it was not less than or greater than anyone else."}]})";

    manager.loadJokesFromApiResponse(response);

    CHECK(manager.getSize() == 2);
    CHECK(manager.mapLookup("Why do programmers confuse Halloween and Christmas?") != nullptr);
    CHECK(manager.mapLookup("Why was the equal sign so humble?") != nullptr);
}

TEST_CASE("Posted joke response returns assigned ID")
{
    PuzzleManager manager;
    string response = R"({"message":"joke added successfully","joke":{"id":44,"category":"general","setup":"A","punchline":"B"}})";

    CHECK(manager.parsePostedJokeId(response) == 44);
}

TEST_CASE("Malformed jokes API response throws exception")
{
    PuzzleManager manager;

    CHECK_THROWS_AS(manager.loadJokesFromApiResponse("{bad json"), PuzzleException);
    CHECK_THROWS_AS(manager.parsePostedJokeId("{bad json"), PuzzleException);
}

#endif
