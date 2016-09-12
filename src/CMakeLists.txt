project (kaleidoscope)

include_directories ("${PROJECT_SOURCE_DIR}/libkaleidoscope_lexer")
add_subdirectory (libkaleidoscope_lexer)

include_directories ("${PROJECT_SOURCE_DIR}/libkaleidoscope_parser")
add_subdirectory (libkaleidoscope_parser)
