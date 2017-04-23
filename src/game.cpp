#include "game.hpp"

void update_player(scene_t *scene, unsigned int ticks)
{
    entity_t *player = scene->player;

    if ( controller_manager->get_mousedown(SDL_BUTTON_MIDDLE) ) {
        glm::vec2 delta        = controller_manager->cursor - controller_manager->last_cursor;
        glm::vec3 eye          = scene->camera_position - scene->camera_lookat;
        glm::vec3 new_eye      = glm::rotate(eye, (delta.x/-50.f), glm::vec3(0.0f, 1.0f, 0.0f));
        scene->camera_position = scene->camera_lookat + new_eye;
    }
    else { 
        scene->camera_position = scene->camera_lookat + CAMERA_OFFSET;
    }

    if ( controller_manager->get_keydown(SDLK_a) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   'a') == 0 ) {
            player->position += glm::vec3(-1 * TILE_SIZE.x, 0.0f, 0.0f);
            player->level_coordinate.x -= 1;
        }

    }

    if ( controller_manager->get_keydown(SDLK_d) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   'd') == 0 ) {
            player->position += glm::vec3(1.0f * TILE_SIZE.x, 0.0f, 0.0f);
            player->level_coordinate.x += 1;
        }
    }

    if ( controller_manager->get_keydown(SDLK_w) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   'w') == 0 ) {
            player->position += glm::vec3(0.0f, 0.0f, -1 * TILE_SIZE.z);
            player->level_coordinate.y -= 1;
        }
    }

    if ( controller_manager->get_keydown(SDLK_s) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   's') == 0 ) {

            player->position += glm::vec3(0.0f, 0.0f, 1 * TILE_SIZE.z);
            player->level_coordinate.y += 1;
        }
    }


}