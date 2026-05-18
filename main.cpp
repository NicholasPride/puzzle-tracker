// Uncomment this line to run unit tests.
// Leave it commented out to run the normal program with the menu.
#define RUN_TESTS

#ifdef RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdexcept>
#include <map>

using namespace std;

// ===========================
// CRT MEMORY LEAK DETECTION
// ===========================
#ifndef RUN_TESTS
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#endif

/* ===========================
   CUSTOM EXCEPTION
   =========================== */
class PuzzleException : public runtime_error
{
public:
    PuzzleException(const string& message)
        : runtime_error(message)
    {
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
        : name(n), duration(d), difficulty(diff)
    {
    }

    virtual ~Puzzle()
    {
    }

    string getName() const
    {
        return name;
    }

    int getDuration() const
    {
        return duration;
    }

    Difficulty getDifficulty() const
    {
        return difficulty;
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
   DERIVED CLASS 1
   =========================== */
class LogicPuzzle : public Puzzle
{
private:
    int cluesUsed;

public:
    LogicPuzzle(string n, int d, Difficulty diff, int clues)
        : Puzzle(n, d, diff), cluesUsed(clues)
    {
    }

    string getCategory() const override
    {
        return "Logic";
    }

    int getCluesUsed() const
    {
        return cluesUsed;
    }

    void toStream(ostream& os) const override
    {
        os << left << setw(15) << name
            << setw(12) << "Logic"
            << setw(12) << duration
            << setw(12) << cluesUsed;
    }

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
        : Puzzle(n, d, diff), wordsFound(words)
    {
    }

    string getCategory() const override
    {
        return "Word";
    }

    int getWordsFound() const
    {
        return wordsFound;
    }

    void toStream(ostream& os) const override
    {
        os << left << setw(15) << name
            << setw(12) << "Word"
            << setw(12) << duration
            << setw(12) << wordsFound;
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
   ARRAY-BASED STACK CLASS
   =========================== */
template<typename T>
class PuzzleStack
{
private:
    T* data;
    int capacity;
    int topIndex;

public:
    PuzzleStack(int cap = 100)
    {
        if (cap <= 0)
            throw PuzzleException("Stack capacity must be positive");

        capacity = cap;
        topIndex = -1;
        data = new T[capacity];
    }

    ~PuzzleStack()
    {
        delete[] data;
    }

    bool isEmpty() const
    {
        return topIndex == -1;
    }

    bool isFull() const
    {
        return topIndex == capacity - 1;
    }

    void push(T value)
    {
        if (isFull())
            throw PuzzleException("Cannot push to a full stack");

        topIndex++;
        data[topIndex] = value;
    }

    void pop()
    {
        if (isEmpty())
            throw PuzzleException("Cannot pop from an empty stack");

        topIndex--;
    }

    T peek() const
    {
        if (isEmpty())
            throw PuzzleException("Cannot peek an empty stack");

        return data[topIndex];
    }

    int size() const
    {
        return topIndex + 1;
    }
};

/* ===========================
   ARRAY-BASED QUEUE CLASS
   =========================== */
template<typename T>
class PuzzleQueue
{
private:
    T* data;
    int capacity;
    int frontIndex;
    int rearIndex;
    int count;

public:
    PuzzleQueue(int cap = 100)
    {
        if (cap <= 0)
            throw PuzzleException("Queue capacity must be positive");

        capacity = cap;
        frontIndex = 0;
        rearIndex = -1;
        count = 0;
        data = new T[capacity];
    }

    ~PuzzleQueue()
    {
        delete[] data;
    }

    bool isEmpty() const
    {
        return count == 0;
    }

    bool isFull() const
    {
        return count == capacity;
    }

    void enqueue(T value)
    {
        if (isFull())
            throw PuzzleException("Cannot enqueue to a full queue");

        rearIndex = (rearIndex + 1) % capacity;
        data[rearIndex] = value;
        count++;
    }

    void dequeue()
    {
        if (isEmpty())
            throw PuzzleException("Cannot dequeue from an empty queue");

        frontIndex = (frontIndex + 1) % capacity;
        count--;
    }

    T front() const
    {
        if (isEmpty())
            throw PuzzleException("Cannot view front of an empty queue");

        return data[frontIndex];
    }

    int size() const
    {
        return count;
    }
};

/* ===========================
   MANAGER CLASS
   =========================== */
class PuzzleManager
{
private:
    LinkedList<Puzzle*> items;

    PuzzleStack<string> actionStack;

    PuzzleQueue<string> puzzleQueue;

    //parallel map for faster puzzle searching 
    map<string, Puzzle*> puzzleMap;

    int countRecursiveHelper(int index) const
    {
        if (index >= items.size())
            return 0;

        return 1 + countRecursiveHelper(index + 1);
    }

public:
    PuzzleManager()
        : actionStack(100), puzzleQueue(100)
    {
    }

    void showBanner() const
    {
        cout << "=====================================\n";
        cout << "        Welcome to Puzzle Tracker\n";
        cout << "=====================================\n";
    }

    void add(Puzzle* p)
    {
        items.insertBack(p);

        puzzleQueue.enqueue(p->getName());

        actionStack.push("Added " + p->getName());

        puzzleMap[p->getName()] = p;
    }

    Puzzle* operator[](int index) const
    {
        return items.getAtIndex(index);
    }

    PuzzleManager& operator+=(Puzzle* p)
    {
        add(p);
        return *this;
    }

    PuzzleManager& operator-=(int index)
    {
        Puzzle* p = items.getAtIndex(index);

        if (!p)
            throw PuzzleException("Invalid removal index");

        string removedName = p->getName();

        items.deleteNode(p);

        actionStack.push("Removed " + removedName);

        puzzleMap.erase(removedName);

        delete p;

        return *this;
    }

    int getSize() const
    {
        return items.size();
    }

    int getMapCount() const
    {
        return static_cast<int>(puzzleMap.size());
    }

    int countPuzzlesRecursive() const
    {
        return countRecursiveHelper(0);
    }

    string getDifficultyLabel(Difficulty d) const
    {
        switch (d)
        {
        case EASY:
            return "Easy";
        case MEDIUM:
            return "Medium";
        case HARD:
            return "Hard";
        default:
            return "Unknown";
        }
    }

    string peekLastAction() const
    {
        return actionStack.peek();
    }

    bool hasRecentActions() const
    {
        return !actionStack.isEmpty();
    }

    int getActionCount() const
    {
        return actionStack.size();
    }

    string frontPendingPuzzle() const
    {
        return puzzleQueue.front();
    }

    void removeFrontPendingPuzzle()
    {
        puzzleQueue.dequeue();
    }

    bool hasPendingPuzzles() const
    {
        return !puzzleQueue.isEmpty();
    }

    int getPendingPuzzleCount() const
    {
        return puzzleQueue.size();
    }

    Puzzle* mapLookup(const string& name) const
    {
        map<string, Puzzle*>::const_iterator it = puzzleMap.find(name);

        if (it != puzzleMap.end())
            return it->second;

        return nullptr;
    }

    void printMap() const
    {
        map<string, Puzzle*>::const_iterator it;

        for (it = puzzleMap.begin(); it != puzzleMap.end(); ++it)
        {
            cout << it->first << " -> " << it->second->getCategory() << endl;
        }
    }

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

    /* ===========================
       MENU FUNCTION
       =========================== */
    void showMenu()
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
            case 1:
                addPuzzleSession();
                break;

            case 2:
                showReport();
                break;

            case 3:
                saveReportToFile("report.txt");
                break;

            case 4:
                cout << "Exiting program.\n";
                break;

            default:
                cout << "Invalid choice.\n";
            }

        } while (choice != 4);
    }

    /* ===========================
       ADD SESSION FUNCTION
       =========================== */
    void addPuzzleSession()
    {
        int type;
        string name;
        int duration;
        int diffChoice;

        cout << "Enter puzzle name: ";
        getline(cin, name);

        while (name.empty())
        {
            cout << "Puzzle name cannot be empty. Try again: ";
            getline(cin, name);
        }

        cout << "Puzzle type (1=Logic, 2=Word): ";
        cin >> type;

        while (type < 1 || type > 2)
        {
            cout << "Invalid type. Puzzle type (1=Logic, 2=Word): ";
            cin >> type;
        }

        cout << "Duration in minutes: ";
        cin >> duration;

        while (duration <= 0)
        {
            cout << "Duration must be positive. Try again: ";
            cin >> duration;
        }

        cout << "Difficulty (1=Easy, 2=Medium, 3=Hard): ";
        cin >> diffChoice;

        while (diffChoice < EASY || diffChoice > HARD)
        {
            cout << "Invalid difficulty. Difficulty (1=Easy, 2=Medium, 3=Hard): ";
            cin >> diffChoice;
        }

        Difficulty diff = static_cast<Difficulty>(diffChoice);

        if (type == 1)
        {
            int clues;

            cout << "Clues used: ";
            cin >> clues;

            while (clues < 0)
            {
                cout << "Clues cannot be negative. Try again: ";
                cin >> clues;
            }

            *this += new LogicPuzzle(name, duration, diff, clues);
        }
        else
        {
            int words;

            cout << "Words found: ";
            cin >> words;

            while (words < 0)
            {
                cout << "Words cannot be negative. Try again: ";
                cin >> words;
            }

            *this += new WordPuzzle(name, duration, diff, words);
        }

        cin.ignore();

        cout << "Puzzle session added.\n";
    }

    /* ===========================
       REPORT FUNCTION
       =========================== */
    void showReport() const
    {
        cout << "\nPuzzle Session Report\n";

        if (getSize() == 0)
        {
            cout << "No puzzle sessions available.\n";
            return;
        }

        cout << left << setw(15) << "Puzzle"
            << setw(12) << "Category"
            << setw(12) << "Duration"
            << setw(12) << "Extra"
            << endl;

        cout << "---------------------------------------------------\n";

        printAllPuzzles();

        cout << "\nTotal puzzles: " << getSize() << endl;

        if (hasRecentActions())
        {
            cout << "Most recent action: " << peekLastAction() << endl;
        }

        if (hasPendingPuzzles())
        {
            cout << "First puzzle in queue: " << frontPendingPuzzle() << endl;
        }
    }

    /* ===========================
       SAVE REPORT FUNCTION
       =========================== */
    void saveReportToFile(const string& filename) const
    {
        ofstream outFile(filename);

        if (!outFile)
        {
            cout << "Error opening file.\n";
            return;
        }

        outFile << "Puzzle Session Report\n";

        if (getSize() == 0)
        {
            outFile << "No puzzle sessions available.\n";
            outFile.close();
            cout << "Report saved to " << filename << endl;
            return;
        }

        outFile << left << setw(15) << "Puzzle"
            << setw(12) << "Category"
            << setw(12) << "Duration"
            << setw(12) << "Extra"
            << endl;

        outFile << "---------------------------------------------------\n";

        typename LinkedList<Puzzle*>::Iterator it(items.getHead());

        while (it.hasNext())
        {
            Puzzle* p = it.getData();

            if (p)
                outFile << *p << endl;

            it.next();
        }

        outFile << "\nTotal puzzles: " << getSize() << endl;

        if (hasRecentActions())
        {
            outFile << "Most recent action: " << peekLastAction() << endl;
        }

        if (hasPendingPuzzles())
        {
            outFile << "First puzzle in queue: " << frontPendingPuzzle() << endl;
        }

        outFile.close();

        cout << "Report saved to " << filename << endl;
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

    manager.showBanner();
    manager.showMenu();

    return 0;
}

#else

/* ===========================
   TEST MODE
   =========================== */

TEST_CASE("Function template works")
{
    CHECK(getMax(3, 5) == 5);
    CHECK(getMax(7.5, 2.5) == doctest::Approx(7.5));
}

TEST_CASE("Custom exception what works")
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

TEST_CASE("Equality operator works")
{
    LogicPuzzle a("Sudoku", 30, MEDIUM, 3);
    LogicPuzzle b("Sudoku", 30, MEDIUM, 3);

    CHECK(a == b);
}

TEST_CASE("DynamicArray throws on invalid access")
{
    DynamicArray<int> arr;
    arr.add(10);

    CHECK(arr[0] == 10);
    CHECK_THROWS_AS(arr[2], PuzzleException);
}

TEST_CASE("LinkedList insert and search work")
{
    LinkedList<int> list;

    list.insertFront(5);
    list.insertBack(10);

    CHECK(list.search(5) == true);
    CHECK(list.search(10) == true);
    CHECK(list.search(20) == false);
    CHECK(list.size() == 2);
}

TEST_CASE("LinkedList delete missing node returns false")
{
    LinkedList<int> list;

    list.insertFront(5);

    CHECK(list.deleteNode(10) == false);
}

TEST_CASE("LinkedList invalid index throws")
{
    LinkedList<int> list;

    list.insertFront(5);

    CHECK_THROWS_AS(list.getAtIndex(2), PuzzleException);
}

TEST_CASE("Stack push peek and pop work")
{
    PuzzleStack<int> stack(3);

    CHECK(stack.isEmpty() == true);

    stack.push(10);
    stack.push(20);

    CHECK(stack.isEmpty() == false);
    CHECK(stack.peek() == 20);
    CHECK(stack.size() == 2);

    stack.pop();

    CHECK(stack.peek() == 10);
    CHECK(stack.size() == 1);
}

TEST_CASE("Pushing to a full stack throws exception")
{
    PuzzleStack<int> stack(2);

    stack.push(1);
    stack.push(2);

    CHECK_THROWS_AS(stack.push(3), PuzzleException);
}

TEST_CASE("Popping from an empty stack throws exception")
{
    PuzzleStack<int> stack(2);

    CHECK_THROWS_AS(stack.pop(), PuzzleException);
}

TEST_CASE("Peeking an empty stack throws exception")
{
    PuzzleStack<int> stack(2);

    CHECK_THROWS_AS(stack.peek(), PuzzleException);
}

TEST_CASE("Queue enqueue front and dequeue work")
{
    PuzzleQueue<string> queue(3);

    CHECK(queue.isEmpty() == true);

    queue.enqueue("Sudoku");
    queue.enqueue("Crossword");

    CHECK(queue.isEmpty() == false);
    CHECK(queue.front() == "Sudoku");
    CHECK(queue.size() == 2);

    queue.dequeue();

    CHECK(queue.front() == "Crossword");
    CHECK(queue.size() == 1);
}

TEST_CASE("Enqueue to a full queue throws exception")
{
    PuzzleQueue<int> queue(2);

    queue.enqueue(1);
    queue.enqueue(2);

    CHECK_THROWS_AS(queue.enqueue(3), PuzzleException);
}

TEST_CASE("Dequeue from an empty queue throws exception")
{
    PuzzleQueue<int> queue(2);

    CHECK_THROWS_AS(queue.dequeue(), PuzzleException);
}

TEST_CASE("Front on an empty queue throws exception")
{
    PuzzleQueue<int> queue(2);

    CHECK_THROWS_AS(queue.front(), PuzzleException);
}

TEST_CASE("Queue circular behavior works")
{
    PuzzleQueue<int> queue(3);

    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    CHECK(queue.front() == 1);

    queue.dequeue();

    CHECK(queue.front() == 2);

    queue.enqueue(4);

    CHECK(queue.size() == 3);
    CHECK(queue.front() == 2);
}

TEST_CASE("PuzzleManager add and count work")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);
    manager += new WordPuzzle("Crossword", 20, EASY, 10);

    CHECK(manager.getSize() == 2);
}

TEST_CASE("PuzzleManager operator[] throws on invalid index")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    CHECK(manager[0] != nullptr);
    CHECK_THROWS_AS(manager[5], PuzzleException);
}

