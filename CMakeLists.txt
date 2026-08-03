cmake_minimum_required(VERSION 3.16)
project(quiz_and_examination_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(quiz_and_examination_system
    main.cpp
    src/Question.cpp
    src/MCQ.cpp
    src/TF.cpp
    src/InputValidator.cpp
    src/QuestionBank.cpp
    src/Quiz.cpp
    src/QuizManager.cpp
    src/DataFileManager.cpp
    src/QuizEngine.cpp
    src/QuizApp.cpp
)

target_include_directories(quiz_and_examination_system PRIVATE include)
