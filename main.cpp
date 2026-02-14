#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

/* =====================================================
   MODE SWITCH
   =====================================================
   Uncomment RUN_TESTS to run unit tests.
   Comment it out to run the actual program.
*/
#define RUN_TESTS

/* ==================== DOCTEST SETUP ==================== */
#ifdef RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

/* -------------------- Constants -------------------- */
const int MAX_SESSIONS = 5;
const int MAX_TIMES = 5;

/* -------------------- Enum -------------------- */
enum Difficulty
{
    EASY = 1,
    MEDIUM,
    HARD
};

/* -------------------- Struct -------------------- */
struct PuzzleSession
{
    string puzzleName;
    Difficulty level;
    int puzzlesSolved;
    double times[MAX_TIMES];
};

/* -------------------- Class -------------------- */
class PuzzleTracker
{
private:
    PuzzleSession sessions[MAX_SESSIONS];
    int sessionCount;

public:
    PuzzleTracker();

    void showBanner() const;
    void showMenu();
    void addSession();
    void showReport() const;
    void saveReportToFile(const string& filename) const;

    bool addSession(const PuzzleSession& s);
    int getSessionCount() const;
    double getAverageTimeForSession(int index) const;
    string getDifficultyLabel(Difficulty d) const;
    double calculateAverageTime(const double times[], int size) const;
};

/* -------------------- Constructor -------------------- */
PuzzleTracker::PuzzleTracker()
{
    sessionCount = 0;
}

/* -------------------- Banner -------------------- */
void PuzzleTracker::showBanner() const
{
    cout << "=====================================\n";
    cout << "        Welcome to Puzzle Tracker\n";
    cout << "=====================================\n";
}

/* -------------------- Interactive Add Session -------------------- */
void PuzzleTracker::addSession()
{
    if (sessionCount >= MAX_SESSIONS)
    {
        cout << "Maximum number of sessions reached.\n";
        return;
    }

    PuzzleSession& s = sessions[sessionCount];

    cout << "Enter puzzle name: ";
    getline(cin, s.puzzleName);

    while (s.puzzleName.empty())
    {
        cout << "Puzzle name cannot be empty. Try again: ";
        getline(cin, s.puzzleName);
    }

    int diffChoice;
    do
    {
        cout << "Difficulty (1=Easy, 2=Medium, 3=Hard): ";
        cin >> diffChoice;
    } while (diffChoice < EASY || diffChoice > HARD);

    s.level = static_cast<Difficulty>(diffChoice);

    do
    {
        cout << "Number of puzzles solved: ";
        cin >> s.puzzlesSolved;
    } while (s.puzzlesSolved <= 0);

    for (int i = 0; i < MAX_TIMES; i++)
    {
        do
        {
            cout << "Enter time for puzzle " << i + 1 << ": ";
            cin >> s.times[i];
        } while (s.times[i] <= 0);
    }

    cin.ignore();
    sessionCount++;
}

/* -------------------- Test-Friendly Add Session -------------------- */
bool PuzzleTracker::addSession(const PuzzleSession& s)
{
    if (sessionCount >= MAX_SESSIONS)
        return false;

    sessions[sessionCount] = s;
    sessionCount++;
    return true;
}

/* -------------------- Calculations -------------------- */
double PuzzleTracker::calculateAverageTime(const double times[], int size) const
{
    if (size <= 0)
        return 0.0;

    double total = 0.0;

    for (int i = 0; i < size; i++)
    {
        if (times[i] < 0)
            return 0.0;
        total += times[i];
    }

    return total / size;
}

/* -------------------- Enum Decision Logic -------------------- */
string PuzzleTracker::getDifficultyLabel(Difficulty d) const
{
    switch (d)
    {
    case EASY: return "Easy";
    case MEDIUM: return "Medium";
    case HARD: return "Hard";
    default: return "Unknown";
    }
}

/* -------------------- Struct / Array Processing -------------------- */
double PuzzleTracker::getAverageTimeForSession(int index) const
{
    if (index < 0 || index >= sessionCount)
        return 0.0;

    return calculateAverageTime(sessions[index].times, MAX_TIMES);
}

/* -------------------- Accessors -------------------- */
int PuzzleTracker::getSessionCount() const
{
    return sessionCount;
}

/* -------------------- Show Report -------------------- */
void PuzzleTracker::showReport() const
{
    cout << "\nPuzzle Session Report\n";
    cout << setw(15) << "Puzzle"
        << setw(10) << "Solved"
        << setw(15) << "Avg Time\n";

    for (int i = 0; i < sessionCount; i++)
    {
        cout << setw(15) << sessions[i].puzzleName
            << setw(10) << sessions[i].puzzlesSolved
            << setw(15) << fixed << setprecision(2)
            << calculateAverageTime(sessions[i].times, MAX_TIMES)
            << endl;
    }
}

/* -------------------- Save Report -------------------- */
void PuzzleTracker::saveReportToFile(const string& filename) const
{
    ofstream outFile(filename);

    if (!outFile)
    {
        cout << "Error opening file.\n";
        return;
    }

    outFile << "Puzzle Session Report\n";
    outFile << setw(15) << "Puzzle"
        << setw(10) << "Solved"
        << setw(15) << "Avg Time\n";

    for (int i = 0; i < sessionCount; i++)
    {
        outFile << setw(15) << sessions[i].puzzleName
            << setw(10) << sessions[i].puzzlesSolved
            << setw(15) << fixed << setprecision(2)
            << calculateAverageTime(sessions[i].times, MAX_TIMES)
            << endl;
    }

    outFile.close();
}

/* -------------------- Menu -------------------- */
void PuzzleTracker::showMenu()
{
    int choice;

    do
    {
        cout << "\nMenu\n";
        cout << "1. Add Puzzle Session\n";
        cout << "2. View Report\n";
        cout << "3. Save Report to File\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1: addSession(); break;
        case 2: showReport(); break;
        case 3: saveReportToFile("report.txt"); break;
        case 4: cout << "Exiting program.\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

/* ==================== MAIN (PROGRAM MODE) ==================== */
#ifndef RUN_TESTS
int main()
{
    PuzzleTracker tracker;
    tracker.showBanner();
    tracker.showMenu();
    return 0;
}
#endif

/* ==================== DOCTESTS ==================== */
#ifdef RUN_TESTS

TEST_CASE("Average calculation normal")
{
    PuzzleTracker t;
    double times[MAX_TIMES] = { 10,20,30,40,50 };
    CHECK(t.calculateAverageTime(times, MAX_TIMES) == doctest::Approx(30.0));
}

TEST_CASE("Average zero size")
{
    PuzzleTracker t;
    double times[1] = { 10 };
    CHECK(t.calculateAverageTime(times, 0) == 0.0);
}

TEST_CASE("Negative time guard")
{
    PuzzleTracker t;
    double times[3] = { 10,-5,20 };
    CHECK(t.calculateAverageTime(times, 3) == 0.0);
}

TEST_CASE("Enum decision logic")
{
    PuzzleTracker t;
    CHECK(t.getDifficultyLabel(EASY) == "Easy");
    CHECK(t.getDifficultyLabel(MEDIUM) == "Medium");
    CHECK(t.getDifficultyLabel(HARD) == "Hard");
}

TEST_CASE("Add session and count")
{
    PuzzleTracker t;
    PuzzleSession s = { "Sudoku", EASY, 3,{10,10,10,10,10} };
    CHECK(t.addSession(s));
    CHECK(t.getSessionCount() == 1);
}

#endif