cmake_minimum_required (VERSION 3.18)
cmake_policy(VERSION 3.18)

include(FetchContent)

FetchContent_Declare(
  FetchedBux
  GIT_REPOSITORY https://github.com/buck-yeh/bux.git
  GIT_TAG        main
  SOURCE_DIR     ../bux
)
FetchContent_Declare(
  FetchedCBrackets
  GIT_REPOSITORY https://github.com/buck-yeh/CBrackets.git
  GIT_TAG        main
  SOURCE_DIR     ../CBrackets
)
FetchContent_MakeAvailable(FetchedBux FetchedCBrackets)

project(ParserSuite)

IF(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU)
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wshadow -Wconversion -Ofast -std=c++2a")
    IF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("Clang")
        string(APPEND CMAKE_CXX_FLAGS " -Wno-potentially-evaluated-expression -Wno-deprecated-anon-enum-enum-conversion")
    ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message("GNU")
        #string(APPEND CMAKE_CXX_FLAGS " -Wno-maybe-uninitialized")
    ELSE()
        message("Else 1/2")
    ENDIF()
ELSE()
    message("Else 2/2")
ENDIF()

add_subdirectory (ParserGen)
add_subdirectory (ScannerGen)
add_subdirectory (example)
