#include "util.hpp"

std::string read_file(char const * file_name)
{
    std::string file_contents;
    std::ifstream file(file_name, std::ios::in);

    if ( !file.good() ) {
        #ifdef SLOW
        std::cout << "[ERROR] Could not read the file given: " << file_name << std::endl;
        return "";
        #endif
    }

    file.seekg(0, std::ios::end);
    file_contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&file_contents[0], file_contents.size());
    file.close();

    return file_contents;
}