#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <fstream>

typedef void (*update_function_callback)(void);
typedef void (*render_function_callback)(void);

std::string read_file(char *file_name);

#endif 