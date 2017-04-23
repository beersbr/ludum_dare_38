#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <OpenGL/gl3.h>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2_Image/SDL_image.h>

#include <cstddef>

#include "util.hpp"

#define WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define RED   glm::vec3(1.0f, 0.0f, 0.0f)
#define GREEN glm::vec3(0.0f, 1.0f, 0.0f)
#define BLUE  glm::vec3(0.0f, 0.0f, 1.0f)
#define BLACK glm::vec3(0.0f, 0.0f, 0.0f)

#define NEG_X glm::vec3(-1.f, 0.0f, 0.0f)
#define POS_X glm::vec3( 1.f, 0.0f, 0.0f)
#define NEG_Y glm::vec3(0.f, -1.0f, 0.0f)
#define POS_Y glm::vec3(0.f,  1.0f, 0.0f)
#define NEG_Z glm::vec3(0.f, 0.0f, -1.0f)
#define POS_Z glm::vec3(0.f, 0.0f,  1.0f)

#define SHADER_ATTRIBUTE_POSITION 0
#define SHADER_ATTRIBUTE_NORMAL 1
#define SHADER_ATTRIBUTE_COLOR 2
#define SHADER_ATTRIBUTE_UV 3


typedef struct _vertex_definition_t {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uv;
} vertex_definition_t;


#define GFX_VERTEX_SIZE sizeof(vertex_definition_t)


typedef struct _shader_t {
    GLuint id;
    bool in_use;

    std::map<std::string, GLint> uniform_locations;
} shader_t;


typedef struct _texture_t {
    GLuint id;

    bool in_use;

    unsigned int width;
    unsigned int height;
} texture_t;


typedef struct _mesh_t {
    std::vector<vertex_definition_t> vertices;
} mesh_t;


typedef struct _model_t {

    unsigned int id;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    shader_t *shader;
    texture_t *texture;

    GLuint VAO;
    GLuint VBO;

    mesh_t mesh;

} model_t;


typedef struct _shader_manager_t {


} shader_manager_t;

void use_shader(shader_t *shader);

GLint get_uniform_location(shader_t *shader, std::string name);

void create_shader_program(shader_t *shader, char const * vertex_shader, char const * fragment_shader);

void draw_model(model_t *model);

GLuint create_shader(GLenum type, char *shader_source);

void create_model(model_t *model, mesh_t mesh, shader_t *shader, texture_t *texture);

void create_texture(texture_t *texture, char const * image_path);

void create_texture_raw(texture_t *texture,
                        unsigned long width,
                        unsigned long height,
                        GLvoid *bytes,
                        GLenum ex_internal_format = GL_RGBA,
                        GLenum ex_format          = GL_BGRA,
                        GLenum ex_type            = GL_UNSIGNED_BYTE);

void use_texture(texture_t *texture, GLint unit);

#endif