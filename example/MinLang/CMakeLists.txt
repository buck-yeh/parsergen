add_executable(minlang
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
#target_compile_options(minlang PRIVATE -O0 -g3)
target_link_libraries(minlang bux stdc++)
target_compile_features(minlang PRIVATE cxx_std_23)
