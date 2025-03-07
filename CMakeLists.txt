cmake_minimum_required (VERSION 3.18)
cmake_policy(VERSION 3.18)

if(NOT DEFINED DEPENDEE_ROOT)
set(DEPENDEE_ROOT "..")
endif()

if(DEFINED FETCH_DEPENDEES)
include(FetchContent)

FetchContent_Declare(
  FetchedBux
  GIT_REPOSITORY https://github.com/buck-yeh/bux.git
  GIT_TAG        main
  SOURCE_DIR     ${DEPENDEE_ROOT}/bux
)
FetchContent_Declare(
  FetchedCBrackets
  GIT_REPOSITORY https://github.com/buck-yeh/CBrackets.git
  GIT_TAG        main
  SOURCE_DIR     ${DEPENDEE_ROOT}/CBrackets
)
FetchContent_MakeAvailable(FetchedBux FetchedCBrackets)
endif()

project(ParserSuite)

IF(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU)
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wshadow -Wconversion -Ofast")
    IF(DEFINED FETCH_DEPENDEES)
        string(APPEND CMAKE_CXX_FLAGS " -I${fetchedbux_SOURCE_DIR}/include -I${fetchedcbrackets_SOURCE_DIR}")
        string(APPEND CMAKE_CXX_FLAGS " -L${fetchedbux_SOURCE_DIR}/src -L${fetchedcbrackets_SOURCE_DIR}")
    ENDIF()

    IF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(DEBUG "Clang")
        string(APPEND CMAKE_CXX_FLAGS " -Wno-potentially-evaluated-expression -Wno-deprecated-anon-enum-enum-conversion")
    ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(DEBUG "GNU")
        #string(APPEND CMAKE_CXX_FLAGS " -Wno-maybe-uninitialized")
    ELSE()
        message(DEBUG "Else 1/2")
    ENDIF()
ELSE()
    message(DEBUG "Else 2/2")
ENDIF()
message(DEBUG "Root/CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

add_subdirectory (ParserGen)
add_subdirectory (ScannerGen)
add_subdirectory (example)
