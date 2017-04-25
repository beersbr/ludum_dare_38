#include "core.hpp"

void create_scene(scene_t *scene,
                  glm::mat4 projection_matrix,
                  glm::vec3 camera_location,
                  glm::vec3 camera_lookat,
                  glm::vec2 level_size)
{
    static unsigned int ids = 0;
    assert(scene);

    scene->id = ++ids;
    scene->projection_matrix = projection_matrix;

    scene->camera_position = camera_location;
    scene->camera_lookat = camera_lookat;

    scene->entities_pool.resize(SCENE_ENTITY_POOL_MAX);
    // scene->open_entities.resize(SCENE_ENTITY_POOL_MAX);

    for( int i = 0; i < SCENE_ENTITY_POOL_MAX; ++i ) { 
        scene->open_entities.push_back(&(scene->entities_pool[i]));
    }

    scene->level_counter = 0;

    glm::vec3 player_size = glm::vec3(30.0f, 50.0f, 30.f);
    entity_t *player = request_scene_entity(scene,
                                            glm::vec3(TILE_SIZE.x/2.f,
                                                  TILE_SIZE.y+player_size.y/2.f,
                                                  TILE_SIZE.z/2.f),
                                            &GFX_MODELS["player"]);
    player->player_health = 3;
    player->player_stat_damage = 1;
    scene->player = player;

    update_scene_create_new_level(scene, level_size);

}


void update_scene_create_new_level(scene_t *scene, glm::vec2 level_size)
{
    assert(scene);

    scene->level_counter += 1;

    for ( entity_t* entity : scene->active_entities ) {
        if ( entity == scene->player )
            continue;

        scene->open_entities.push_front(entity);
    }

    scene->active_entities.clear();

    for ( entity_t* entity : scene->dead_entities ) {
        scene->open_entities.push_front(entity);
    }

    scene->dead_entities.clear();

    // scene->level_t level;
    scene->level.create_level(level_size.x, level_size.y);
    scene->level.create_wall( 4, 3, 'w' );
    scene->level.create_wall( 1, 2, 'a' );
    scene->level.create_wall( 6, 7, 's' );
    scene->level.create_wall( 5, 1, 'd' );
    // level.print_level();

    for( int offset = 0; offset < scene->level.grid.size(); ++offset ) {
        tile_t tile = scene->level.grid[offset];

        float x_offset = tile.x * TILE_SIZE.x + TILE_SIZE.x/2.f;
        float z_offset = tile.y * TILE_SIZE.z + TILE_SIZE.z/2.f;
        float y_offset = 0.f;

        if ( tile.type == TILE_ENTRANCE ) {
            y_offset -= 20.f;
        }
        if ( tile.type == TILE_EXIT ) {
            y_offset += 20.f;
        }

        // NOTE(Brett):An entity is where we care abot things. It is the holder for the actual game object.
        // right now an entity is nothing more than a position, id and memory inside the scene.
        entity_t *tile_entity = request_scene_entity(scene,
                                                     glm::vec3(x_offset, y_offset, z_offset),
                                                     &GFX_MODELS["floor"]);

        if ( tile.wall_s ) { 
            entity_t *tile_wall = request_scene_entity(scene,
                                                       glm::vec3(x_offset, 55.f/2.f, z_offset+25.f-(5.0/2.f)),
                                                       &GFX_MODELS["wall"]);

            tile_wall->scale = glm::vec3(50.f, 55.f, 5.f);
            tile_wall->is_static = true;

        }

        if ( tile.wall_w ) { 
            entity_t *tile_wall = request_scene_entity(scene,
                                                       glm::vec3(x_offset, 55.f/2.f, z_offset-25.f+(5.0/2.f)),
                                                       &GFX_MODELS["wall"]);

            tile_wall->scale = glm::vec3(50.f, 55.f, 5.f);
            tile_wall->is_static = true;

        }

        if ( tile.wall_a ) { 
            entity_t *tile_wall = request_scene_entity(scene,
                                                       glm::vec3(x_offset-25.f+(5.0/2.f), 55.f/2.f, z_offset),
                                                       &GFX_MODELS["wall"]);

            tile_wall->scale = glm::vec3(5.f, 55.f, 50.f);
            tile_wall->is_static = true;

        }

        if ( tile.wall_d ) { 
            entity_t *tile_wall = request_scene_entity(scene,
                                                       glm::vec3(x_offset+25.f-(5.0/2.f), 55.f/2.f, z_offset),
                                                       &GFX_MODELS["wall"]);

            tile_wall->scale = glm::vec3(5.f, 55.f, 50.f);
            tile_wall->is_static = true;

        }

        tile_entity->scale = TILE_SIZE;
    }

    // scene->level = level;    

    
    glm::vec3 enemy_size = glm::vec3(30.0f, 50.0f, 30.f);
    glm::vec3 player_size = glm::vec3(30.0f, 50.0f, 30.f);

    glm::vec3 camera_lookat = glm::vec3((level_size.x*TILE_SIZE.x)/2.f,
                                        0.0f,
                                        (level_size.y*TILE_SIZE.z)/2.f);


    glm::vec3 camera_position = camera_lookat + CAMERA_OFFSET;
    scene->camera_lookat      = camera_lookat;
    scene->camera_position    = camera_position;
    
    entity_t *enemy = request_scene_entity(scene,
                                           glm::vec3(TILE_SIZE.x/2.f,
                                                     TILE_SIZE.y+enemy_size.y/2.f,
                                                     TILE_SIZE.z/2.f),
                                           &GFX_MODELS["player"]);

    scene->player->scale = player_size;
    scene->player->level_coordinate = scene->level.start_tile;
    scene->player->position = glm::vec3(TILE_SIZE.x*scene->level.start_tile.x + TILE_SIZE.x/2.f,
                                        scene->player->position.y,
                                        TILE_SIZE.y*scene->level.start_tile.y + TILE_SIZE.z/2.f);

    scene->player->player_health += 1;

    glm::vec2 enemy_coordinates = glm::vec2(5.f, 4.f);
    glm::vec2 enemy_position = enemy_coordinates * glm::vec2(TILE_SIZE.x, TILE_SIZE.z);
    enemy->scale             = enemy_size;

    enemy->level_coordinate = enemy_coordinates;
    enemy->position += glm::vec3(enemy_position.x, 0.f, enemy_position.y);
    enemy->is_enemy         = true;
    enemy->enemy_can_move   = true;
    enemy->enemy_health     = 2;

    scene->active_entities.push_back(scene->player);
}


