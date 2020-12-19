#set(CMAKE_CXX_COMPILER "clang++")

#include(FindPkgConfig)
#pkg_check_modules(ncurses REQUIRED ncurses)

#include(FindBoost)
#set(Boost_USE_STATIC_LIBS       OFF)
#set(Boost_USE_MULTITHREADED     ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)
#find_package( Boost REQUIRED COMPONENTS system )

include_directories("../../bux/include" "../../CBrackets")
#link_directories("../../bux/src" "../CBrackets")
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wshadow -Wconversion -Wno-parentheses -Wno-unknown-pragmas -Wno-maybe-uninitialized -Ofast -std=c++2a")

add_executable(scannergen
    Context.cpp
    main.cpp
    Parser.cpp
    Scanner.cpp
    spec2charset.cpp
    )
#target_compile_options(scannergen PRIVATE -O0 -g3)
target_link_libraries(scannergen bracketPairing Bux fmt)

add_executable(handycharset
    HandyCharset.cpp
    spec2charset.cpp
    )
target_link_libraries(handycharset Bux fmt)
