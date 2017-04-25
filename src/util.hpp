#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <fstream>

#include <unistd.h>
#include <limits.h>

#define PI 3.14159

#define DEGREES_TO_RADIANS(d) ((d) * PI/180.f)
#define RADIANS_TO_DEGRESS(r) ((d) * 180.f/PI)

#define ARRAY_SIZE(ar) (sizeof((ar))/sizeof((ar)[0]))

#define RANDOM(min, max) ((min) + (rand() % (max - min + 1)))

// typedef unsigned int ld38_uint;
// typedef unsigned long ld38_ulong;

std::string read_file(char const * file_name);

#endif 