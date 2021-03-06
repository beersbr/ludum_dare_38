#include "graphics.hpp"

std::map<std::string, shader_t> GFX_SHADERS;
std::map<std::string, model_t> GFX_MODELS;
std::map<std::string, texture_t> GFX_TEXTURES;

void use_shader(shader_t *shader)
{
    static shader_t *current_shader = NULL;

    assert(shader);

    if ( shader != current_shader ) {
        glUseProgram(shader->id);
        shader->in_use = true;
        if ( current_shader ) {
            current_shader->in_use = false;
        }
        current_shader = shader;
    }
}


GLint get_uniform_location(shader_t *shader, std::string name)
{

    GLint location = -1;

    if ( shader->uniform_locations.find(name) == shader->uniform_locations.end() ) {
        location = glGetUniformLocation(shader->id, name.c_str());
        shader->uniform_locations[name] = location;
    }
    else { 
        location = shader->uniform_locations[name];
    }

    return location;
}


void draw_model(model_t *model) 
{
    // glBindVertexArray(model->VAO);
    glDrawArrays(GL_TRIANGLES, 0, model->mesh.vertices.size());
    // glBindVertexArray(0);
}


GLuint create_shader(GLenum shader_type, std::string shader_source)
{
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
        std::cout << "[ERROR] shader compilation error in shader " << shader_type << std::endl << &log[0] << std::endl;
        #endif

        return 0;
    }

    return shader;
}


void create_shader_program(shader_t *shader, char const * vertex_shader_path, char const * fragment_shader_path)
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
    assert(vertex_shader);


    #ifdef SLOW 
    std::cout << "compiling fragment shader: " << std::endl << fragment_shader_source << std::endl;
    #endif
    
    GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    assert(fragment_shader);

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


void create_model(model_t *model, mesh_t mesh, shader_t *shader, texture_t *texture)
{
    static int ids = 0;

    assert(model);
    assert(shader);

    model->id      = ++ids;
    model->mesh    = mesh;
    model->shader  = shader;
    model->texture = texture;

    glGenVertexArrays(1, &model->VAO);

    #ifdef SLOW
    if ( !model->VAO ) {
        std::cout << "[ERROR] Could not create VAO for model object" << std::endl;
        assert(model->VAO);
    }
    #endif

    assert(model->VAO);

    glBindVertexArray(model->VAO);
    glGenBuffers(1, &model->VBO);

    #ifdef SLOW
    if ( !model->VBO ) {
        std::cout << "[ERROR] Could not create VBO for model object" << std::endl;
        assert(model->VBO);
    }
    #endif

    assert(model->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 (model->mesh.vertices.size()*GFX_VERTEX_SIZE),
                 &model->mesh.vertices[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(SHADER_ATTRIBUTE_POSITION);
    glEnableVertexAttribArray(SHADER_ATTRIBUTE_NORMAL);
    glEnableVertexAttribArray(SHADER_ATTRIBUTE_COLOR);
    glEnableVertexAttribArray(SHADER_ATTRIBUTE_UV);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Winvalid-offsetof"

    glVertexAttribPointer(SHADER_ATTRIBUTE_POSITION,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          GFX_VERTEX_SIZE,
                          (GLvoid*)offsetof(vertex_definition_t, position));

    glVertexAttribPointer(SHADER_ATTRIBUTE_NORMAL,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          GFX_VERTEX_SIZE,
                          (GLvoid*)offsetof(vertex_definition_t, normal));

    glVertexAttribPointer(SHADER_ATTRIBUTE_COLOR,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          GFX_VERTEX_SIZE,
                          (GLvoid*)offsetof(vertex_definition_t, color));

    glVertexAttribPointer(SHADER_ATTRIBUTE_UV,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          GFX_VERTEX_SIZE,
                          (GLvoid*)offsetof(vertex_definition_t, uv));

    #pragma GCC diagnostic pop

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void create_texture(texture_t *texture, char const * image_path) 
{

    SDL_Surface *tmp_sdl_surface = IMG_Load(image_path);    
    create_texture_raw(texture,
                       tmp_sdl_surface->w,
                       tmp_sdl_surface->h,
                       tmp_sdl_surface->pixels);

}


void create_texture_raw(texture_t *texture,
                        unsigned long width,
                        unsigned long height,
                        GLvoid *data,
                        GLenum ex_internal_format,
                        GLenum ex_format,
                        GLenum ex_type,
                        GLenum ex_wrap )
{
    assert(texture); 

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture->id);
    assert(texture->id);

    texture->width  = width;
    texture->height = height;
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // NOTE(Brett):assume bytes are in the corect order for now
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 ex_internal_format,
                 texture->width,
                 texture->height,
                 0,
                 ex_format,
                 ex_type,
                 data);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void use_texture(texture_t *texture, GLint unit)
{
    assert(texture);
    glActiveTexture(GL_TEXTURE0+unit);
    glBindTexture(GL_TEXTURE_2D, texture->id);

}



