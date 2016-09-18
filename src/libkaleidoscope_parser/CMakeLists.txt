add_library(kaleidoscope_parser ast.cpp expression.cpp prototype.cpp utils.cpp)

install(TARGETS kaleidoscope_parser DESTINATION lib)
install(FILES parser.h DESTINATION include)
