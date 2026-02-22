// Uncomment this line to run unit tests
#define RUN_TESTS

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

using namespace std;

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
    Puzzle(string n = "Unknown",
        int dur = 0,
        Difficulty diff = EASY)
        : name(n), duration(dur), difficulty(diff) {
    }

    virtual ~Puzzle() {}  // Virtual destructor REQUIRED

    void setName(const string& n) { name = n; }
    void setDuration(int d) { duration = d; }
    void setDifficulty(Difficulty diff) { difficulty = diff; }

    string getName() const { return name; }
    int getDuration() const { return duration; }
    Difficulty getDifficulty() const { return difficulty; }

    virtual void print() const
    {
        cout << "Name: " << name
            << ", Duration: " << duration
            << ", Difficulty: " << difficulty;
    }

    // PURE VIRTUAL FUNCTION (makes class abstract)
    virtual string getCategory() const = 0;
};

/* ===========================
   DERIVED CLASS 1
   =========================== */
class LogicPuzzle : public Puzzle
{
private:
    int cluesUsed;

public:
    LogicPuzzle(string n,
        int dur,
        Difficulty diff,
        int clues)
        : Puzzle(n, dur, diff), cluesUsed(clues) {
    }

    void setCluesUsed(int c) { cluesUsed = c; }
    int getCluesUsed() const { return cluesUsed; }

    string getCategory() const override
    {
        return "Logic";
    }

    void print() const override
    {
        Puzzle::print();
        cout << ", Clues Used: " << cluesUsed;
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
    WordPuzzle(string n,
        int dur,
        Difficulty diff,
        int words)
        : Puzzle(n, dur, diff), wordsFound(words) {
    }

    void setWordsFound(int w) { wordsFound = w; }
    int getWordsFound() const { return wordsFound; }

    string getCategory() const override
    {
        return "Word";
    }

    void print() const override
    {
        Puzzle::print();
        cout << ", Words Found: " << wordsFound;
    }
};

/* ===========================
   MANAGER CLASS (Dynamic Array)
   =========================== */
class PuzzleManager
{
private:
    Puzzle** items;
    int size;
    int capacity;

    void resize()
    {
        capacity *= 2;

        Puzzle** temp = new Puzzle * [capacity];

        for (int i = 0; i < size; i++)
            temp[i] = items[i];

        delete[] items;
        items = temp;
    }

public:
    PuzzleManager(int cap = 5)
    {
        capacity = cap;
        size = 0;
        items = new Puzzle * [capacity];
    }

    ~PuzzleManager()
    {
        for (int i = 0; i < size; i++)
            delete items[i];

        delete[] items;
    }

    void add(Puzzle* p)
    {
        if (size >= capacity)
            resize();

        items[size++] = p;
    }

    void remove(int index)
    {
        if (index < 0 || index >= size)
            return;

        delete items[index];

        for (int i = index; i < size - 1; i++)
            items[i] = items[i + 1];

        size--;
    }

    void printAll() const
    {
        for (int i = 0; i < size; i++)
        {
            items[i]->print();   // Polymorphism
            cout << " | Category: "
                << items[i]->getCategory()
                << endl;
        }
    }

    int getSize() const { return size; }
};

#ifndef RUN_TESTS

/* ===========================
   PROGRAM MODE
   =========================== */
int main()
{
#ifdef _DEBUG
    // Enable memory leak detection
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    PuzzleManager manager;

    manager.add(new LogicPuzzle("Sudoku", 30, MEDIUM, 3));
    manager.add(new WordPuzzle("Crossword", 20, EASY, 15));

    manager.printAll();

    manager.remove(0);

    cout << "\nAfter removal:\n";
    manager.printAll();

    return 0;
}
#else

/* ===========================
   TEST MODE
   =========================== */

TEST_CASE("Constructor initializes derived correctly")
{
    LogicPuzzle lp("Sudoku", 30, MEDIUM, 4);

    CHECK(lp.getName() == "Sudoku");
    CHECK(lp.getDuration() == 30);
    CHECK(lp.getDifficulty() == MEDIUM);
    CHECK(lp.getCluesUsed() == 4);
}

TEST_CASE("Pure virtual override works")
{
    LogicPuzzle lp("Sudoku", 30, MEDIUM, 4);
    WordPuzzle wp("Crossword", 20, EASY, 10);

    CHECK(lp.getCategory() == "Logic");
    CHECK(wp.getCategory() == "Word");
}

TEST_CASE("Polymorphism via base pointer")
{
    Puzzle* p = new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    CHECK(p->getCategory() == "Logic");

    delete p;
}

TEST_CASE("Manager adds items")
{
    PuzzleManager manager;

    manager.add(new LogicPuzzle("Sudoku", 30, MEDIUM, 3));
    manager.add(new WordPuzzle("Crossword", 20, EASY, 10));

    CHECK(manager.getSize() == 2);
}

TEST_CASE("Manager removes items")
{
    PuzzleManager manager;

    manager.add(new LogicPuzzle("Sudoku", 30, MEDIUM, 3));
    manager.add(new WordPuzzle("Crossword", 20, EASY, 10));

    manager.remove(0);

    CHECK(manager.getSize() == 1);
}

TEST_CASE("Dynamic polymorphic storage")
{
    PuzzleManager manager;

    manager.add(new LogicPuzzle("Sudoku", 30, MEDIUM, 3));

    CHECK(manager.getSize() == 1);
}

#endif

