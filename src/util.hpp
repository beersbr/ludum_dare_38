#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <fstream>

#include <unistd.h>
#include <limits.h>

#define ARRAY_SIZE(ar) (sizeof((ar))/sizeof((ar)[0]))

std::string read_file(char const * file_name);

#endif 