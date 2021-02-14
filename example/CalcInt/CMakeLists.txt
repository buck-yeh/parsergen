add_executable(calci
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_include_directories(calci PRIVATE "../../../bux/include")
target_link_libraries(calci bux fmt stdc++)
