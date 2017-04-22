#include "graphics.hpp"

void use_shader(shader_t *shader)
{
    static shader_t *current_shader = NULL;

    if ( shader != current_shader ) {

        glUseProgram(shader->id);
        shader->in_use = true;
        current_shader->in_use = false;
        current_shader = shader;
    }
}


GLuint create_shader(GLenum shader_type, std::string shader_source) {
    GLuint shader = glCreateShader(shader_type);

    char *source = (char*)shader_source.c_str();
    int size = shader_source.size();
    glShaderSource(shader, 1, (GLchar **)&source, &size);

    glCompileShader(shader);

    int compilation_result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_result);
    if ( compilation_result == GL_FALSE ) {
        #ifdef SLOW
        int log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> log(log_length);
        glGetShaderInfoLog(shader, log_length, NULL, (GLchar *)(&log[0]));
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

    std::string vertex_shader_source   = read_file(vertex_shader_path);
    std::string fragment_shader_source = read_file(fragment_shader_path);
    
    #ifdef SLOW 
    std::cout << "compiling vertex shader: " << std::endl << vertex_shader_source << std::endl;
    #endif

    GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);

    #ifdef SLOW 
    std::cout << "compiling fragment shader: " << std::endl << fragment_shader_source << std::endl;
    #endif

    GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    int link_result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &link_result);

    if ( link_result == GL_FALSE ) {
        #ifdef SLOW
        int log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> log(log_length);
        glGetProgramInfoLog(program, log_length, NULL, &log[0]);
        std::cout << "[ERROR] shader link error in shader: " <<  &log[0] << std::endl;
        #endif 

        return;
    }

    assert(shader);

    shader->id = program;
    shader->in_use = false;

    #ifdef SLOW 
    std::cout << "shader compile successfully" << std::endl;
    #endif

    return;

}