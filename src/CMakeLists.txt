project(asx86)

set(SRC
    encoding.cpp
    lex.cpp
    lex_debug.cpp
    main.cpp
    parser.cpp
    register.cpp
    symbol.cpp
)

add_executable(asx86 ${SRC})

target_link_libraries(asx86 elf)

