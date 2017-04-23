#ifndef CORE_HPP
#define CORE_HPP

#include "util.hpp"
#include "graphics.hpp"
#include "tile.hpp"
#include "level.hpp"

#include <vector>
#include <list>
#include <glm/ext.hpp>


#define SCENE_ENTITY_POOL_MAX 512
#define TILE_SIZE glm::vec3(50.f, 20.f, 50.f)
#define CAMERA_OFFSET glm::vec3(25.0f, 250.f, 125.f)

typedef struct _entity_t {
    unsigned int id;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::vec2 level_coordinate;

    bool is_static;

    bool is_tile;
    tile_t *tile;

    // Enemy stuff
    bool is_enemy;
    int enemy_health;
    int enemy_stat_damage;


    // Player stuff
    bool is_player;
    int player_health;
    int player_stat_damage;

    // Item stuff
    bool is_item;
    std::string item_name;
    std::string item_description;

    typedef void (*item_callback)(_entity_t *entity);

    item_callback before_turn;
    item_callback after_turn;
    item_callback before_move;
    item_callback after_move;
    item_callback before_attack;
    item_callback after_attack;

    model_t *model;
} entity_t;


typedef struct _scene_t {

	unsigned int id;

    glm::mat4 projection_matrix;

    glm::vec3 light1_direction;
    glm::vec3 light1_position;

    glm::vec3 camera_position;
    glm::vec3 camera_lookat;

    std::vector<entity_t> entities_pool;

    std::list<entity_t*> open_entities;
    std::list<entity_t*> active_entities;
    std::list<entity_t*> dead_entities;

    entity_t *player;

    level_t *level;

} scene_t;


void create_scene(scene_t *scene,
                  glm::mat4 projection_matrix,
                  glm::vec3 camera_location,
                  glm::vec3 camera_lookat);

void draw_scene(scene_t *scene);

entity_t * request_scene_entity(scene_t *scene, glm::vec3 position, model_t *model);

void draw_entity(entity_t *entity);

#endif