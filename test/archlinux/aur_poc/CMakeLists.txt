cmake_minimum_required (VERSION 3.18)
cmake_policy(VERSION 3.18)
project(aur_poc)

IF(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU)
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wshadow -Wconversion")
    IF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("Clang")
        string(APPEND CMAKE_CXX_FLAGS " -Wno-potentially-evaluated-expression -Wno-deprecated-anon-enum-enum-conversion")
    ELSE()
        message("Not Clang")
    ENDIF()
ELSE()
    message("Not Clang/GNU")
ENDIF()

add_executable(calci
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_compile_features(calci PRIVATE cxx_std_23)
target_link_libraries(calci bux stdc++)
