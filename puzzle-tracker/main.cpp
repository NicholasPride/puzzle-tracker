// Uncomment this line to run unit tests 
#define RUN_TESTS

#ifdef RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>
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
template <typename T>
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
    Puzzle(string n = "Unknown",
        int dur = 0,
        Difficulty diff = EASY)
        : name(n), duration(dur), difficulty(diff) {
    }

    virtual ~Puzzle() {}

    string getName() const { return name; }
    int getDuration() const { return duration; }
    Difficulty getDifficulty() const { return difficulty; }

    virtual string getCategory() const = 0;

    virtual void toStream(ostream& os) const
    {
        os << "Name: " << name
            << ", Duration: " << duration
            << ", Difficulty: " << difficulty;
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

    int getCluesUsed() const { return cluesUsed; }

    string getCategory() const override
    {
        return "Logic";
    }

    void toStream(ostream& os) const override
    {
        Puzzle::toStream(os);
        os << ", Clues Used: " << cluesUsed;
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
    WordPuzzle(string n,
        int dur,
        Difficulty diff,
        int words)
        : Puzzle(n, dur, diff), wordsFound(words) {
    }

    int getWordsFound() const { return wordsFound; }

    string getCategory() const override
    {
        return "Word";
    }

    void toStream(ostream& os) const override
    {
        Puzzle::toStream(os);
        os << ", Words Found: " << wordsFound;
    }
};

/* ===========================
   CLASS TEMPLATE (DynamicArray)
   =========================== */
template <typename T>
class DynamicArray
{
private:
    T* data;
    int size;
    int capacity;

    void resize()
    {
        capacity *= 2;
        T* temp = new T[capacity];

        for (int i = 0; i < size; i++)
            temp[i] = data[i];

        delete[] data;
        data = temp;
    }

public:
    DynamicArray(int cap = 5)
        : size(0), capacity(cap)
    {
        data = new T[capacity];
    }

    ~DynamicArray()
    {
        delete[] data;
    }

    void add(T item)
    {
        if (size >= capacity)
            resize();

        data[size++] = item;
    }

    void remove(int index)
    {
        if (index < 0 || index >= size)
            throw PuzzleException("DynamicArray removal index out of bounds.");

        for (int i = index; i < size - 1; i++)
            data[i] = data[i + 1];

        size--;
    }

    T operator[](int index) const
    {
        if (index < 0 || index >= size)
            throw PuzzleException("DynamicArray index out of bounds.");

        return data[index];
    }

    int getSize() const { return size; }
};

/* ===========================
   MANAGER CLASS
   =========================== */
class PuzzleManager
{
private:
    DynamicArray<Puzzle*> items;

public:
    void add(Puzzle* p)
    {
        items.add(p);
    }

    /* operator[] */
    Puzzle* operator[](int index) const
    {
        return items[index];  // Will throw if invalid
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
        Puzzle* toDelete = items[index]; // throws if invalid
        delete toDelete;
        items.remove(index);             // throws if invalid
        return *this;
    }

    void printAll() const
    {
        for (int i = 0; i < items.getSize(); i++)
        {
            cout << *items[i]
                << " | Category: "
                << items[i]->getCategory()
                << endl;
        }
    }

    int getSize() const
    {
        return items.getSize();
    }

    ~PuzzleManager()
    {
        for (int i = 0; i < items.getSize(); i++)
            delete items[i];
    }
};

#ifndef RUN_TESTS

int main()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);
    manager += new WordPuzzle("Crossword", 20, EASY, 15);

    manager.printAll();

    manager -= 0;

    cout << "\nAfter removal:\n";
    manager.printAll();

    return 0;
}

#else

/* ===========================
   TEST MODE
   =========================== */

TEST_CASE("Equality operator works")
{
    LogicPuzzle a("Sudoku", 30, MEDIUM, 3);
    LogicPuzzle b("Sudoku", 30, MEDIUM, 3);
    LogicPuzzle c("Sudoku", 30, MEDIUM, 4);

    CHECK(a == b);
    CHECK_FALSE(a == c);
}

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
    try {
        throw PuzzleException("Test message");
    }
    catch (const PuzzleException& ex) {
        CHECK(string(ex.what()) == "Test message");
    }
}

TEST_CASE("Function template works")
{
    CHECK(getMax(3, 5) == 5);
    CHECK(getMax(3.5, 2.1) == 3.5);
}

#endif
