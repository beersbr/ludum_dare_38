#ifndef FONT_HPP
#define FONT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "util.hpp"
#include "graphics.hpp"

#include <map>
// #include <pair>

// NOTE(Brett):https://learnopengl.com/#!In-Practice/Text-Rendering

#define FONT_SHADER_ATTRIBUTE_POSITION 0
#define FONT_SHADER_ATTRIBUTE_UV 1

typedef struct _font_vertex_description_t {
    glm::vec3 position;
    glm::vec2 uv;
} font_vertex_description_t;

#define FONT_GFF_VERTEX_SIZE sizeof(font_vertex_description_t)

typedef struct _glyph_t {
    texture_t texture;
    glm::ivec2 size;
    glm::ivec2 baseline_offset;
    GLuint glyph_offset;
} glyph_t;


void setup_text_renderer(shader_t *shader, char const * font_path);
void render_text(std::string text,
                 glm::vec3 position,
                 glm::vec3 color,
                 float scale, glm::mat4 projection,
                 glm::mat4 view = glm::mat4(1.0f) );


#endif