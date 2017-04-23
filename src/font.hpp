#ifndef FONT_HPP
#define FONT_HPP

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "graphics.hpp"

#include <map>
// #include <pair>

#define FONT_SHADER_ATTRIBUTE_POSITION 0
#define FONT_SHADER_ATTRIBUTE_UV 1

typedef struct _font_vertex_description_t {
    glm::vec3 position;
    glm::vec2 uv;
} font_vertex_description_t;

#define FONT_GFF_VERTEX_SIZE sizeof(font_vertex_description_t)

typedef struct _glyph_t {
    texture_t texture;
    glm::vec2 size;
    glm::vec2 baseline_offset;
    GLuint glyph_offset;
} glyph_t;


void setup_text_renderer(shader_t *shader, char const * font_path);
void redner_text(std::string text, float x, float y, float scale, glm::vec3 color);


#endif