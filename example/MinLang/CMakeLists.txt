add_executable(minlang
    main.cpp
    Parser.cpp
    Scanner.cpp
    )
#target_compile_options(minlang PRIVATE -O0 -g3)
target_include_directories(minlang PRIVATE "../../${DEPENDEE_ROOT}/bux/include")
target_link_libraries(minlang bux fmt stdc++)
