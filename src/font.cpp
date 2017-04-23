#include "font.hpp"

static GLuint FONT_VAO = 0;
static GLuint FONT_VBO = 0;

static shader_t *FONT_SHADER;

static std::map<GLchar, glyph_t> glyphs;

void setup_text_renderer(shader_t *shader, char const * font_path )
{
    assert(shader);
    FONT_SHADER = shader;

    FT_Library ft;
    if ( FT_Init_FreeType(&ft) ) {
        std::cout << "[ERROR] Could not initialize freetype :(" << std::endl;
        exit(1);
    }

    FT_Face face;
    if ( FT_New_Face(ft, font_path, 0, &face) ) {
        std::cout << "[ERROR] Coudl not initialize font face" << font_path << std::endl;
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, 24);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for ( GLubyte character = 0; character < 128; character++ ) {

        if ( FT_Load_Char(face, character, FT_LOAD_RENDER) ) {
            std::cout << "[ERROR] coudl not load freetype glyph '" << character << "'" << std::endl;
            exit(1);
        }

        glyph_t glyph = {};
        create_texture_raw(&glyph.texture,
                           face->glyph->bitmap.width,
                           face->glyph->bitmap.rows,
                           face->glyph->bitmap.buffer,
                           GL_RED,
                           GL_RED,
                           GL_UNSIGNED_BYTE,
                           GL_CLAMP_TO_EDGE);

        glyph.size            = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        glyph.baseline_offset = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        glyph.glyph_offset    = face->glyph->advance.x;


        glyphs.insert(std::pair<GLchar, glyph_t>(character, glyph));
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);


    glGenVertexArrays(1, &FONT_VAO);
    assert(FONT_VAO);
    glBindVertexArray(FONT_VAO);

    glGenBuffers(1, &FONT_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, FONT_VBO);

    glBufferData(GL_ARRAY_BUFFER, FONT_GFF_VERTEX_SIZE * 6, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(FONT_SHADER_ATTRIBUTE_POSITION);
    glEnableVertexAttribArray(FONT_SHADER_ATTRIBUTE_UV);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Winvalid-offsetof"

    glVertexAttribPointer(FONT_SHADER_ATTRIBUTE_POSITION,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          FONT_GFF_VERTEX_SIZE,
                          (GLvoid*)offsetof(font_vertex_description_t, position));

    glVertexAttribPointer(FONT_SHADER_ATTRIBUTE_UV,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          FONT_GFF_VERTEX_SIZE,
                          (GLvoid*)offsetof(font_vertex_description_t, uv));

    #pragma GCC diagnostic pop

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void render_text(std::string text,
                 glm::vec3 position,
                 glm::vec3 color,
                 float scale,
                 glm::mat4 projection,
                 glm::mat4 view)
{
    use_shader(FONT_SHADER);

    glUniformMatrix4fv(get_uniform_location(FONT_SHADER, "projection"),
                       1, GL_FALSE, (GLfloat*)&projection[0]);

    glUniformMatrix4fv(get_uniform_location(FONT_SHADER, "view"),
                       1, GL_FALSE, (GLfloat*)&view[0]);

    glUniform1i(get_uniform_location(FONT_SHADER, "u_image"), 0);
    glUniform3fv(get_uniform_location(FONT_SHADER, "u_color"), 1, &color[0]);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(FONT_VAO);

    float x = 0;
    float y = 0;

    for (std::string::const_iterator c = text.begin(); c != text.end(); c++ ) {

        glyph_t g = glyphs[*c];

        float xpos = x + g.baseline_offset.x * scale;
        float ypos = y + (g.size.y - g.baseline_offset.y) * scale;

        float w = g.size.x * scale;
        float h = g.size.y * scale;

        // std::cout << "Working on char: " << *c << " " << xpos << ", " << ypos << ", " << w << ", " << h << std::endl;

        font_vertex_description_t vertices[] = {
            { glm::vec3( xpos,     ypos + h, 0.0f ), glm::vec2( 0.0f, 1.0f ) },
            { glm::vec3( xpos,     ypos,     0.0f ), glm::vec2( 0.0f, 0.0f ) },
            { glm::vec3( xpos + w, ypos + h, 0.0f ), glm::vec2( 1.0f, 1.0f ) },
            { glm::vec3( xpos + w, ypos + h, 0.0f ), glm::vec2( 1.0f, 1.0f ) },
            { glm::vec3( xpos,     ypos,     0.0f ), glm::vec2( 0.0f, 0.0f ) },
            { glm::vec3( xpos + w, ypos,     0.0f ), glm::vec2( 1.0f, 0.0f ) },
        };

        use_texture(&g.texture, 0);

        glBindBuffer(GL_ARRAY_BUFFER, FONT_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, ARRAY_SIZE(vertices) * FONT_GFF_VERTEX_SIZE, &vertices[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, position);

        glUniformMatrix4fv(get_uniform_location(FONT_SHADER, "model"),
                           1, GL_FALSE, (GLfloat*)&model[0]);


        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (g.glyph_offset >> 6) * scale;
    }

}

