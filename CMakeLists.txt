cmake_minimum_required(VERSION 3.10)

# Define the project name and the languages used
project(QuizAndExaminationSystem VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the src directory to the include path so headers can be found
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Discover all source and header files in the src folder
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.h" "src/*.hpp")

# Create the executable targeting the discovered files
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} 

# Organize files into filters in the Visual Studio Solution Explorer
source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
