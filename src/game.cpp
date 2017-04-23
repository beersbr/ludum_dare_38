#include "game.hpp"

static state_update_function STATE_FUNCTIONS[] = {
    player_action,
    player_attack_animation,
    player_move_animation,
    enemy_action,
    enemy_attack_animation,
    enemy_move_animation
};

state_update_function get_state(STATE_FUNCTION_ID id) {
    return STATE_FUNCTIONS[id];
}

STATE_FUNCTION_ID player_action(scene_t *scene, unsigned int ticks)
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
 
            player->level_coordinate.x -= 1;

            player->animation_start_position = player->position;
            player->animation_end_position = player->position + glm::vec3(-1.f * TILE_SIZE.x, 0.0f, 0.0f);
            create_animation(&player->animation, ticks, 300, ease_out_circ);
            return PLAYER_MOVE_ANIMATION;
        }

    }

    if ( controller_manager->get_keydown(SDLK_d) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   'd') == 0 ) {

            player->level_coordinate.x += 1;
            
            player->animation_start_position = player->position;
            player->animation_end_position   = player->position + glm::vec3(1.f * TILE_SIZE.x, 0.0f, 0.0f);
            create_animation(&player->animation, ticks, 300, ease_out_circ);
            return PLAYER_MOVE_ANIMATION;
        }

    }

    if ( controller_manager->get_keydown(SDLK_w) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   'w') == 0 ) {

            player->level_coordinate.y -= 1;

            player->animation_start_position = player->position;
            player->animation_end_position = player->position + glm::vec3(0.0f, 0.0f, -1.f * TILE_SIZE.z);
            create_animation(&player->animation, ticks, 300, ease_out_circ);
            return PLAYER_MOVE_ANIMATION;
        }

    }

    if ( controller_manager->get_keydown(SDLK_s) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   's') == 0 ) {

            player->level_coordinate.y += 1;

            player->animation_start_position = player->position;
            player->animation_end_position = player->position + glm::vec3(0.0f, 0.0f, 1.f * TILE_SIZE.z);    
            create_animation(&player->animation, ticks, 300, ease_out);
            return PLAYER_MOVE_ANIMATION;
        }   

    }

    return PLAYER_ACTION;
}


STATE_FUNCTION_ID player_attack_animation(scene_t *scene, unsigned int ticks) {
    return PLAYER_ATTACK_ANIMATION;
}


STATE_FUNCTION_ID player_move_animation(scene_t *scene, unsigned int ticks) {
    entity_t *player = scene->player;

    if ( player->animation.is_done || ticks - player->animation.start_tick > player->animation.duration ) {
        player->position = player->animation_end_position;
        return PLAYER_ACTION;
    }
    else {
        float value = eval_animation(&player->animation, ticks);

        glm::vec3 delta = (player->animation_end_position - player->animation_start_position);
        player->position = player->animation_start_position + (delta * value);
    }

    return PLAYER_MOVE_ANIMATION;
}

STATE_FUNCTION_ID enemy_action( scene_t *scene, unsigned int tick ) {
    entity_t *enemy;

    for(int i=0; i<scene->entities_pool.size(); i++) {
        if(!(scene->entities_pool[i].is_enemy)) {
            continue;
        }

        enemy = &scene->entities_pool[i];
    }

    // TODO(JP): move towards player
    // TODO(JP): attack if player in neighboring tile

    return ENEMY_ACTION;
}

STATE_FUNCTION_ID enemy_attack_animation( scene_t *scene, unsigned int tick ) {
    entity_t *enemy;

    for(int i=0; i<scene->entities_pool.size(); i++) {
        if(!(scene->entities_pool[i].is_enemy)) {
            continue;
        }

        enemy = &scene->entities_pool[i];
    }

    return ENEMY_ATTACK_ANIMATION;
}

STATE_FUNCTION_ID enemy_move_animation( scene_t *scene, unsigned int tick ) {
    entity_t *enemy;

    for(int i=0; i<scene->entities_pool.size(); i++) {
        if(!(scene->entities_pool[i].is_enemy)) {
            continue;
        }

        enemy = &scene->entities_pool[i];
    }

    return ENEMY_MOVE_ANIMATION;
}