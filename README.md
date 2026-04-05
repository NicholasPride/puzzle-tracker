# Puzzle Tracker (C++ with Operator Overloading, Templates, Exceptions, Recursion, Abstract Classes, Polymorphism, Searching & Sorting Algorithms, Linked Lists, Iterators, and Unit Tests)

[![C++ doctest (Windows)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml/badge.svg)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml)

## Overview

Puzzle Tracker is a C++ console application designed using:

- Abstract base classes
- Virtual functions and runtime polymorphism
- Operator overloading
- Function templates
- Class templates
- Exception handling
- Custom exception classes
- Recursive functions
- Custom Linked List ADT
- Iterator implementation
- Searching algorithms
- Sorting algorithms
- Automated unit testing with doctest
- CRT memory leak detection (Debug mode)
- GitHub Actions CI workflow

The design emphasizes safe memory usage, algorithm implementation, and clear object-oriented structure.

---

## Key Design Concepts

### Abstract Base Class

The `Puzzle` class is abstract.

It defines properties common to all puzzle types:

- `string name`
- `int duration`
- `Difficulty difficulty`

Key feature:

```cpp
virtual string getCategory() const = 0;
```

Because this function is pure virtual, the `Puzzle` class cannot be instantiated directly.

The class also includes a virtual helper function used for polymorphic output:

```cpp
virtual void toStream(ostream&) const;
```

A virtual destructor ensures correct cleanup when deleting derived objects through base-class pointers.

---

### Derived Classes

Two concrete classes inherit from `Puzzle`:

#### LogicPuzzle

- Adds `int cluesUsed`
- Overrides `getCategory()`
- Overrides `toStream()`
- Implements

```cpp
bool operator==(const LogicPuzzle& other) const;
```

Two `LogicPuzzle` objects are considered equal when the following fields match:

- `name`
- `duration`
- `difficulty`
- `cluesUsed`

#### WordPuzzle

- Adds `int wordsFound`
- Overrides `getCategory()`
- Overrides `toStream()`

Both derived classes demonstrate:

- Inheritance ("is-a" relationship)
- Virtual function overriding
- Polymorphic behavior

---

### Manager Class (Dynamic Memory)

Responsible for:

- Owning all dynamically allocated `Puzzle` objects
- Deleting objects when removed
- Storing puzzle pointers
- Adding/removing puzzles
- Providing indexed access
- Searching/sorting puzzles
- Printing puzzle information
- Releasing all allocated memory

The container used is a custom linked list ADT:
```cpp
LinkedList<Puzzle*> items;
```

Demonstrates composition:

- `PuzzleManager` owns the puzzle objects and deletes them in its destructor.

---

## Linked List ADT

The application includes a custom linked list implementation.

The linked list is implemented as its own Abstract Data Type (ADT).

### Node Structure

Each node contains:

```cpp
struct Node
{
    T data;
    Node* next;
};
```

This structure stores:

  - The puzzle pointer
  - A pointer to the next node

Nodes form a chain:

```cpp
Node → Node → Node → nullptr
```

---

### Linked List Operations

The `LinkedList<T>` class implements the required operations.

#### Insert

Adds a node to the front of the list.

```cpp
void insertFront(T value);
```

---

#### Delete

Removes a node containing a specific value.

```cpp
bool deleteNode(T value);
```

Returns:

  - `true` if deletion occurred
  - `false` if the value was not found

---

#### Search

Locates a node in the list.

```cpp
bool search(T value);
```

---

#### Traverse / Print

Visits every node and displays the stored values.

```cpp
void print();
```

Traversal follows each `next` pointer until the end of the list.

---

## Iterator Implementation

The linked list also includes a custom iterator class.

The iterator allows safe traversal without exposing node internals.

```cpp
class Iterator
```

Capabilities include:

- Initialization at the front of the list
- Advancing to the next node
- Accessing current node data

Key methods:
```cpp
bool hasNext();
void next();
T getData();
```

The iterator is used inside the `PuzzleManager` destructor to delete all stored puzzle objects.

---

## Operator Overloading

