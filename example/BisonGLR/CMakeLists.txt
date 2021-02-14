add_executable(bisonglr
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_include_directories(bisonglr PRIVATE "../../../bux/include")
target_link_libraries(bisonglr bux fmt stdc++)
