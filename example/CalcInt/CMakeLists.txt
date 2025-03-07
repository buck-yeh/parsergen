add_executable(calci
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_link_libraries(calci bux stdc++)
target_compile_features(calci PRIVATE cxx_std_23)
