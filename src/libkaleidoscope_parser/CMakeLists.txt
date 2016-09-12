add_library(kaleidoscope_parser parser.cpp)

install(TARGETS kaleidoscope_parser DESTINATION lib)
install(FILES parser.h DESTINATION include)
