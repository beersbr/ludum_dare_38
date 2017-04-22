#include "core.hpp"

void create_entity(entity_t *entity, glm::vec3 position, model_t *model)
{
}


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


entity_t * request_scene_entity(scene_t *scene, glm::vec3 position, model_t *model) {
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
	
	return entity;
}