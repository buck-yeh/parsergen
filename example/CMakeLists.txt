IF(NOT DEFINED FETCH_DEPENDEES)
    string(APPEND CMAKE_CXX_FLAGS " -I../../${DEPENDEE_ROOT}/bux/include -L../../${DEPENDEE_ROOT}/bux/src")
ENDIF()

add_subdirectory (MinLang)
add_subdirectory (CalcInt)
add_subdirectory (BisonGLR)
