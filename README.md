# Quiz and Examination System

A C++11 console-based **Quiz and Examination System** demonstrating Object-Oriented Programming, custom data structures, file persistence, and automated testing with **Google Test**.

## Build

### Requirements

* C++11 compatible compiler: GCC, Clang, or MSVC
* CMake 3.14+

### Compile

Run from the project root:

```cmd
mkdir build
cd build
cmake ..
cmake --build .
```

## Testing

The project contains three testing levels:

* **Unit Tests** — individual classes/components
* **Integration Tests** — interaction between components
* **System / E2E Tests** — complete application flows

### Run All Tests

From the `build` directory:

```cmd
ctest --output-on-failure
```

### Run Tests Separately

**Command Prompt:**

```cmd
Debug\run_unit_tests.exe
Debug\run_integration_tests.exe
Debug\run_system_tests.exe
```

**PowerShell:**

```powershell
.\Debug\run_unit_tests.exe
.\Debug\run_integration_tests.exe
.\Debug\run_system_tests.exe
```

## Google Test Filtering

Use `--gtest_filter` to execute selected test suites or test cases.

### Run one test

```cmd
Debug\run_unit_tests.exe --gtest_filter=QuizEngineTest.Submit_CorrectMCQ_WrongTF_ScoreIs2of3_TC11
```

### Run an entire test suite

```cmd
Debug\run_unit_tests.exe --gtest_filter=QuizEngineTest.*
```

### Run tests matching a pattern

```cmd
Debug\run_unit_tests.exe --gtest_filter=*TC12*
```

### Run multiple suites

```cmd
Debug\run_unit_tests.exe --gtest_filter=QuestionBankTest.*:QuizEngineTest.*
```

### Exclude selected tests

```cmd
Debug\run_unit_tests.exe --gtest_filter=QuizEngineTest.*-*TC14*
```

> In PowerShell, prepend `.\` to executable paths.

## Running the Application

After building successfully, run the main quiz application from the `build` directory:

**Command Prompt:**

```cmd
Debug\quiz_app.exe
```

**PowerShell:**

```powershell
.\Debug\quiz_app.exe
```

> If your executable has a different name, replace `quiz_app.exe` with the executable name defined in `CMakeLists.txt`.
