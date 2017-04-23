#include "game.hpp"
#include <cmath>

static state_update_function STATE_FUNCTIONS[] = {
    player_action,
    player_attack_animation,
    player_attack_complete,
    player_move_animation,
    level_change_check,
    level_transition,
    enemy_action,
    enemy_death_animation,
    enemy_attack_animation,
    enemy_move_animation
};

state_update_function get_state(STATE_FUNCTION_ID id) {
    return STATE_FUNCTIONS[id];
}

STATE_FUNCTION_ID player_action(scene_t *scene, unsigned int ticks)
{
    entity_t *player = scene->player;
    entity_t *enemy;
    std::list<entity_t *>::const_iterator enemy_iterator;

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
                                   'a') != TILE_INVALID ) {

            // NOTE(JP): Check for an enemy in the next tile before moving
            for ( enemy_iterator = scene->active_entities.begin(); enemy_iterator != scene->active_entities.end(); ++enemy_iterator ) {
                enemy = *enemy_iterator;

                if( !(enemy->is_enemy) ) {
                    continue;
                }

                if ( enemy->level_coordinate.x == player->level_coordinate.x - 1 &&
                     enemy->level_coordinate.y == player->level_coordinate.y ) {
                    enemy->enemy_health--;

                    player->animation_start_position = player->position;
                    player->animation_end_position = player->position + glm::vec3(-ATTACK_MOVE_DISTANCE * TILE_SIZE.x, 0.0f, 0.0f);    
                    create_animation(&player->animation, ticks, DEFAULT_ANIMATION_TICKS, ease_out);

                    return PLAYER_ATTACK_ANIMATION;
                }
            }
            player->level_coordinate.x -= 1;

            player->animation_start_position = player->position;
            player->animation_end_position = player->position + glm::vec3(-1.f * TILE_SIZE.x, 0.0f, 0.0f);
            create_animation(&player->animation, ticks, DEFAULT_ANIMATION_TICKS, ease_out_circ);
            return PLAYER_MOVE_ANIMATION;
        }
    }
    else if ( controller_manager->get_keydown(SDLK_d) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   'd') != TILE_INVALID ) {


            // NOTE(JP): Check for an enemy in the next tile before moving
            for ( enemy_iterator = scene->active_entities.begin(); 
                 enemy_iterator != scene->active_entities.end(); 
                 ++enemy_iterator ) 
            {
                enemy = *enemy_iterator;

                if( !(enemy->is_enemy) ) {
                    continue;
                }

                if ( enemy->level_coordinate.x == player->level_coordinate.x + 1 &&
                     enemy->level_coordinate.y == player->level_coordinate.y ) {
                    enemy->enemy_health--;

                    player->animation_start_position = player->position;
                    player->animation_end_position = player->position + glm::vec3(ATTACK_MOVE_DISTANCE * TILE_SIZE.x, 0.0f, 0.0f);    
                    create_animation(&player->animation, ticks, DEFAULT_ANIMATION_TICKS, ease_out);

                    return PLAYER_ATTACK_ANIMATION;
                }
            }
            player->level_coordinate.x += 1;
            
            player->animation_start_position = player->position;
            player->animation_end_position   = player->position + glm::vec3(1.f * TILE_SIZE.x, 0.0f, 0.0f);
            create_animation(&player->animation, ticks, DEFAULT_ANIMATION_TICKS, ease_out_circ);
            return PLAYER_MOVE_ANIMATION;
        }
    }
    else if ( controller_manager->get_keydown(SDLK_w) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   'w') != TILE_INVALID ) {

            // NOTE(JP): Check for an enemy in the next tile before moving
            for ( enemy_iterator = scene->active_entities.begin(); 
                 enemy_iterator != scene->active_entities.end(); 
                 ++enemy_iterator ) 
            {
                enemy = *enemy_iterator;

                if( !(enemy->is_enemy) ) {
                    continue;
                }

                if ( enemy->level_coordinate.x == player->level_coordinate.x &&
                     enemy->level_coordinate.y == player->level_coordinate.y - 1 ) {
                    enemy->enemy_health--;

                    player->animation_start_position = player->position;
                    player->animation_end_position = player->position + glm::vec3(0.0f, 0.0f, -ATTACK_MOVE_DISTANCE * TILE_SIZE.z);    
                    create_animation(&player->animation, ticks, DEFAULT_ANIMATION_TICKS, ease_out);

                    return PLAYER_ATTACK_ANIMATION;
                }
            }
            player->level_coordinate.y -= 1;

            player->animation_start_position = player->position;
            player->animation_end_position = player->position + glm::vec3(0.0f, 0.0f, -1.f * TILE_SIZE.z);
            create_animation(&player->animation, ticks, DEFAULT_ANIMATION_TICKS, ease_out_circ);
            return PLAYER_MOVE_ANIMATION;
        }
    }
    else if ( controller_manager->get_keydown(SDLK_s) ) {
        if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   's') != TILE_INVALID ) {

            // NOTE(JP): Check for an enemy in the next tile before moving
            for ( enemy_iterator = scene->active_entities.begin(); 
                 enemy_iterator != scene->active_entities.end(); 
                 ++enemy_iterator ) 
            {
                enemy = *enemy_iterator;

                if( !(enemy->is_enemy) ) {
                    continue;
                }

                if ( enemy->level_coordinate.x == player->level_coordinate.x &&
                     enemy->level_coordinate.y == player->level_coordinate.y + 1 ) {
                    enemy->enemy_health--;

                    player->animation_start_position = player->position;
                    player->animation_end_position = player->position + glm::vec3(0.0f, 0.0f, ATTACK_MOVE_DISTANCE * TILE_SIZE.z);    
                    create_animation(&player->animation, ticks, DEFAULT_ANIMATION_TICKS, ease_out);

                    return PLAYER_ATTACK_ANIMATION;
                }
            }
            player->level_coordinate.y += 1;

            player->animation_start_position = player->position;
            player->animation_end_position = player->position + glm::vec3(0.0f, 0.0f, 1.f * TILE_SIZE.z);    
            create_animation(&player->animation, ticks, DEFAULT_ANIMATION_TICKS, ease_out);
            return PLAYER_MOVE_ANIMATION;
        }   
    }

    

    return PLAYER_ACTION;
}

 
STATE_FUNCTION_ID player_attack_animation( scene_t *scene, unsigned int ticks ) {
    entity_t *player = scene->player;
    
    if ( player->animation.is_done || ticks - player->animation.start_tick > player->animation.duration ) {
        player->animation_end_position = player->animation_start_position;
        player->animation_start_position = player->position;
        create_animation(&player->animation, ticks, DEFAULT_ANIMATION_TICKS, ease_out);

        return PLAYER_ATTACK_COMPLETE;
    }
    else {
        float value = eval_animation(&player->animation, ticks);

        glm::vec3 delta = (player->animation_end_position - player->animation_start_position);
        player->position = player->animation_start_position + (delta * value);
    }

    return PLAYER_ATTACK_ANIMATION;
}


