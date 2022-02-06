cmake_minimum_required(VERSION 3.14)
project(matrix)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include(FetchContent)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

include_directories(include)

enable_testing()

add_executable(matrix_test "test/matrix.cpp")

target_link_libraries(matrix_test
  PRIVATE
    gtest_main
    fmt::fmt
)

include(GoogleTest)
gtest_discover_tests(matrix_test)
