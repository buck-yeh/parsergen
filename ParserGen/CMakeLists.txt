#set(CMAKE_CXX_COMPILER_ID "Clang")

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
target_include_directories(parsergen PRIVATE "../${DEPENDEE_ROOT}/bux/include" "../${DEPENDEE_ROOT}/CBrackets")
target_link_directories(parsergen PRIVATE "../${DEPENDEE_ROOT}/bux/src" "../${DEPENDEE_ROOT}/CBrackets")
target_link_libraries(parsergen bracketPairing bux fmt stdc++)
#add_dependencies(parsergen bracketPairing bux)

add_executable(grammarstrip
    GrammarStrip.cpp
    ${COMMON_SRC}
    )
target_include_directories(grammarstrip PRIVATE "../${DEPENDEE_ROOT}/bux/include" "../${DEPENDEE_ROOT}/CBrackets")
target_link_directories(grammarstrip PRIVATE "../${DEPENDEE_ROOT}/bux/src" "../${DEPENDEE_ROOT}/CBrackets")
target_link_libraries(grammarstrip bracketPairing bux fmt stdc++)
#add_dependencies(grammarstrip bracketPairing bux)