The application includes several overloaded operators.

### Equality Operator (`operator==`)

Implemented as a member of `LogicPuzzle`.

```cpp
bool operator==(const LogicPuzzle& other) const;
```

Compares two objects based on their meaningful state.

---

### Stream Insertion Operator (`operator<<`)

Implemented as a non-member function:

```cpp
ostream& operator<<(ostream& os, const Puzzle& p);
```

Internally calls the virtual function,`p.toStream(os)`, ensuring:

- Correct derived behavior executes
- Proper polymorphic dispatch occurs

---

### Subscript Operator (`operator[]`)

Implemented in `PuzzleManager`:

```cpp
Puzzle* operator[](int index) const;
```

Behavior:

- Returns the puzzle pointer when the index is valid
- Throws a `PuzzleException` if the index is invalid

Examples of invalid cases:

- Negative index
- Index greater than or equal to container size

---

### Add and Remove Operators (`+=`, `-=`)

#### Add Operator

```cpp
PuzzleManager& operator+=(Puzzle* p);
```

Adds a puzzle to the container.

Returns `*this` so operations can be chained.

---

#### Remove Operator

```cpp
PuzzleManager& operator-=(int index);
```

Behavior:

- Deletes the puzzle at the given index
- Removes the node from the linked list
- Frees the puzzle memory

If the index is invalid, a `PuzzleException` is thrown.

---

## Templates

### Function Template

```cpp
template <typename T>
T getMax(T a, T b);
```

Returns the larger of two values.

Example usage:
```cpp
getMax(3,5);
getMax(3.5, 2.1);
```

Demonstrates generic programming by working with multiple types, such as:

- `int`
- `double`

---

### Class Template: DynamicArray<T>

The application also includes a reusable template container.

```cpp
template <typename T>
class DynamicArray
```

Features:

- Dynamic resizing
- Manual memory allocation
- Generic type storage
- `add()`
- `operator[]`
- `size()`

Example usage:

```cpp
DynamicArray<int> arr;
arr.add(10);
```

The class is included to demonstrate template programming concepts.

---

## Searching Algorithms

### Sequential Search

Sequential search scans the container from beginning to end.

```cpp
int sequentialSearch(const string& target);
```

Behavior:

1. Compare the target with each puzzle name.
2. Return the index if found.
3. Return `-1` if the puzzle does not exist.

Example:

```cpp
manager.sequentialSearch("Sudoku");
```

---

### Binary Search

Binary search operates on sorted data.

```cpp
int binarySearch(const string& target);
```

Steps performed:

1. Sort the container.
2. Compare the middle element.
3. Narrow the search range.

Binary search is faster than sequential search for large containers.

---

## Sorting Algorithm

### Bubble Sort

Bubble sort orders puzzles alphabetically by name.

```cpp
void bubbleSort();
```

Algorithm steps:

1. Compare adjacent puzzle names
2. Swap if they are out of order
3. Repeat until sorted

Example:

```cpp
manager.bubbleSort();
```

---

## Exception Handling

The program uses exception-based error handling to prevent invalid operations.

---

### Custom Exception Class

A custom exception class is defined:

```cpp
class PuzzleException : public runtime_error
```

This class inherits from `std::runtime_error` and stores an error message.

Example usage:

```cpp
throw PuzzleException("DynamicArray index out of bounds.");
```

The message can be accessed using:

```cpp
what()
```

---

### Where Exceptions Are Used

#### DynamicArray<T>

Exceptions are thrown when:

- Accessing an invalid index

Example errors:

```
DynamicArray index out of bounds
```

---

#### PuzzleManager

Exceptions also occur when:

- `operator[]` receives an invalid index
- `operator-=` attempts to remove an invalid item

This prevents invalid operations from silently failing.

---

## Recursion

The program includes a recursive member function implemented inside `PuzzleManager`.

Recursion allows a function to call itself until a stopping condition is reached.

---

### Recursive Function

```cpp
int countPuzzlesRecursive();
```

This function counts the number of puzzles stored in the manager.

It calls a helper function.

---

### Recursive Helper Function