STATE_FUNCTION_ID player_attack_complete( scene_t *scene, unsigned int ticks ) {
    entity_t *player = scene->player;

    if ( player->animation.is_done || ticks - player->animation.start_tick > player->animation.duration ) {
        return LEVEL_CHANGE_CHECK;
    }
    else {
        float value = eval_animation(&player->animation, ticks);

        glm::vec3 delta = (player->animation_end_position - player->animation_start_position);
        player->position = player->animation_start_position + (delta * value);
    }

    return PLAYER_ATTACK_COMPLETE;
}

STATE_FUNCTION_ID player_move_animation( scene_t *scene, unsigned int ticks ) {
    entity_t *player = scene->player;

    if ( player->animation.is_done || ticks - player->animation.start_tick > player->animation.duration ) {
        player->position = player->animation_end_position;
        return LEVEL_CHANGE_CHECK;
    }
    else {
        float value = eval_animation(&player->animation, ticks);

        glm::vec3 delta = (player->animation_end_position - player->animation_start_position);
        player->position = player->animation_start_position + (delta * value);
    }

    return PLAYER_MOVE_ANIMATION;
}


STATE_FUNCTION_ID level_change_check( scene_t *scene, unsigned int ticks ) {
    entity_t *player = scene->player;

    if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   '\0') == TILE_EXIT ) {

        std::cout << "Found exit" << std::endl;

        return LEVEL_TRANSITION;
    }
    else if ( scene->level->query_location(player->level_coordinate.x,
                                   player->level_coordinate.y,
                                   '\0') == TILE_ENTRANCE ) {
        std::cout << "Found entrance" << std::endl;

        return LEVEL_TRANSITION;
    }

    return ENEMY_ACTION;
}

STATE_FUNCTION_ID level_transition( scene_t *scene, unsigned int ticks ) {
    std::cout << "Changing level..." << std::endl;

    // TODO(JP): change the level

    return PLAYER_ACTION;
}

