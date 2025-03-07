#set(CMAKE_CXX_COMPILER_ID "Clang")

IF(NOT DEFINED FETCH_DEPENDEES)
    string(APPEND CMAKE_CXX_FLAGS " -I../${DEPENDEE_ROOT}/bux/include -L../${DEPENDEE_ROOT}/bux/src")
    string(APPEND CMAKE_CXX_FLAGS " -I../${DEPENDEE_ROOT}/CBrackets -L../${DEPENDEE_ROOT}/CBrackets")
ENDIF()

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
if(Iconv_FOUND)
target_link_libraries(parsergen bracketPairing bux Iconv::Iconv stdc++)
else()
target_link_libraries(parsergen bracketPairing bux stdc++)
endif()
target_compile_features(parsergen PRIVATE cxx_std_23)
#target_compile_options(parsergen PRIVATE -O0 -g3)
#add_dependencies(parsergen bracketPairing bux)

add_executable(grammarstrip
    GrammarStrip.cpp
    ${COMMON_SRC}
    )
if(Iconv_FOUND)
target_link_libraries(grammarstrip bracketPairing bux Iconv::Iconv stdc++)
else()
target_link_libraries(grammarstrip bracketPairing bux stdc++)
endif()
target_compile_features(grammarstrip PRIVATE cxx_std_23)
#add_dependencies(grammarstrip bracketPairing bux)
