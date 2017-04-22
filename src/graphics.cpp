#include "graphics.hpp"

void use_shader(shader_t *shader)
{
    if ( !shader->inuse ) {
        glUseProgram(shader->id);
        shader->inuse = true;
    }
}


GLuint create_shader(GLenum shader_type, std::string shader_source) {
    GLuint shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, &shader_source.c_str(), &shader_source.size());

    glCompileShader(shader);

    int compilation_result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_result);
    if ( compilation_result == GL_FALSE ) {
        #ifdef SLOW
        int log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> log(log_length);
        glGetShaderInfoLog(shader, log_length, NULL, *log[0]);
        std::cout << "[ERROR] shader compilcation error in shader " << shader_type << std::endl << &log[0] << std::endl;
        #endif

        return 0;
    }
    return shader;
}


void create_shader_program(shader_t *shader, char *vertex_shader_path, char *fragment_shader_path)
{
    #ifdef SLOW 
    std::cout << "creating shader with: " << vertex_shader_path << " " << fragment_shader_path << std::endl;
    #endif

    
    

    #ifdef SLOW 
    std::cout << "shader compile successfully" << std::endl;
    #endif

}