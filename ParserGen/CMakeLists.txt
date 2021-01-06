#set(CMAKE_CXX_COMPILER "clang++")

include_directories("../../bux/include" "../../CBrackets") # ${Boost_INCLUDE_DIRS})
#link_directories("../../lib/bux" "../CBrackets")
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wconversion -Wno-parentheses -Wno-unknown-pragmas -O3 -std=c++2a")
#string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wno-parentheses -Wno-unknown-pragmas -g3 -Og -std=c++2a")
set(COMMON_SRC
    BNFContext.cpp
    Parser.cpp ParseFile.cpp ParserGenBase.cpp
    Scanner.cpp
    )

add_executable(parsergen
    ActionMap.cpp
    Cfa.cpp
    main.cpp
    Output.cpp
    ${COMMON_SRC}
    )
#target_compile_options(parsergen PRIVATE -O0 -g3)
target_link_libraries(parsergen bracketPairing bux fmt)
#add_dependencies(parsergen bracketPairing bux)

add_executable(grammarstrip
    GrammarStrip.cpp
    ${COMMON_SRC}
    )
target_link_libraries(grammarstrip bracketPairing bux fmt)
#add_dependencies(grammarstrip bracketPairing bux)
