cmake_minimum_required(VERSION 3.10)
project(QuizAndExaminationSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CORE_SOURCES
    src/Question.cpp
    src/MCQ.cpp
    src/TF.cpp
    src/QuestionBank.cpp
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/DataFileManager.cpp
    src/InputValidator.cpp
    src/Quiz.cpp
    src/QuizApp.cpp
    src/QuizEngine.cpp
    src/QuizManager.cpp
    ${CORE_SOURCES}
)

add_executable(QuestionModuleTests
    tests/test_main.cpp
    tests/test_QB.cpp
    ${CORE_SOURCES}
)

enable_testing()
add_test(NAME QuestionModuleTests COMMAND QuestionModuleTests)