STATE_FUNCTION_ID enemy_action( scene_t *scene, unsigned int ticks ) {
    entity_t *enemy;
    entity_t *player = scene->player;
    std::list<entity_t *>::const_iterator enemy_iterator;
    int delta_x;
    int delta_y;
    int a_delta_x;
    int a_delta_y;

    std::cout << "Enemy action" << std::endl;

    for ( enemy_iterator = scene->active_entities.begin(); 
         enemy_iterator != scene->active_entities.end(); 
         ++enemy_iterator ) 
    {
        enemy = *enemy_iterator;

        if( !(enemy->is_enemy) ) {
            continue;
        }

        if ( 0 >= enemy->enemy_health ) {
            return ENEMY_DEATH_ANIMATION;
        }

        delta_x = player->level_coordinate.x - enemy->level_coordinate.x;
        delta_y = player->level_coordinate.y - enemy->level_coordinate.y;
        a_delta_x = std::abs(delta_x);
        a_delta_y = std::abs(delta_y);

        if ( a_delta_x <=1 && a_delta_y <= 1 && a_delta_x != a_delta_y ) {
            // Attack, don't bother moving
            return ENEMY_ATTACK_ANIMATION;
        }

        if ( enemy->enemy_can_move ) {
            std::cout << "Delta x: " << delta_x << " Delta y: " << delta_y << std::endl;

            if( a_delta_x > a_delta_y ) {

                if( 0 < delta_x ) {
                    if ( scene->level->query_location(enemy->level_coordinate.x,
                                   enemy->level_coordinate.y,
                                   'd') == 0 ) {
                        enemy->level_coordinate.x += 1;

                        enemy->animation_start_position = enemy->position;
                        enemy->animation_end_position   = enemy->position + glm::vec3(+1.f * TILE_SIZE.x, 0.0f, 0.0f);
                        create_animation(&enemy->animation, ticks, 300, ease_out_circ);
                        return ENEMY_MOVE_ANIMATION;
                    }
                }
                else {
                    if ( scene->level->query_location(enemy->level_coordinate.x,
                                   enemy->level_coordinate.y,
                                   'a') == 0 ) {
                        enemy->level_coordinate.x -= 1;

                        enemy->animation_start_position = enemy->position;
                        enemy->animation_end_position   = enemy->position + glm::vec3(-1.f * TILE_SIZE.x, 0.0f, 0.0f);
                        create_animation(&enemy->animation, ticks, 300, ease_out_circ);
                        return ENEMY_MOVE_ANIMATION;
                    }
                }
            }
            else {
                if( 0 > delta_y ) {
                    if ( scene->level->query_location(enemy->level_coordinate.x,
                                   enemy->level_coordinate.y,
                                   'w') == 0 ) {
                        enemy->level_coordinate.y -= 1;

                        enemy->animation_start_position = enemy->position;
                        enemy->animation_end_position = enemy->position + glm::vec3(0.0f, 0.0f, -1.f * TILE_SIZE.z);
                        create_animation(&enemy->animation, ticks, 300, ease_out_circ);
                        return ENEMY_MOVE_ANIMATION;
                    }
                }
                else {
                    if ( scene->level->query_location(enemy->level_coordinate.x,
                                   enemy->level_coordinate.y,
                                   's') == 0 ) {
                        enemy->level_coordinate.y += 1;

                        enemy->animation_start_position = enemy->position;
                        enemy->animation_end_position = enemy->position + glm::vec3(0.0f, 0.0f, 1.f * TILE_SIZE.z);
                        create_animation(&enemy->animation, ticks, 300, ease_out_circ);
                        return ENEMY_MOVE_ANIMATION;
                    }
                }
            }
        }
        // TODO(JP): attack if player in neighboring tile
    }

    return PLAYER_ACTION;
}

STATE_FUNCTION_ID enemy_death_animation( scene_t *scene, unsigned int ticks ) { 
    entity_t *enemy;

    std::cout << "Enemy action" << std::endl;

    std::list<entity_t *>::const_iterator enemy_iterator = scene->active_entities.begin();
    while ( enemy_iterator != scene->active_entities.end() ) {

        enemy = *enemy_iterator;
        if ( !(enemy->is_enemy) ) {
            ++enemy_iterator;
        }
        else {
            if( 0 >= enemy->enemy_health ) {
                scene->active_entities.erase(enemy_iterator++);
                scene->dead_entities.push_back(enemy);
            }
            else {
                ++enemy_iterator;
            }
        }
    }

    return PLAYER_ACTION;
}

STATE_FUNCTION_ID enemy_attack_animation( scene_t *scene, unsigned int ticks ) {
    entity_t *enemy;
    std::list<entity_t *>::const_iterator enemy_iterator;

    for ( enemy_iterator = scene->active_entities.begin(); enemy_iterator != scene->active_entities.end(); ++enemy_iterator ) {
        enemy = *enemy_iterator;

        if( !(enemy->is_enemy) ) {
            continue;
        }

        std::cout << "Enemy attack" << std::endl;
        return PLAYER_ACTION;
    }

    return ENEMY_ATTACK_ANIMATION;
}

STATE_FUNCTION_ID enemy_move_animation( scene_t *scene, unsigned int ticks ) {
    entity_t *enemy;
    std::list<entity_t *>::const_iterator enemy_iterator;

    for ( enemy_iterator = scene->active_entities.begin(); enemy_iterator != scene->active_entities.end(); ++enemy_iterator ) {
        enemy = *enemy_iterator;

        if( !(enemy->is_enemy) ) {
            continue;
        }

        if ( enemy->animation.is_done || ticks - enemy->animation.start_tick > enemy->animation.duration ) {
            enemy->position = enemy->animation_end_position;
            return PLAYER_ACTION;
        }
        else {
            float value = eval_animation(&enemy->animation, ticks);

            glm::vec3 delta = (enemy->animation_end_position - enemy->animation_start_position);
            enemy->position = enemy->animation_start_position + (delta * value);
        }
    }

    return ENEMY_MOVE_ANIMATION;
}