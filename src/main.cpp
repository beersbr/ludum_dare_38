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

    texture_t default_texture = {};
    create_texture(&default_texture, "images/tile_simple.png");

    texture_t wall_texture = {};
    create_texture(&wall_texture, "images/wall.png");

    texture_t player_texture = {};
    create_texture(&player_texture, "images/player.png");

    texture_t wall2_texture = {};
    create_texture(&wall2_texture, "images/wall2.png");

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
    create_model(&floor_model, cube_mesh, &default_shader, &default_texture);

    model_t player_model = {};
    create_model(&player_model, cube_mesh, &default_shader, &player_texture);

    model_t wall_model = {};
    create_model(&wall_model, cube_mesh, &default_shader, &wall_texture);

    // NOTE(Brett):Create the scene. The scene handles all the objects and stuff that we need for our game at a certain
    // point
    scene_t scene = {};
    create_scene(&scene, projection_matrix,
                 glm::vec3(0.0f, 500.f, 500.f),
                 glm::vec3(0.0f, 0.0f, 0.0f));

    // NOTE(Brett):prepare the level
    level_t level1;
    level1.create_level(8,8);
    level1.create_wall( 4, 3, 'w' );
    level1.create_wall( 1, 2, 'a' );
    level1.create_wall( 6, 7, 's' );
    level1.create_wall( 5, 1, 'd' );
    level1.print_level();

    for( int offset = 0; offset < level1.grid.size(); ++offset ) {
        tile_t tile = level1.grid[offset];

        float x_offset = tile.x * TILE_SIZE.x + TILE_SIZE.x/2.f;
        float z_offset = tile.y * TILE_SIZE.z + TILE_SIZE.z/2.f;
        float y_offset = 0.f;

        std::cout << x_offset << " " << z_offset << std::endl;

        if ( tile.type == level_entrance ) {
            y_offset -= 20.f;
        }
        if ( tile.type == level_exit ) {
            y_offset += 20.f;
        }

        // NOTE(Brett):An entity is where we care abot things. It is the holder for the actual game object.
        // right now an entity is nothing more than a position, id and memory inside the scene.
        entity_t *tile_entity = request_scene_entity(&scene,
                                                     glm::vec3(x_offset, y_offset, z_offset),
                                                     &floor_model);

        if ( tile.wall_s ) { 
            entity_t *tile_wall = request_scene_entity(&scene,
                                                       glm::vec3(x_offset, 55.f/2.f, z_offset+25.f-(5.0/2.f)),
                                                       &wall_model);

            tile_wall->scale = glm::vec3(50.f, 55.f, 5.f);
            tile_wall->is_static = true;

        }

        if ( tile.wall_w ) { 
            entity_t *tile_wall = request_scene_entity(&scene,
                                                       glm::vec3(x_offset, 55.f/2.f, z_offset-25.f+(5.0/2.f)),
                                                       &wall_model);

            tile_wall->scale = glm::vec3(50.f, 55.f, 5.f);
            tile_wall->is_static = true;

        }

        if ( tile.wall_a ) { 
            entity_t *tile_wall = request_scene_entity(&scene,
                                                       glm::vec3(x_offset-25.f+(5.0/2.f), 55.f/2.f, z_offset),
                                                       &wall_model);

            tile_wall->scale = glm::vec3(5.f, 55.f, 50.f);
            tile_wall->is_static = true;

        }

        if ( tile.wall_d ) { 
            entity_t *tile_wall = request_scene_entity(&scene,
                                                       glm::vec3(x_offset+25.f-(5.0/2.f), 55.f/2.f, z_offset),
                                                       &wall_model);

            tile_wall->scale = glm::vec3(5.f, 55.f, 50.f);
            tile_wall->is_static = true;

        }

        std::cout << tile_entity->id << std::endl;
        tile_entity->scale = TILE_SIZE;
    }

    glm::vec3 player_size = glm::vec3(30.0f, 50.0f, 30.f);

    glm::vec3 camera_lookat = glm::vec3((8.0*TILE_SIZE.x)/2.f,
                                        0.0f,
                                        (8.0*TILE_SIZE.z)/2.f);


    glm::vec3 camera_position = camera_lookat + CAMERA_OFFSET;

    entity_t *player = request_scene_entity(&scene,
                                            glm::vec3(TILE_SIZE.x/2.f,
                                                      TILE_SIZE.y+player_size.y/2.f,
                                                      TILE_SIZE.z/2.f),
                                            &player_model);

   	entity_t *enemy = request_scene_entity(&scene,
                                            glm::vec3(TILE_SIZE.x/2.f,
                                                      TILE_SIZE.y+player_size.y/2.f,
                                                      TILE_SIZE.z/2.f),
                                            &player_model);

    player->scale = player_size;
    player->level_coordinate = glm::vec2(0.f, 0.f);



    glm::vec2 enemy_coordinates = glm::vec2(4.f, 4.f);
    glm::vec2 enemy_position = enemy_coordinates * glm::vec2(TILE_SIZE.x, TILE_SIZE.z);
    enemy->scale            = player_size;

    enemy->level_coordinate = enemy_coordinates;
    enemy->position += glm::vec3(enemy_position.x, 0.f, enemy_position.y);
    enemy->is_enemy         = true;
    enemy->enemy_can_move   = true;



    scene.camera_lookat   = camera_lookat;
    scene.camera_position = camera_position;
    scene.level           = &level1;
    scene.player          = player;

    game_state_t game_state = {};

    int state_loop = 0;

    game_state.update = player_action;

    SDL_GL_SetSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        
        SDL_GL_SwapWindow(main_window);
    }

    // NOTE(Brett):Do I really need this?
    IMG_Quit();
    SDL_Quit();

    return 0;
}