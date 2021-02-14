#set(CMAKE_CXX_COMPILER_ID "Clang")

include_directories("../../bux/include" "../../CBrackets")

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
