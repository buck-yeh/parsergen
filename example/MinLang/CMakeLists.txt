add_executable(minlang
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
#target_compile_options(minlang PRIVATE -O0 -g3)
target_link_libraries(minlang bux fmt stdc++)
