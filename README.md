# Puzzle Tracker (C++ with Unit Tests)

[![C++ doctest (Windows)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml/badge.svg)](https://github.com/NicholasPride/puzzle-tracker/actions/workflows/tests.yml)

## Overview
Puzzle Tracker is a C++ console application that allows users to record puzzle-solving sessions, calculate
average completion times, and generate formatted reports.  

This project also includes **unit tests using the doctest framework**
to verify correctness of calculations, enum-based decision logic,
struct and array processing, and class methods.

---

## Features
- Menu-driven console interface
- User input with validation
- Enum for puzzle difficulty levels
- Struct to model puzzle sessions
- Arrays to store puzzle completion times
- Class-based design for encapsulation
- Calculations for averages with guard conditions
- Formatted console output using `setw` and `setprecision`
- File output to `report.txt`
- Unit testing with doctest

---

## Build Instructions

This project supports **two execution modes**, controlled by a
compile-time switch.

### Program Mode (Interactive)
- Runs the interactive Puzzle Tracker program
- Allows adding sessions, viewing reports, and saving to a file

To run the program:
1. Open `main.cpp`
2. Ensure this line is **commented out**:
   ```cpp
   //#define RUN_TESTS
