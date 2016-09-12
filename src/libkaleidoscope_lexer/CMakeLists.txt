add_library(kaleidoscope_lexer lexer.cpp)

install(TARGETS kaleidoscope_lexer DESTINATION lib)
install(FILES lexer.h DESTINATION include)
