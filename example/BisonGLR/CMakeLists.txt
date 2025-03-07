add_executable(bisonglr
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_link_libraries(bisonglr bux stdc++)
target_compile_features(bisonglr PRIVATE cxx_std_23)
