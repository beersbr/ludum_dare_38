#ifndef CORE_HPP
#define CORE_HPP

#include "graphics.hpp"
#include "tile.hpp"
#include "level.hpp"

#include <vector>
#include <list>
#include <glm/ext.hpp>


#define SCENE_ENTITY_POOL_MAX 512


typedef struct _entity_t {
    unsigned int id;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    bool is_tile;
    tile_t *tile;

    bool is_enemy;
    // TODO: add enemy

    bool is_player;
    // TODO: add player

    bool is_item;

    // std::vector<item_t> item_list;

    model_t *model;
} entity_t;


typedef void (*item_callback)(entity_t *player_entity);


typedef struct _item_t {
    std::string description;
    std::string name;

    item_callback before_turn;
    item_callback after_turn;
    item_callback before_move;
    item_callback after_move;
    item_callback before_attack;
    item_callback after_attack;
    
} item_t;


typedef struct _scene_t {

	unsigned int id;

    glm::mat4 projection_matrix;

    glm::vec3 camera_position;
    glm::vec3 camera_lookat;

    std::vector<entity_t> entities_pool;

    std::list<entity_t*> open_entities;
    std::list<entity_t*> active_entities;
    std::list<entity_t*> dead_entities;

    level_t level;

} scene_t;


void create_scene(scene_t *scene,
                  glm::mat4 projection_matrix,
                  glm::vec3 camera_location,
                  glm::vec3 camera_lookat);

void draw_scene(scene_t *scene);

entity_t * request_scene_entity(scene_t *scene, glm::vec3 position, model_t *model);

void draw_entity(entity_t *entity);

#endif