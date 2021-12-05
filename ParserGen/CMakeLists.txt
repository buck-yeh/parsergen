#set(CMAKE_CXX_COMPILER_ID "Clang")

include_directories("../${DEPENDEE_ROOT}/bux/include" "../${DEPENDEE_ROOT}/CBrackets") # ${Boost_INCLUDE_DIRS})
string(APPEND CMAKE_CXX_FLAGS " -Wno-shadow")
#string(APPEND CMAKE_CXX_FLAGS " -g3 -Og")

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
target_link_libraries(parsergen bracketPairing bux fmt stdc++)
#add_dependencies(parsergen bracketPairing bux)

add_executable(grammarstrip
    GrammarStrip.cpp
    ${COMMON_SRC}
    )
target_link_libraries(grammarstrip bracketPairing bux fmt stdc++)
#add_dependencies(grammarstrip bracketPairing bux)