void draw_scene(scene_t *scene)
{
    assert(scene);

    glm::mat4 view_matrix = glm::lookAt(scene->camera_position,
                                        scene->camera_lookat,
                                        glm::vec3(0.0f, 1.0f, 0.0f));


    for ( entity_t* entity : scene->active_entities ) {

        model_t *model = entity->model;
        glBindVertexArray(model->VAO);
        use_shader(model->shader);

        glm::mat4 translate = glm::translate(glm::mat4(), entity->position);

        // TODO(Brett):These should be added to any scaling/rtoation/transaltion that is on the model
        glm::mat4 scale = glm::scale(glm::mat4(), entity->scale);

        glm::mat4 rotate = glm::rotate(glm::mat4(), entity->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        rotate           = glm::rotate(rotate, entity->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rotate           = glm::rotate(rotate, entity->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));


        glm::mat4 model_matrix = translate * scale;

        GLint projection_location = get_uniform_location(model->shader, "projection");
        GLint view_location       = get_uniform_location(model->shader, "view");
        GLint model_location      = get_uniform_location(model->shader, "model");
        GLint light_position      = get_uniform_location(model->shader, "light1_position");
        GLint tex_position        = get_uniform_location(model->shader, "tex");

        glUniformMatrix4fv(projection_location, 1, false, (GLfloat*)&scene->projection_matrix[0]);
        glUniformMatrix4fv(view_location, 1, false, (GLfloat*)&view_matrix[0]);
        glUniformMatrix4fv(model_location, 1, false, (GLfloat*)&model_matrix[0]);

        glUniform3fv(light_position, 1, &scene->light1_position[0]);

        use_texture(model->texture, 0);
        glUniform1i(tex_position, 0);

        draw_model(model);
    }
}


entity_t * request_scene_entity(scene_t *scene, glm::vec3 position, model_t *model)
{
    static unsigned int ids = 0;

    assert(scene);

    entity_t *entity = scene->open_entities.back();

    assert(entity);

    scene->open_entities.pop_back();

    memset((void*)entity, 0, sizeof(entity_t));

    entity->id       = ++ids;
    entity->position = position;
    entity->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    entity->scale    = glm::vec3(1.0f, 1.0f, 1.0f);
    entity->model    = model;
    
    scene->active_entities.push_back(entity);

    return entity;
}


void return_scene_entity(scene_t *scene, entity_t *entity ) {

}


void create_animation(animation_t *animation,
                      unsigned long start,
                      unsigned long duration,
                      animation_function fn)
{
    assert(animation);

    animation->start_tick = start;
    animation->duration = duration;
    animation->is_done = false;
    animation->function = fn;
}


float eval_animation(animation_t *animation, unsigned long current_tick)
{
    float result = animation->function(current_tick - animation->start_tick,
                                       animation->duration);

    if ( result >= 1.0f ) {
        result = 1.0f;
        animation->is_done = true;
    }

    return result;
}


float linear(float t, float d)
{

    return t / (float)d;
}


float ease_out(float t, float d)
{
    t /= d;
    return -1.f * t * (t - 2);
}


float ease_out_circ(float t, float d)
{
    t /= d;
    t--;
    return sqrtf(1 - t*t);
}

