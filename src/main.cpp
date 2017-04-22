
#include <iostream>
#include <sdl2/SDL.h>

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

static int WIDTH  = 800;
static int HEIGHT = 600;

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) { 
        #ifdef DEBUG
        std::cout << "cout not initialize sdl2" << std::endl;
        #endif
    }

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
    
    shader_t default_shader = {};
    create_shader_program(&default_shader,
                          "shaders/simple.vertex.glsl",
                          "shaders/simple.fragment.glsl");

    SDL_GL_SetSwapInterval(0);

    glEnable(GL_DEPTH_TEST);

    static bool running = true;
    static SDL_Event event = {};

    level_t level1;

    level1.create_level(3,4);

    std::cout << "Level event: " << level1.grid[7].type << std::endl;

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

        if ( controller_manager->get_keydown(SDLK_LEFT) ) {
            level1.move(move_left);
        }

        if ( controller_manager->get_keydown(SDLK_RIGHT) ) {
        	level1.move(move_right);
        }

        if ( controller_manager->get_keydown(SDLK_UP) ) {
            level1.move(move_up);
        }

        if ( controller_manager->get_keydown(SDLK_DOWN) ) {
            level1.move(move_down);
        }

        glClearColor(((float)sin(ticks)+1.f)/4.f + 0.25f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(main_window);
    }

    return 0;
}