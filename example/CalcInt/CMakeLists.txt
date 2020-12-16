string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wshadow -Wconversion -std=c++2a")
set(CMAKE_BUILD_TYPE Debug)

add_executable(calci
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_include_directories(calci PRIVATE "../../../bux/include")
target_link_libraries(calci Bux)
