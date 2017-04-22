
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

static int WIDTH  = 800;
static int HEIGHT = 600;

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) { 
        #ifdef DEBUG
        std::cout << "cout not initialize sdl2" << std::endl;
        #endif
    }

    IMG_Init(IMG_INIT_PNG);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window *main_window = SDL_CreateWindow("LUDUM DARE 38 :: A SMALL WORLD",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   WIDTH, HEIGHT, 
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    SDL_GLContext main_context = SDL_GL_CreateContext(main_window);


    
    #ifdef _WIN32
    // glew things for windows
    #endif
    
    // NOTE(Brett):This is the default shader, it doesnt do anything super interesting right now... and its on the stack
    // TODO(Brett):We may need a shader maanger
    shader_t default_shader = {};
    create_shader_program(&default_shader,
                          "shaders/simple.vertex.glsl",
                          "shaders/simple.fragment.glsl");

    // NOTE(Brett):Create the scene. The scene handles all the objects and stuff that we need for our game at a certain
    // point
    scene_t scene = {};

    glm::mat4 projection_matrix = glm::ortho((float)WIDTH/2.f*-1.f,
                                             (float)WIDTH/2.f,
                                             (float)HEIGHT/2.f*-1.f,
                                             (float)HEIGHT/2.f,
                                             0.01f,
                                             10000.f);

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

        // back
        { glm::vec3( 0.5f,  0.5f, -0.5f), POS_Z, BLACK, glm::vec2(0.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f), POS_Z, BLACK, glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f), POS_Z, BLACK, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f), POS_Z, BLACK, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f), POS_Z, BLACK, glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), POS_Z, BLACK, glm::vec2(1.0f, 0.0f) },

        // top
        { glm::vec3(-0.5f, 0.5f, -0.5f), POS_Y, GREEN, glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, 0.5f,  0.5f), POS_Y, GREEN, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, 0.5f, -0.5f), POS_Y, GREEN, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f, 0.5f, -0.5f), POS_Y, GREEN, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f, 0.5f,  0.5f), POS_Y, GREEN, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, 0.5f,  0.5f), POS_Y, GREEN, glm::vec2(1.0f, 0.0f) },

        // bottom
        { glm::vec3(-0.5f, -0.5f,  0.5f), NEG_Y, RED, glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), NEG_Y, RED, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), NEG_Y, RED, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), NEG_Y, RED, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), NEG_Y, RED, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f), NEG_Y, RED, glm::vec2(1.0f, 0.0f) },

        // left
        { glm::vec3(-0.5f,  0.5f, -0.5f), NEG_X, BLUE, glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), NEG_X, BLUE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), NEG_X, BLUE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), NEG_X, BLUE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), NEG_X, BLUE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), NEG_X, BLUE, glm::vec2(1.0f, 0.0f) },

        // right
        { glm::vec3( 0.5f,  0.5f,  0.5f), NEG_X, BLUE, glm::vec2(0.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), NEG_X, BLUE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f), NEG_X, BLUE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f), NEG_X, BLUE, glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), NEG_X, BLUE, glm::vec2(0.0f, 0.0f) },
        { glm::vec3( 0.5f, -0.5f, -0.5f), NEG_X, BLUE, glm::vec2(1.0f, 0.0f) },
    };

    mesh_t cube_mesh = {};

    cube_mesh.vertices.resize(ARRAY_SIZE(mesh_data));
    cube_mesh.vertices.assign(mesh_data, mesh_data+ARRAY_SIZE(mesh_data));


    create_scene(&scene, projection_matrix,
                 glm::vec3(0.0f, 500.f, 500.f),
                 glm::vec3(0.0f, 0.0f, 0.0f));

    // NOTE(Brett): a model is the opengl manifsstation of a mesh. (it could have more than one mesh) and handles
    // all the information needed to draw the mesh onto the screen (like location etc)
    model_t model = {};
    create_model(&model, cube_mesh, &default_shader);

    // NOTE(Brett):prepare the level
    level_t level1;
    level1.create_level(3,4);

    glm::vec3 tile_size = glm::vec3(50.f, 20.f, 50.f);

    for( int offset = 0; offset < level1.grid.size(); ++offset ) {
        tile_t tile = level1.grid[offset];

        float x_offset = tile.x * tile_size.x + tile_size.x/2.f;
        float z_offset = tile.y * tile_size.z + tile_size.z/2.f;

        std::cout << x_offset << " " << z_offset << std::endl;

        // NOTE(Brett):An entity is where we care abot things. It is the holder for the actual game object.
        // right now an entity is nothing more than a position, id and memory inside the scene.
        entity_t *tile_entity = request_scene_entity(&scene,
                                                     glm::vec3(x_offset, 0.0f, z_offset),
                                                     &model);

        std::cout << tile_entity->id << std::endl;
        tile_entity->scale = tile_size;
    }

    SDL_GL_SetSwapInterval(0);
    glEnable(GL_DEPTH_TEST);

    static bool running = true;
    static SDL_Event event = {};

    while ( running ) { 
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
            }
        }

        float ticks = SDL_GetTicks()/500.f;

        if ( controller_manager->get_keydown(SDLK_a) ) {
            level1.move(move_left);
        }

        if ( controller_manager->get_keydown(SDLK_d) ) {
            level1.move(move_right);
        }

        if ( controller_manager->get_keydown(SDLK_w) ) {
            level1.move(move_up);
        }

        if ( controller_manager->get_keydown(SDLK_s) ) {
            level1.move(move_down);
        }

        // prepare scene()

        float factor = ((float)sin(ticks)+1.f)/4.f + 0.25f;
        glClearColor(factor, 0.0f, factor, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_scene(&scene);
        
        SDL_GL_SwapWindow(main_window);
    }

    // NOTE(Brett):Do I really need this?
    IMG_Quit();
    SDL_Quit();

    return 0;
}