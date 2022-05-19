IF(DEFINED FETCH_DEPENDEES)
    string(APPEND CMAKE_CXX_FLAGS " -I../../${DEPENDEE_ROOT}/bux -I../../${DEPENDEE_ROOT}/CBrackets")
ENDIF()

add_subdirectory (MinLang)
add_subdirectory (CalcInt)
add_subdirectory (BisonGLR)