```cpp
int countRecursiveHelper(int index);
```

---

### Base Case

This case stops recursion when the index reaches the container size.

```cpp
if(index >= items.size())
return 0;
```

---

### Recursive Case

The case counts the current puzzle and recursively calls itself.

```cpp
return 1 + countRecursiveHelper(index + 1);
```

Rules satisfied:

```
No loops used
Clear base case
Clear recursive case
Member function implementation
```

---

## Memory Management

All objects are allocated dynamically:

```cpp
manager += new LogicPuzzle(...);
```

Memory is properly released:

- When an item is removed
- In the `PuzzleManager` destructor

The destructor uses the linked list iterator to safely delete each puzzle object.

No memory leaks occur.

---

## CRT Memory Leak Detection (Debug Mode)

When compiled in Debug mode:

```cpp
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
```

Inside `main()`:

```cpp
_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
```

Behavior:

- Tracks all dynamic allocations
- Reports memory leaks at program exit
- No report indicates all memory was properly freed

---

## Unit Tests

The project includes doctest test cases verifying program behavior.

### operator[] Exception 

- Ensures invalid index access throws `PuzzleException`

### operator-= Exception 

- Ensures invalid removal throws `PuzzleException`

### Template Class Exception 

- Verifies `DynamicArray<T>` throws exceptions when accessing invalid indices

### Custom Exception 

- Confirms `PuzzleException::what()` returns the correct message

### Function Template 

- Ensures `getMax<T>()` works correctly with multiple types

### Equality Operator 

- Verifies `LogicPuzzle` objects compare correctly using `operator==`

### Recursive Function 

- Verifies that recursive counting works correctly.

Example:

```cpp
CHECK(manager.countPuzzlesRecursive() == 2);
```

### Searching Algorithms

- Ensures `std::vector` is scanned from beginning to end.

Sequential search:

```cpp
CHECK(manager.sequentialSearch("Sudoku") == 0);
```

- Ensures data sorting works correctly.

Binary search:

```cpp
CHECK(manager.binarySearch("Beta") != -1);
```

### Sorting Algorithm

- Verifies that puzzles are sorted alphabetically.

Example:

```cpp
CHECK(manager[0]->getName() == "APuzzle");
```

### Linked List Edge Cases

- Verifies the linked list behavior.

Insert into empty list:

```cpp
CHECK(list.search(5) == true);
```

Delete missing node:

```cpp
CHECK(list.deleteNode(10) == false);
```

Traverse empty list:

```cpp
list.print();
CHECK(true);
```

All tests pass when `RUN_TESTS` is enabled.

---

## Class Diagram

The repository includes a Visual Studio Class Designer file showing:

- Abstract class `Puzzle`
- Derived classes `LogicPuzzle` and `WordPuzzle`
- Template class `DynamicArray<T>`
- Linked list ADT `LinkedList<T>`
- Node structure `struct Node`
- Iterator class `Iterator`
- Manager class `PuzzleManager`
- Custom exception `PuzzleException`
- Function template `getMax<T>()`
- Recursive functions `countPuzzlesRecursive()` and `countRecursiveHelper()`
- Searching algorithms `sequentialSearch()` and `binarySearch()`
- Sorting algorithm `bubbleSort()`

The diagram visually represents:

- Inheritance (open triangle)
- Composition (filled diamond)
- Polymorphic hierarchy
- Template relationships

---

## Build Instructions

This project supports **two execution modes**, controlled by a compile-time switch.

### Test Mode
Runs the unit tests.

To run the tests:
1. Open `main.cpp`
2. Ensure this line is **commented**:

   ```cpp
   #define RUN_TESTS
   ```

### Program Mode (Interactive)
Runs the interactive Puzzle Tracker program.

To run the program:
1. Open `main.cpp`
2. Ensure this line is **commented out**:

   ```cpp
   //#define RUN_TESTS
   ```

---

## Continuous Integration

This repository includes:

- GitHub Actions workflow
- Automatic build and test execution
- Status badge showing passing/failing state

The badge at the top of this README reflects current CI status.
