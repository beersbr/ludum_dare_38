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


    // NOTE(Brett):prepare the level
    level_t level;
    level.create_level(level_size.x, level_size.y);
    level.create_wall( 4, 3, 'w' );
    level.create_wall( 1, 2, 'a' );
    level.create_wall( 6, 7, 's' );
    level.create_wall( 5, 1, 'd' );
    // level.print_level();

    for( int offset = 0; offset < level.grid.size(); ++offset ) {
        tile_t tile = level.grid[offset];

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

        tile_entity->scale = TILE_SIZE;
    }



}


void draw_scene(scene_t *scene)
{
    assert(scene);

    glm::mat4 view_matrix = glm::lookAt(scene->camera_position,
                                        scene->camera_lookat,
                                        glm::vec3(0.0f, 1.0f, 0.0f));


    for ( auto entity : scene->active_entities ) {

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