TEST_CASE("PuzzleManager operator-= removes puzzle")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    manager -= 0;

    CHECK(manager.getSize() == 0);
}

TEST_CASE("PuzzleManager operator-= throws on invalid removal")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    CHECK_THROWS_AS(manager -= 5, PuzzleException);
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
    CHECK(manager.sequentialSearch("Crossword") == 1);
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

TEST_CASE("Binary search works after sorted data")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Alpha", 30, MEDIUM, 3);
    manager += new LogicPuzzle("Beta", 20, EASY, 2);
    manager += new LogicPuzzle("Gamma", 40, HARD, 1);

    CHECK(manager.binarySearch("Beta") != -1);
    CHECK(manager.binarySearch("Missing") == -1);
}

TEST_CASE("PuzzleManager uses stack and queue when adding puzzles")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    CHECK(manager.hasRecentActions() == true);
    CHECK(manager.peekLastAction() == "Added Sudoku");
    CHECK(manager.getActionCount() == 1);

    CHECK(manager.hasPendingPuzzles() == true);
    CHECK(manager.frontPendingPuzzle() == "Sudoku");
    CHECK(manager.getPendingPuzzleCount() == 1);
}

TEST_CASE("PuzzleManager stack tracks removed puzzle")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);
    manager -= 0;

    CHECK(manager.peekLastAction() == "Removed Sudoku");
    CHECK(manager.getActionCount() == 2);
}

