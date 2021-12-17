#set(CMAKE_CXX_COMPILER_ID "Clang")

add_executable(scannergen
    Context.cpp
    main.cpp
    Parser.cpp
    Scanner.cpp
    spec2charset.cpp
    )
#target_compile_options(scannergen PRIVATE -O0 -g3)
target_include_directories(scannergen PRIVATE ../${DEPENDEE_ROOT}/bux/include ../${DEPENDEE_ROOT}/CBrackets)
target_link_directories(scannergen PRIVATE ../${DEPENDEE_ROOT}/bux/src ../${DEPENDEE_ROOT}/CBrackets)
target_link_libraries(scannergen bracketPairing bux fmt stdc++)

add_executable(handycharset
    HandyCharset.cpp
    spec2charset.cpp
    )
target_include_directories(handycharset PRIVATE ../${DEPENDEE_ROOT}/bux/include)
target_link_directories(handycharset PRIVATE ../${DEPENDEE_ROOT}/bux/src)
target_link_libraries(handycharset bux fmt stdc++)
