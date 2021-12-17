add_executable(bisonglr
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
target_include_directories(bisonglr PRIVATE "../../${DEPENDEE_ROOT}/bux/include")
target_link_directories(grammarstrip PRIVATE "../../${DEPENDEE_ROOT}/bux/src")
target_link_libraries(bisonglr bux fmt stdc++)
