# Puzzle Tracker (C++ with Operator Overloading, Templates, Abstract Classes, Polymorphism, and Unit Tests)

[![C++ doctest (Windows)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml/badge.svg)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml)

## Overview

Puzzle Tracker is a C++ console application designed using:

- Abstract base classes
- Virtual functions and runtime polymorphism
- Operator overloading
- Function templates
- Class templates
- Dynamic memory allocation
- Manual dynamic array management (no STL containers)
- Automated unit testing with doctest
- CRT memory leak detection (Debug mode)
- GitHub Actions CI workflow

The design emphasizes extensibility, safe memory management, and clear object-oriented structure.

---

## Key Design Concepts

### Abstract Base Class

The `Puzzle` class is abstract.

It includes:
- `string name`
- `int duration`
- `Difficulty difficulty`

It provides:
- Virtual destructor
- Getter methods
- Pure virtual function:

```cpp
virtual string getCategory() const = 0;
```

- Virtual streaming helper:

```cpp
virtual void toStream(ostream&) const;
```

Because of this pure virtual function, `Puzzle` cannot be instantiated directly.

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

Two `LogicPuzzle` objects are equal when their meaningful identity fields match.

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
- Cleaning up all memory in destructor
- Providing indexed access
- Supporting overloaded add/remove operators

Demonstrates composition:

- `PuzzleManager` owns `DynamicArray<Puzzle*>`

---

## Operator Overloading

The application includes several overloaded operators.

### Equality Operator (`operator==`)

Implemented as a member of `LogicPuzzle`.

Used to compare two objects based on:

- Name
- Duration
- Difficulty
- Clues used

---

### Stream Insertion Operator (`operator<<`)

Implemented as a non-member function:

```cpp
ostream& operator<<(ostream& os, const Puzzle& p);
```

Internally calls the virtual `toStream()` method, ensuring:

- Correct derived behavior executes
- Proper polymorphic dispatch occurs

---

### Subscript Operator (`operator[]`)

Implemented in `PuzzleManager`:

```cpp
Puzzle* operator[](int index) const;
```

Behavior:

- Returns pointer at valid index
- Returns `nullptr` for invalid index
- No exceptions are thrown

This provides safe bounds checking.

---

### Add and Remove Operators (`+=`, `-=`)

```cpp
PuzzleManager& operator+=(Puzzle* p);
PuzzleManager& operator-=(int index);
```

`operator+=`
- Adds a puzzle to the container
- Resizes dynamically if necessary
- Returns `*this` (demonstrates explicit `this` pointer usage)

`operator-=`
- Removes puzzle by index
- Deletes removed object
- Shifts remaining elements
- Returns `*this`

---

## Templates

### Function Template

```cpp
template <typename T>
T getMax(T a, T b);
```

Demonstrates generic programming by working with multiple types,
such as:

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
- `add()`
- `remove()`
- `operator[]`
- `getSize()`

Instantiated inside `PuzzleManager` as:

```cpp
DynamicArray<Puzzle*> items;
```

No STL containers are used.

---

## Memory Management

All objects are allocated dynamically:

```cpp
manager += new LogicPuzzle(...);
```

Memory is properly released:

- When an item is removed
- In the `PuzzleManager` destructor

The `DynamicArray` template manages only its internal array,
not the objects stored inside it.

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

The project includes doctest test cases verifying:

### Equality Operator
- Equal objects compare true
- Different objects compare false

### Stream Operator
- Uses `std::ostringstream`
- Confirms correct formatted output

### Subscript Operator
- Valid index returns correct pointer
- Invalid index returns `nullptr`

### Add / Remove Operators
- Adding increases size
- Removing deletes and shifts correctly

### Function Template
- Works with multiple data types

### Class Template
- Stores values
- Resizes correctly
- Maintains size integrity

All tests pass when `RUN_TESTS` is enabled.

---

## Class Diagram

The repository includes a Visual Studio Class Designer file showing:

- Abstract `Puzzle`
- Derived `LogicPuzzle`
- Derived `WordPuzzle`
- Template `DynamicArray<T>`
- `PuzzleManager`
- Function template `getMax<T>()`

The diagram visually represents:

- Inheritance (open triangle)
- Composition (filled diamond)
- Polymorphic hierarchy
- Template instantiation

---

## Build Instructions

This project supports **two execution modes**, controlled by a
compile-time switch.

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
