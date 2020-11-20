cmake_minimum_required (VERSION 2.8.1)
cmake_policy(VERSION 2.8.1)

include(FindPkgConfig)
#include(FindBoost)

#set(CMAKE_CXX_COMPILER "clang++")

#set(Boost_USE_STATIC_LIBS       OFF)
#set(Boost_USE_MULTITHREADED     ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)
#find_package( Boost REQUIRED COMPONENTS system )

#pkg_check_modules(ncurses REQUIRED ncurses)

include_directories("../../lib/bux" "../CBrackets") # ${Boost_INCLUDE_DIRS})
link_directories("../../lib/bux" "../CBrackets")
list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -Wno-parentheses -Wno-unknown-pragmas -O3 -std=c++2a")
#list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-parentheses -Wno-unknown-pragmas -g3 -Og -std=c++17")

add_executable(parsergen
    ActionMap.cpp
    BNFContext.cpp
    Cfa.cpp
    main.cpp
    Parser.cpp ParseFile.cpp ParserGenBase.cpp
    Scanner.cpp
    Output.cpp
    )
target_link_libraries(parsergen bracketPairing Bux fmt)
add_dependencies(parsergen bracketPairing Bux)

add_executable(grammarstrip
    GrammarStrip.cpp
    BNFContext.cpp
    Parser.cpp ParseFile.cpp ParserGenBase.cpp
    Scanner.cpp
    )
target_link_libraries(grammarstrip bracketPairing Bux fmt)
add_dependencies(grammarstrip bracketPairing Bux)
