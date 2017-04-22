#include "controller_manager.hpp"

controller_manager_t *controller_manager_t::singleton_instance = nullptr;

controller_manager_t *controller_manager_t::instance() {
    if ( ! controller_manager_t::singleton_instance ) {
        controller_manager_t::singleton_instance = new controller_manager_t();
    }
    return controller_manager_t::singleton_instance;
}


controller_manager_t::controller_manager_t() {

}

void controller_manager_t::set_keydown(const SDL_Keycode keycode) {
    keystates[keycode] = true;
}

void controller_manager_t::set_keyup(const SDL_Keycode keycode) {
    keystates[keycode] = false;
}

bool controller_manager_t::get_keydown(const SDL_Keycode keycode) {
    return keystates[keycode];
}