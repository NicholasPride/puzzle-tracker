# Puzzle Tracker (C++ with Operator Overloading, Templates, Exceptions, Abstract Classes, Polymorphism, and Unit Tests)

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
- Manual dynamic memory management (no STL containers)
- Automated unit testing with doctest
- CRT memory leak detection (Debug mode)
- GitHub Actions CI workflow

The design emphasizes safe memory usage, robust error handling, and clear object-oriented structure.

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
- Releasing all allocated memory

Demonstrates composition:

- `PuzzleManager` owns `DynamicArray<Puzzle*>`

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
- Shifts remaining elements
- Decreases container size

If the index is invalid, a `PuzzleException` is thrown.

---

## Templates

### Function Template

```cpp
template <typename T>
T getMax(T a, T b);
```

Returns the larger of two values.

Demonstrates generic programming by working with multiple types, such as:

- `int`
- `double`

---

### Class Template: DynamicArray<T>

Replaces the previous raw pointer array implementation.

```cpp
template <typename T>
class DynamicArray
```

Features:

- Dynamic resizing
- Manual memory allocation
- Generic type storage
- `add()`
- `remove()`
- `operator[]`
- `getSize()`
- `resize()`

Instantiated inside `PuzzleManager` as:

```cpp
DynamicArray<Puzzle*> items;
```

No STL containers are used.

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
- Removing an invalid index

Example errors:

```
DynamicArray index out of bounds
DynamicArray removal index out of bounds
```

---

#### PuzzleManager

Exceptions also occur when:

- `operator[]` receives an invalid index
- `operator-=` attempts to remove an invalid item

This prevents invalid operations from silently failing.

---

## Memory Management

All objects are allocated dynamically:

```cpp
manager += new LogicPuzzle(...);
```

Memory is properly released:

- When an item is removed
- In the `PuzzleManager` destructor

The `DynamicArray` template manages only its internal array, not the objects stored inside it.

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

### Equality Operator 

- Verifies `LogicPuzzle` objects compare correctly using `operator==`

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

All tests pass when `RUN_TESTS` is enabled.

---

## Class Diagram

The repository includes a Visual Studio Class Designer file showing:

- Abstract class `Puzzle`
- Derived classes `LogicPuzzle` and `WordPuzzle`
- Template class `DynamicArray<T>`
- Manager class `PuzzleManager`
- Custom exception `PuzzleException`
- Function template `getMax<T>()`

The diagram visually represents:

- Inheritance (open triangle)
- Composition (filled diamond)
- Polymorphic hierarchy
- Template relationships
- Exception usage

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

### Program Mode (Interactive)
Runs the interactive Puzzle Tracker program.

To run the program:
1. Open `main.cpp`
2. Ensure this line is **commented out**:
   ```cpp
   //#define RUN_TESTS

---

## Continuous Integration

This repository includes:

- GitHub Actions workflow
- Automatic build and test execution
- Status badge showing passing/failing state

The badge at the top of this README reflects current CI status.
