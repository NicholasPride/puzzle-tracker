#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

/* =====================================================
   MODE SWITCH
   ===================================================== */
#define RUN_TESTS

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

/* =====================================================
   COMPOSITION CLASS
   ===================================================== */;
   class PuzzleCollection
   {
   private:
       string items[5];
       int count;

   public:
       PuzzleCollection() : count(0) {}

       bool addItem(const string& name)
       {
           if (count >= 5) return false;
           items[count++] = name;
           return true;
       }

       int getCount() const { return count; }

       // REQUIRED helper method
       bool isEmpty() const
       {
           return count == 0;
       }
   };

   /* =====================================================
      BASE CLASS
      ===================================================== */
   class Puzzle
   {
   protected:
       string name;
       Difficulty difficulty;
       int duration;   // REQUIRED int member

   public:
       Puzzle(string n = "Unknown", Difficulty d = EASY, int dur = 0)
           : name(n), difficulty(d), duration(dur) {
       }

       // Setters
       void setName(string n) { name = n; }
       void setDifficulty(Difficulty d) { difficulty = d; }
       void setDuration(int dur) { duration = dur; }

       // Getters
       string getName() const { return name; }
       Difficulty getDifficulty() const { return difficulty; }
       int getDuration() const { return duration; }

       virtual string getType() const
       {
           return "Generic Puzzle";
       }

       // REQUIRED print()
       virtual void print() const
       {
           cout << "Puzzle: " << name << endl;
           cout << "Difficulty: " << difficulty << endl;
           cout << "Duration: " << duration << " minutes\n";
       }
   };

   /* =====================================================
      DERIVED CLASS #1
      ===================================================== */
   class LogicPuzzle : public Puzzle
   {
   private:
       int cluesUsed;
       PuzzleCollection collection; // REQUIRED composition inside derived

   public:
       LogicPuzzle(string n = "Logic", Difficulty d = EASY, int dur = 0, int clues = 0)
           : Puzzle(n, d, dur), cluesUsed(clues) {
       }

       string getType() const override
       {
           return "Logic Puzzle";
       }

       int getCluesUsed() const { return cluesUsed; }

       void addLogicTool(const string& tool)
       {
           collection.addItem(tool);
       }

       // REQUIRED override print()
       void print() const override
       {
           Puzzle::print(); // call base
           cout << "Clues Used: " << cluesUsed << endl;
           cout << "Tools stored: " << collection.getCount() << endl;
       }
   };

   /* =====================================================
      DERIVED CLASS #2
      ===================================================== */
   class WordPuzzle : public Puzzle
   {
   private:
       int wordsFound;
       PuzzleCollection collection; // REQUIRED composition inside derived

   public:
       WordPuzzle(string n = "Word", Difficulty d = EASY, int dur = 0, int words = 0)
           : Puzzle(n, d, dur), wordsFound(words) {
       }

       string getType() const override
       {
           return "Word Puzzle";
       }

       int getWordsFound() const { return wordsFound; }

       void addWordHint(const string& hint)
       {
           collection.addItem(hint);
       }

       // REQUIRED override print()
       void print() const override
       {
           Puzzle::print(); // call base
           cout << "Words Found: " << wordsFound << endl;
           cout << "Hints stored: " << collection.getCount() << endl;
       }
   };

   /* =====================================================
      ORIGINAL PUZZLE TRACKER CLASS
      ===================================================== */
   class PuzzleTracker
   {
   private:
       PuzzleSession sessions[MAX_SESSIONS];
       int sessionCount;

   public:
       PuzzleTracker();

       bool addSession(const PuzzleSession& s);
       int getSessionCount() const;
       double calculateAverageTime(const double times[], int size) const;
   };

   /* -------------------- Constructor -------------------- */
   PuzzleTracker::PuzzleTracker()
   {
       sessionCount = 0;
   }

   /* -------------------- Add Session -------------------- */
   bool PuzzleTracker::addSession(const PuzzleSession& s)
   {
       if (sessionCount >= MAX_SESSIONS)
           return false;

       sessions[sessionCount++] = s;
       return true;
   }

   /* -------------------- Calculation -------------------- */
   double PuzzleTracker::calculateAverageTime(const double times[], int size) const
   {
       if (size <= 0)
           return 0.0;

       double total = 0;
       for (int i = 0; i < size; i++)
           total += times[i];

       return total / size;
   }

   /* -------------------- Accessor -------------------- */
   int PuzzleTracker::getSessionCount() const
   {
       return sessionCount;
   }

   /* ==================== MAIN ==================== */
#ifndef RUN_TESTS
   int main()
   {
       LogicPuzzle lp("Sudoku", MEDIUM, 25, 3);
       lp.addLogicTool("Grid Notes");
       lp.print();

       WordPuzzle wp("Crossword", HARD, 40, 15);
       wp.addWordHint("Dictionary");
       wp.print();

       return 0;
   }
#endif

   /* ==================== DOCTESTS ==================== */
#ifdef RUN_TESTS

   TEST_CASE("Base class type")
   {
       Puzzle p("Test", EASY, 10);
       CHECK(p.getType() == "Generic Puzzle");
   }

   TEST_CASE("LogicPuzzle inheritance")
   {
       LogicPuzzle lp("Sudoku", MEDIUM, 30, 3);
       CHECK(lp.getType() == "Logic Puzzle");
       CHECK(lp.getCluesUsed() == 3);
   }

   TEST_CASE("WordPuzzle inheritance")
   {
       WordPuzzle wp("Crossword", HARD, 45, 15);
       CHECK(wp.getType() == "Word Puzzle");
       CHECK(wp.getWordsFound() == 15);
   }

   TEST_CASE("Composition inside derived")
   {
       LogicPuzzle lp;
       lp.addLogicTool("Notes");
       CHECK(lp.getCluesUsed() == 0);
   }

   TEST_CASE("Average calculation normal")
   {
       PuzzleTracker t;
       double times[MAX_TIMES] = { 10,20,30,40,50 };
       CHECK(t.calculateAverageTime(times, MAX_TIMES) == doctest::Approx(30.0));
   }

   TEST_CASE("Add session and count")
   {
       PuzzleTracker t;
       PuzzleSession s = { "Sudoku", EASY, 3,{10,10,10,10,10} };
       CHECK(t.addSession(s));
       CHECK(t.getSessionCount() == 1);
   }

#endif
