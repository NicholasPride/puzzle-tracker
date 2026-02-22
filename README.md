# Puzzle Tracker (C++ with Unit Tests, Abstract Classes, Polymorphism, and Dynamic Memory)

[![C++ doctest (Windows)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml/badge.svg)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml)

## Overview

Puzzle Tracker is a C++ console application designed using:

- Abstract base classes
- Virtual functions
- Runtime polymorphism
- Dynamic memory allocation
- Manual dynamic array management (no STL containers)
- Automated unit testing with doctests
- GitHub Actions CI workflow

The design supports extensibility and safe memory management.

---

## Key Design Concepts

### Abstract Base Class

The `Puzzle` class is abstract.

It includes:
- `string name`
- `int duration`
- `Difficulty difficulty`

It provides:
- Default and parameterized constructor
- Virtual destructor
- Getters and setters
- Virtual `print()` function
- Pure virtual function:

```cpp
virtual string getCategory() const = 0;
```

Because of this pure virtual function, `Puzzle` cannot be expressed directly.

---

### Derived Classes

Two concrete classes inherit from `Puzzle`:

#### LogicPuzzle
- Adds `int cluesUsed`
- Overrides `getCategory()`
- Overrides `print()` and calls `Puzzle::print()`

#### WordPuzzle
- Adds `int wordsFound`
- Overrides `getCategory()`
- Overrides `print()` and calls `Puzzle::print()`

This demonstrates:

- "is-a" relationship
- Virtual function overriding
- Polymorphic behavior

---

### Manager Class (Dynamic Memory)

The `PuzzleManager` class manages a dynamic array of base class pointers.

Required data members:

```cpp
Puzzle** items;
int size;
int capacity;
```

Features:

- Allocates memory dynamically
- Resizes array when capacity is reached
- Deletes all objects in destructor
- Deletes array memory safely
- Supports add/remove operations
- Demonstrates runtime polymorphism

Example of polymorphic call:

```cpp
items[i]->print();
items[i]->getCategory();
```

---

## Memory Management

All objects are allocated using `new`:

```cpp
manager.add(new LogicPuzzle(...));
```

All memory is properly cleaned up:

```cpp
~PuzzleManager()
{
    for (int i = 0; i < size; i++)
        delete items[i];

    delete[] items;
}
```

No memory leaks occur when program exits.

---

## CRT Memory Leak Detection (Debug Mode)

When compiled in **Debug mode**, the program enables CRT memory tracking:

```
#ifndef RUN_TESTS
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#endif
```

Inside `main()`:

```
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
```

Behavior:

- Tracks all dynamic allocations
- Automatically reports memory leaks at program termination
- Confirms all dynamically allocated memory is properly released

If no leaks exist, no leak report appears in the Output window.

---

## Unit Tests

The project includes six doctest test cases:

1. Constructor initializes derived correctly
2. Pure virtual override works
3. Polymorphism via base pointer
4. Manager adds items
5. Manager removes items
6. Dynamic polymorphic storage

These tests verify:

- Constructors
- Getter correctness
- Pure virtual function overrides
- Polymorphic dispatch
- Dynamic storage
- Add/remove behavior

All tests run automatically when `RUN_TESTS` is enabled.

---

## Class Diagram

The repository includes a Visual Studio Class Designer file showing:

- Abstract Puzzle base class
- LogicPuzzle and WordPuzzle inheritance
- PuzzleManager ownership of dynamic Puzzle pointers

The diagram visually represents:

- "is-a" inheritance relationships
- Polymorphic hierarchy
- Dynamic container ownership
- Destructor responsibility

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
