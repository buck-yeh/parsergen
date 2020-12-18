string(APPEND CMAKE_CXX_FLAGS " -Og -g3 -std=c++20")
set(CMAKE_BUILD_TYPE Debug)

add_executable(bisonglr
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_include_directories(bisonglr PRIVATE "../../../bux/include")
target_link_libraries(bisonglr Bux fmt)