TEST_CASE("PuzzleManager queue can remove front pending puzzle")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);
    manager += new WordPuzzle("Crossword", 20, EASY, 10);

    CHECK(manager.frontPendingPuzzle() == "Sudoku");

    manager.removeFrontPendingPuzzle();

    CHECK(manager.frontPendingPuzzle() == "Crossword");
    CHECK(manager.getPendingPuzzleCount() == 1);
}

TEST_CASE("Difficulty labels work")
{
    PuzzleManager manager;

    CHECK(manager.getDifficultyLabel(EASY) == "Easy");
    CHECK(manager.getDifficultyLabel(MEDIUM) == "Medium");
    CHECK(manager.getDifficultyLabel(HARD) == "Hard");
}

TEST_CASE("Map lookup finds added puzzle")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    CHECK(manager.mapLookup("Sudoku") != nullptr);
    CHECK(manager.mapLookup("Sudoku")->getCategory() == "Logic");
    CHECK(manager.mapLookup("Missing") == nullptr);
}

TEST_CASE("Map count matches added puzzles")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);
    manager += new WordPuzzle("Crossword", 20, EASY, 10);

    CHECK(manager.getMapCount() == 2);
}

TEST_CASE("Map removes puzzle after operator minus equals")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("Sudoku", 30, MEDIUM, 3);

    CHECK(manager.mapLookup("Sudoku") != nullptr);

    manager -= 0;

    CHECK(manager.mapLookup("Sudoku") == nullptr);
    CHECK(manager.getMapCount() == 0);
}

TEST_CASE("Map lookup still works after bubble sort")
{
    PuzzleManager manager;

    manager += new LogicPuzzle("ZPuzzle", 30, MEDIUM, 3);
    manager += new LogicPuzzle("APuzzle", 20, EASY, 2);

    manager.bubbleSort();

    CHECK(manager[0]->getName() == "APuzzle");
    CHECK(manager.mapLookup("ZPuzzle") != nullptr);
    CHECK(manager.mapLookup("APuzzle") != nullptr);
}

#endif
