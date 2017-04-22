#include "core.hpp"

void create_scene(scene_t *scene, glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
    static unsigned int ids = 0;
    assert(scene);

    scene->id = ++ids;
    scene->projection_matrix = projection_matrix;
    scene->view_matrix = view_matrix;

    scene->entities_pool.resize(SCENE_ENTITY_POOL_MAX);
    scene->open_entities.resize(SCENE_ENTITY_POOL_MAX);

    for( int i = 0; i < SCENE_ENTITY_POOL_MAX; ++i ) { 
        scene->open_entities.push_back(&(scene->entities_pool[i]));
    }
}


void draw_scene(scene_t *scene)
{
    for ( auto entity : scene->active_entities ) {

        glm::mat4 model_matrix = glm::mat4(1.f);

        // TODO(Brett):These should be added to any scaling/rtoation/transaltion that is on the model
        model_matrix = glm::scale(model_matrix, entity->scale);

        model_matrix = glm::rotate(model_matrix, entity->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, entity->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, entity->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        model_matrix = glm::translate(model_matrix, entity->position);

        model_t *model = entity->model;
        use_shader(model->shader);

        GLint projection_location = get_uniform_location(model->shader, "projection");
        GLint view_location       = get_uniform_location(model->shader, "view");
        GLint model_location      = get_uniform_location(model->shader, "model");

        glUniformMatrix4fv(projection_location, 1, false, (GLfloat*)&scene->projection_matrix[0]);
        glUniformMatrix4fv(view_location, 1, false, (GLfloat*)&scene->view_matrix[0]);
        glUniformMatrix4fv(model_location, 1, false, (GLfloat*)&model_matrix[0]);

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
    entity->scale    = glm::vec3(0.0f, 0.0f, 0.0f);
    entity->model    = model;
    
    scene->active_entities.push_back(entity);

    return entity;
}