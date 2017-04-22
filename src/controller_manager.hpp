#ifndef _CONTROLLER_MANAGER
#define _CONTROLLER_MANAGER

#include <sdl2/SDL.h>
#include <map>

#define controller_manager controller_manager_t::instance()

struct controller_manager_t {
    static controller_manager_t *instance();

    void set_keydown(const SDL_Keycode keycode);
    void set_keyup(const SDL_Keycode keycode);

    bool get_keydown(const SDL_Keycode keycode);

    static controller_manager_t *singleton_instance;

    controller_manager_t();
    std::map<SDL_Keycode const, bool> keystates;
};

#endif