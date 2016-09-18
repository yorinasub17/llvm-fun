add_library(kaleidoscope_parser ast.cpp parser.cpp)

install(TARGETS kaleidoscope_parser DESTINATION lib)
install(FILES parser.h DESTINATION include)
