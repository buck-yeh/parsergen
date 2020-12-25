cmake_minimum_required (VERSION 3.18)
cmake_policy(VERSION 3.18)
project(aur_poc)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wshadow -Wconversion -std=c++2a")
#set(CMAKE_BUILD_TYPE Debug)

add_executable(calci
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_link_libraries(calci bux fmt)
