cmake_minimum_required(VERSION 3.14)

project(QuizAndExaminationSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)

# Trỏ đến thư mục chứa file Header
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Gom tất cả các file mã nguồn logic (BỎ QUA main.cpp để dính vào runner test)
file(GLOB LOGIC_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Question.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MCQ.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/TF.cpp"
    # Thêm các file .cpp khác của nhóm bạn vào đây khi hoàn thành (ví dụ: QuestionBank.cpp, QuizEngine.cpp...)
)

# 1. Target Executable cho ứng dụng chính
add_executable(QuizApp "src/main.cpp" ${LOGIC_SOURCES})

# ===================================================================
# CẤU HÌNH GOOGLE TEST (LINK TRỰC TIẾP VÀO DỰ ÁN)
# ===================================================================
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.10.0.tar.gz
)
# Tránh xung đột C runtime library trên Windows / MSVC
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# 2. Target cho UNIT TEST (Test lẻ từng class)
file(GLOB UNIT_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/*.cpp")
if(UNIT_TEST_SOURCES)
    add_executable(run_unit_tests ${UNIT_TEST_SOURCES} ${LOGIC_SOURCES})
    target_link_libraries(run_unit_tests PRIVATE gtest_main)
endif()

# 3. Target cho INTEGRATION TEST (Test ghép nối các module)
file(GLOB INT_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/*.cpp")
if(INT_TEST_SOURCES)
    add_executable(run_integration_tests ${INT_TEST_SOURCES} ${LOGIC_SOURCES})
    target_link_libraries(run_integration_tests PRIVATE gtest_main)
endif()

# 4. Target cho SYSTEM / E2E TEST (Test toàn bộ luồng)
file(GLOB SYS_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/system/*.cpp")
if(SYS_TEST_SOURCES)
    add_executable(run_system_tests ${SYS_TEST_SOURCES} ${LOGIC_SOURCES})
    target_link_libraries(run_system_tests PRIVATE gtest_main)
endif()

# Tự động phát hiện các bài test cho ctest
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