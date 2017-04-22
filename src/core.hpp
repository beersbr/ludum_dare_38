#ifndef CORE_HPP
#define CORE_HPP

#include "graphics.hpp"
#include <vector>
#include <list>

#define SCENE_ENTITY_POOL_MAX 512

typedef struct _entity_t {
    unsigned int id;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    model_t *model;
} entity_t;


typedef struct _scene_t {

	unsigned int id;

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;

    std::vector<entity_t> entities_pool;

    std::list<entity_t*> open_entities;
    std::list<entity_t*> alive_entities;
    std::list<entity_t*> dead_entities;

} scene_t;


void create_scene(scene_t *scene, glm::mat4 projection_matrix, glm::mat4 view_matrix);

entity_t * request_scene_entity(scene_t *scene, glm::vec3 position, model_t *model);

#endif