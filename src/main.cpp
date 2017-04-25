// 
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_Image/SDL_image.h>

#ifdef _WIN32
#include <windows.h>
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

#include <map> 

#include "controller_manager.hpp"
#include "level.hpp"
#include "graphics.hpp"
#include "core.hpp"
#include "game.hpp"
#include "font.hpp"

#include "items/item_sword.hpp"

static char TITLE[] = "LUDUM DARE 38 :: A SMALL WORLD";
static int WIDTH  = 800;
static int HEIGHT = 600;

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) { 
        #ifdef DEBUG
        std::cout << "cout not initialize sdl2" << std::endl;
        #endif
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window *main_window = SDL_CreateWindow(TITLE,
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   WIDTH, HEIGHT, 
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
                                   // SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN );


    SDL_GLContext main_context = SDL_GL_CreateContext(main_window);
    
    SDL_GL_SetSwapInterval(0);

    glEnable(GL_DEPTH_TEST);
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        

    #ifdef _WIN32
    // glew things for windows
    #endif

    // NOTE(Brett):This is the default shader, it doesnt do anything super interesting right 
    // now... and its on the stack
    shader_t default_shader = {};

    create_shader_program(&default_shader,
                          "shaders/simple.vertex.glsl",
                          "shaders/simple.fragment.glsl");
    GFX_SHADERS["default"] = default_shader;

    shader_t font_shader = {};
    create_shader_program(&font_shader,
                          "shaders/font.vertex.glsl",
                          "shaders/font.fragment.glsl");
    GFX_SHADERS["font"] = font_shader;

    texture_t default_texture = {};
    create_texture(&default_texture, "images/tile_simple.png");
    GFX_TEXTURES["default"] = default_texture;

    texture_t wall_texture = {};
    create_texture(&wall_texture, "images/wall.png");
    GFX_TEXTURES["wall"] = wall_texture;

    texture_t player_texture = {};
    create_texture(&player_texture, "images/player.png");
    GFX_TEXTURES["player"] = player_texture;

    texture_t wall2_texture = {};
    create_texture(&wall2_texture, "images/wall2.png");
    GFX_TEXTURES["wall2"] = wall2_texture;

    setup_text_renderer(&font_shader, "fonts/Anonymous Pro.ttf");

    // NOTE(Brett):This is a mesh. a mesh, right now, is jst the aggregation of a bunch of vertices. we probably dont want it
    // here but that is wher eit is right now.
    vertex_definition_t mesh_data[] = {
        // front
        { glm::vec3(-0.5f,  0.5f, 0.5f), POS_Z, WHITE, glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.5f), POS_Z, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.5f, 0.5f), POS_Z, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  0.5f, 0.5f), POS_Z, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.5f), POS_Z, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, -0.5f, 0.5f), POS_Z, WHITE, glm::vec2(1.0f, 0.0f) },

        // right
        { glm::vec3( 0.5f,  0.5f,  0.5f), POS_X, WHITE, glm::vec2(0.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), POS_X, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f), POS_X, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f), POS_X, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), POS_X, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f), POS_X, WHITE, glm::vec2(1.0f, 0.0f) },

        // back
        { glm::vec3( 0.5f,  0.5f, -0.5f), NEG_Z, WHITE, glm::vec2(0.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f), NEG_Z, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f), NEG_Z, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f), NEG_Z, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f), NEG_Z, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), NEG_Z, WHITE, glm::vec2(1.0f, 0.0f) },

        // left
        { glm::vec3(-0.5f,  0.5f, -0.5f), NEG_X, WHITE, glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), NEG_X, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), NEG_X, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), NEG_X, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), NEG_X, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), NEG_X, WHITE, glm::vec2(1.0f, 0.0f) },

        // top
        { glm::vec3(-0.5f, 0.5f, -0.5f), POS_Y, WHITE, glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, 0.5f,  0.5f), POS_Y, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, 0.5f, -0.5f), POS_Y, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f, 0.5f, -0.5f), POS_Y, WHITE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f, 0.5f,  0.5f), POS_Y, WHITE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, 0.5f,  0.5f), POS_Y, WHITE, glm::vec2(1.0f, 0.0f) },

        // bottom
        { glm::vec3(-0.5f, -0.5f,  0.5f), NEG_Y, BLACK, glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), NEG_Y, BLACK, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), NEG_Y, BLACK, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), NEG_Y, BLACK, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), NEG_Y, BLACK, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f), NEG_Y, BLACK, glm::vec2(1.0f, 0.0f) },
    };

    mesh_t cube_mesh = {};

    cube_mesh.vertices.resize(ARRAY_SIZE(mesh_data));
    cube_mesh.vertices.assign(mesh_data, mesh_data+ARRAY_SIZE(mesh_data));


    glm::mat4 projection_matrix = glm::ortho((float)WIDTH/2.f*-1.f,
                                             (float)WIDTH/2.f,
                                             (float)HEIGHT/2.f*-1.f,
                                             (float)HEIGHT/2.f,
                                             0.01f,
                                             450.f);


    // NOTE(Brett): a model is the opengl manifsstation of a mesh. (it could have more than one mesh) and handles
    // all the information needed to draw the mesh onto the screen (like location etc)
    model_t floor_model = {};
    create_model(&floor_model, cube_mesh, &default_shader, &GFX_TEXTURES["default"]);
    GFX_MODELS["floor"] = floor_model;

    model_t player_model = {};
    create_model(&player_model, cube_mesh, &default_shader, &GFX_TEXTURES["player"]);
    GFX_MODELS["player"] = player_model;

    model_t wall_model = {};
    create_model(&wall_model, cube_mesh, &default_shader, &GFX_TEXTURES["wall"]);
    GFX_MODELS["wall"] = wall_model;

    // NOTE(Brett):Create the scene. The scene handles all the objects and stuff that we need for our game at a certain
    // point
    scene_t scene = {};
    create_scene(&scene, projection_matrix,
                 glm::vec3(0.0f, 500.f, 500.f),
                 glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec2(8.f, 8.f));

    
    game_state_t game_state = {};
    int state_loop = 0;
    game_state.update = player_action;

    static bool running = true;
    static SDL_Event event = {};

    while ( running ) { 

        controller_manager->last_cursor = controller_manager->cursor;

        while ( SDL_PollEvent(&event) ) {
            switch(event.type) {
                case SDL_QUIT: {
                    running = false;
                    break;
                }
                case SDL_KEYUP: {
                    controller_manager->set_keyup(event.key.keysym.sym);
                    break;
                }
                case SDL_KEYDOWN: {
                    controller_manager->set_keydown(event.key.keysym.sym);
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                	controller_manager->set_mouseup(event.button.button);
                	break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                	controller_manager->set_mousedown(event.button.button);
                	break;
                }
                case SDL_MOUSEMOTION: {
                	controller_manager->set_cursor_position(event.motion.x, event.motion.y);
                	break;
                }
            }
        }

        // NOTE(Brett):Update the entities (the scene)
        STATE_FUNCTION_ID id = game_state.update(&scene, SDL_GetTicks());
        game_state.update = get_state(id);

        scene.light1_position = scene.camera_position;

        glm::vec3 inverse_camera_direction = scene.camera_position - scene.camera_lookat;
        inverse_camera_direction.y = 0.f;
        inverse_camera_direction = glm::normalize(inverse_camera_direction) * 300.f;
        inverse_camera_direction.y = 300.f;

        glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        scene.light1_position = scene.camera_lookat + inverse_camera_direction;


        draw_scene(&scene);

        glm::mat4 view_matrix = glm::lookAt(scene.camera_position,
                                            scene.camera_lookat,
                                            glm::vec3(0.0f, 1.0f, 0.0f));
        
        render_text("A small tower :: floor " + std::to_string(scene.level_counter),
                    glm::vec3(-390.f, -290.0f, -1.0f),
                    WHITE,
                    0.5f,
                    projection_matrix);

        render_text("Health: " + std::to_string(scene.player->player_health),
                    glm::vec3(-390.f, 270.0f, -1.0f),
                    WHITE,
                    0.5f,
                    projection_matrix);

        render_text("Damage: " + std::to_string(scene.player->player_stat_damage),
                    glm::vec3(-390.f, 240.0f, -1.0f),
                    WHITE,
                    0.5f,
                    projection_matrix);
        
        SDL_GL_SwapWindow(main_window);
    }

    // NOTE(Brett):Do I really need this?
    IMG_Quit();
    SDL_Quit();

    return 0;
}