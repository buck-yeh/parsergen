string(APPEND CMAKE_CXX_FLAGS " -std=c++20")
set(CMAKE_BUILD_TYPE Debug)

add_executable(minlang
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_include_directories(minlang PRIVATE "../../../bux/include")
target_link_libraries(minlang bux fmt)
