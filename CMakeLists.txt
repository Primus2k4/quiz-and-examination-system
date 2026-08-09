cmake_minimum_required(VERSION 3.14)

project(QuizAndExaminationSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)

# 1. Include directory for header files (.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 2. Collect ALL core logic files in src/ (src/ contains 100% pure business logic)
file(GLOB LOGIC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# 3. Executable Target for the Main Application (Links main.cpp at root with all LOGIC_SOURCES)
add_executable(QuizApp "main.cpp" ${LOGIC_SOURCES})

# ===================================================================
# GOOGLE TEST v1.10.0 INTEGRATION (LINK DIRECTLY TO LOGIC)
# ===================================================================
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.10.0.tar.gz
)
# Prevent C Runtime Library mismatch on Windows / MSVC
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# ===================================================================
# 4. Target for UNIT TESTS (Excluding Legacy/Custom Dev Test Files)
# ===================================================================
file(GLOB UNIT_TEST_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

# Exclude legacy custom test files created by other devs to prevent duplicate main() or symbol conflicts
list(REMOVE_ITEM UNIT_TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_QB.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_QuizEngine.cpp"
)

if(UNIT_TEST_SOURCES)
    add_executable(run_unit_tests ${UNIT_TEST_SOURCES} ${LOGIC_SOURCES})
    target_link_libraries(run_unit_tests PRIVATE gtest_main)
endif()

# ===================================================================
# 5. Target for INTEGRATION TESTS
# ===================================================================
file(GLOB INT_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/*.cpp")
if(INT_TEST_SOURCES)
    add_executable(run_integration_tests ${INT_TEST_SOURCES} ${LOGIC_SOURCES})
    target_link_libraries(run_integration_tests PRIVATE gtest_main)
endif()

# ===================================================================
# 6. Target for SYSTEM / E2E TESTS
# ===================================================================
file(GLOB SYS_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/system/*.cpp")
if(SYS_TEST_SOURCES)
    add_executable(run_system_tests ${SYS_TEST_SOURCES} ${LOGIC_SOURCES})
    target_link_libraries(run_system_tests PRIVATE gtest_main)
endif()

# ===================================================================
# 7. OPTIONAL: Legacy Test Executable (Kept for other devs' comparison)
# ===================================================================
# Keeps the legacy test runner intact so other devs can still build their own test suite if needed
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_main.cpp")
    add_executable(LegacyQuestionModuleTests
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_QB.cpp"
        ${LOGIC_SOURCES}
    )
endif()

# Discover tests for CTest / Visual Studio Test Explorer
include(GoogleTest)
if(TARGET run_unit_tests)
    gtest_discover_tests(run_unit_tests)
endif()
if(TARGET run_integration_tests)
    gtest_discover_tests(run_integration_tests)
endif()
if(TARGET run_system_tests)
    gtest_discover_tests(run_system_tests)
endif()

# Custom target to quickly build and execute Unit Tests
if(TARGET run_unit_tests)
    add_custom_target(check
        COMMAND run_unit_tests
        DEPENDS run_unit_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "--> Compiling with C++11 and running Unit Tests..."
    )
endif()