cmake_minimum_required (VERSION 3.19)
cmake_policy(VERSION 3.19)

#set(CMAKE_CXX_COMPILER "clang++")

include(FetchContent)

FetchContent_Declare(
  FetchedBux
  GIT_REPOSITORY https://github.com/buck-yeh/bux.git
  GIT_TAG        main
  SOURCE_DIR     bux
)
FetchContent_Declare(
  FetchedCBrackets
  GIT_REPOSITORY https://github.com/buck-yeh/CBrackets.git
  GIT_TAG        main
  SOURCE_DIR     CBrackets
)
FetchContent_MakeAvailable(FetchedBux FetchedCBrackets)

project(ParserSuite)

add_subdirectory (ParserGen)
add_subdirectory (ScannerGen)
