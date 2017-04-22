#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <glm/glm.hpp>
#include <vector>

#include "util.hpp"

typedef struct _vertex_definition_t {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

} vertex_definition_t;

typedef struct _shader_t {
    GLuint id;
    short inuse;
} shader_t;


typedef struct _mesh_t {
    std::vector<vertex_definition_t> vertices;

} mesh_t;


typedef struct _model_t {

    glm::vec3 rotation;
    glm::vec3 position;
    glm::vec3 scale;

    GLuint VAO;
    GLuint VBO;

    shader_id

} model_t;

GLuint create_shader(GLenum type, char *shader_source);
void create_shader(shader_t *shader, char *vertex_shader, char *fragment_shader);

#endif