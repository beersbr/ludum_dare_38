#include "font.hpp"

static GLuint FONT_VAO = 0;
static GLuint FONT_VBO = 0;

static texture_t *FONT_TEXTURE;
static shader_t *FONT_SHADER;

static std::map<GLchar, glyph_t> glyphs;

void setup_text_renderer(texture_t *texture, shader_t *shader, char * font_path )
{
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

    FT_Set_Pixel_Sizes(face, 0, 48);

    for ( GLubyte character = 0; character < 128; character++ ) {

        if ( FT_Load_Char(face, character, FT_LOAD_RENDER) ) {
            std::cout << "[ERROR] coudl not load freetype glyph '" << character << "'" << std::endl;
            exit(1);
        }

        
    }


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


void redner_text(std::string text, float x, float y, float scale, glm::vec3 color )
{
    glUseProgram(FONT_SHADER->id);

}