# Puzzle Tracker (C++ with Inheritance, Composition, and Unit Tests)

[![C++ doctest (Windows)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml/badge.svg)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml)

## Overview

Puzzle Tracker is a C++ console application that allows users to record puzzle-solving sessions, calculate
average completion times, and generate formatted reports. It also demonstrates inheritance and composition, along with
automated unit testing using the doctest framework and GitHub workflow integration.

The design includes:

* A base class
* Two derived classes
* A composition class
* Unit tests validating constructors, getters/setters, inheritance behavior, and composition behavior

---

## Features

* Menu-driven console interface
* User input with validation
* Enum for puzzle difficulty levels
* Struct to model puzzle sessions
* Arrays to store puzzle completion times
* Base class with shared puzzle properties
* Derived classes extending puzzle behavior
* Composition class used inside derived classes
* Overridden print() functions demonstrating inheritance
* Calculations for averages with guard conditions
* Formatted console output using `setw` and `setprecision`
* File output to `report.txt`
* Unit testing with doctest
* GitHub Actions workflow and passing status badge

---

## Class Design (Inheritance & Composition)

### Base Class

**Puzzle**

* Data members:
  * string name
  * int duration
  * Difficulty enum
* Constructors:
  * default
  * parameterized
* Setters/getters
* virtual print() function
* protected members used by derived classes

### Derived Classes

**LogicPuzzle**

* Inherits from Puzzle
* Adds cluesUsed
* Contains composition object
* Overrides print() and calls base version

**WordPuzzle**

* Inherits from Puzzle
* Adds wordsFound
* Contains composition object
* Overrides print() and calls base version

### Composition Class

**PuzzleCollection**

* Stored inside derived classes
* Manages a collection of puzzles
* Includes:
  * constructors
  * getters/setters
  * helper method (e.g., isEmpty())

This demonstrates:

* “is-a” relationship ? inheritance
* “has-a” relationship ? composition

---

## Class Diagram

A Visual Studio Class Designer diagram illustrates:

* Base class (Puzzle)
* Derived classes (LogicPuzzle, WordPuzzle)
* Composition relationships inside derived classes

The class diagram file is included in the repository as part of the Visual Studio project files.

---

## Unit Tests

The project includes doctest unit tests that verify:

* Constructors initialize fields correctly
* Getters and setters work properly
* Derived classes correctly extend base behavior
* Base print() and derived print() relationships
* Composition class functionality
* Average calculation logic

Tests run automatically when the project is compiled in Test mode.

---

## Build Instructions

This project supports **two execution modes**, controlled by a
compile-time switch.

### Program Mode (Interactive)
Runs the interactive Puzzle Tracker program.

To run the program:
1. Open `main.cpp`
2. Ensure this line is **commented out**:
   ```cpp
   //#define RUN_TESTS
