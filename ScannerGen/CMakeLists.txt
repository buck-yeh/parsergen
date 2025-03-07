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
target_compile_features(scannergen PRIVATE cxx_std_23)
#target_compile_options(scannergen PRIVATE -O0 -g3)
if(Iconv_FOUND)
target_link_libraries(scannergen bracketPairing bux Iconv::Iconv stdc++)
else()
target_link_libraries(scannergen bracketPairing bux stdc++)
endif()

add_executable(handycharset
    HandyCharset.cpp
    spec2charset.cpp
    )
target_compile_features(handycharset PRIVATE cxx_std_23)
if(Iconv_FOUND)
target_link_libraries(handycharset bux Iconv::Iconv stdc++)
else()
target_link_libraries(handycharset bux stdc++)
endif()
