#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <OpenGL/gl3.h>

#include <cstddef>

#include "util.hpp"

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
} shader_t;


typedef struct _mesh_t {
    std::vector<vertex_definition_t> vertices;
} mesh_t;


typedef struct _model_t {

    unsigned int id;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    GLuint VAO;
    GLuint VBO;

    shader_t *shader;

    mesh_t mesh;

} model_t;


typedef struct _entity_t {
    unsigned int id;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    model_t *model;
} entity_t;


typedef struct _scene_t {
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
} scene_t;


GLuint create_shader(GLenum type, char *shader_source);
void create_shader(shader_t *shader, char *vertex_shader, char *fragment_shader);

void create_model(model_t *model, mesh_t mesh, shader_t *shader);

#endif