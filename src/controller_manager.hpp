#ifndef _CONTROLLER_MANAGER
#define _CONTROLLER_MANAGER

#include <sdl2/SDL.h>
#include <map>
#include <iostream>
#include <glm/glm.hpp>

#define controller_manager controller_manager_t::instance()

struct controller_manager_t {
    static controller_manager_t *instance();

    void set_keydown(const SDL_Keycode keycode);
    void set_keyup(const SDL_Keycode keycode);

    bool get_keydown(const SDL_Keycode keycode);

    void set_mousedown(const unsigned int);
    void set_mouseup(const unsigned int);

    bool get_mousedown(const unsigned int);

    static controller_manager_t *singleton_instance;

    controller_manager_t();
    std::map<SDL_Keycode const, bool> keystates;

    std::map<int const, bool> mousestates;

    glm::vec2 last_cursor;
    glm::vec2 cursor;

    void set_cursor_position(const int x, const int y);
};

#endif