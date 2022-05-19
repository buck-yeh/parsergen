IF(NOT DEFINED FETCH_DEPENDEES)
    string(APPEND CMAKE_CXX_FLAGS " -I../${DEPENDEE_ROOT}/bux/include -L../${DEPENDEE_ROOT}/bux/src")
    string(APPEND CMAKE_CXX_FLAGS " -I../${DEPENDEE_ROOT}/CBrackets -L../${DEPENDEE_ROOT}/CBrackets")
ENDIF()

add_executable(scannergen
    Context.cpp
    main.cpp
    Parser.cpp
    Scanner.cpp
    spec2charset.cpp
    )
#target_compile_options(scannergen PRIVATE -O0 -g3)
target_link_libraries(scannergen bracketPairing bux fmt stdc++)

add_executable(handycharset
    HandyCharset.cpp
    spec2charset.cpp
    )
target_link_libraries(handycharset bux fmt stdc++)
