add_executable(bisonglr
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_link_libraries(bisonglr bux stdc++)
