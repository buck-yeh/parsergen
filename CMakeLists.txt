cmake_minimum_required (VERSION 3.11)
cmake_policy(VERSION 3.11)

#set(CMAKE_CXX_COMPILER "clang++")

include(FetchContent)
FetchContent_Declare(
  FetchedCBrackets
  GIT_REPOSITORY https://github.com/buck-yeh/parsergen.git
  GIT_TAG        main
  SOURCE_DIR     CBrackets
)
FetchContent_MakeAvailable(FetchedCBrackets)

project(ParserSuite)

add_subdirectory (ParserGen)
add_subdirectory (ScannerGen)
