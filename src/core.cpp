#include "core.hpp"

void create_scene(scene_t *scene,
                  glm::mat4 projection_matrix,
                  glm::vec3 camera_location,
                  glm::vec3 camera_lookat)
{
    static unsigned int ids = 0;
    assert(scene);

    scene->id = ++ids;
    scene->projection_matrix = projection_matrix;

    scene->camera_position = camera_location;
    scene->camera_lookat = camera_lookat;

    scene->entities_pool.resize(SCENE_ENTITY_POOL_MAX);
    scene->open_entities.resize(SCENE_ENTITY_POOL_MAX);

    for( int i = 0; i < SCENE_ENTITY_POOL_MAX; ++i ) { 
        scene->open_entities.push_back(&(scene->entities_pool[i]));
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

        glUniformMatrix4fv(projection_location, 1, false, (GLfloat*)&scene->projection_matrix[0]);
        glUniformMatrix4fv(view_location, 1, false, (GLfloat*)&view_matrix[0]);
        glUniformMatrix4fv(model_location, 1, false, (GLfloat*)&model_matrix[0]);

        glUniform3fv(light_position, 1, &scene->light1_position[0]);

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