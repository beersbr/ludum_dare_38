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

#define DEFAULT_ANIMATION_TICKS 150

typedef float (*animation_function)(float t, float d);

typedef struct _animtion_t {
    bool is_done;
    unsigned long start_tick;
    unsigned long duration;
    animation_function function;
} animation_t;

typedef struct _entity_t {
    unsigned int id;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::vec2 level_coordinate;

    glm::vec3 animation_start_position;
    glm::vec3 animation_end_position;

    bool is_animating;
    animation_t animation;

    bool is_static;

    bool is_tile;
    tile_t *tile;

    // Enemy stuff
    bool is_enemy;
    bool enemy_can_move;
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
    std::string level_name;

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

    level_t level;

} scene_t;


void create_animation(animation_t *animation,
                      unsigned long start,
                      unsigned long duration,
                      animation_function fn);

float eval_animation(animation_t * animation, unsigned long current_tick);

float linear(float t, float d);
float ease_out(float t, float d);
float ease_out_circ(float t, float d);

void create_scene(scene_t *scene,
                  glm::mat4 projection_matrix,
                  glm::vec3 camera_location,
                  glm::vec3 camera_lookat,
                  glm::vec2 level_size);

void draw_scene(scene_t *scene);

entity_t * request_scene_entity(scene_t *scene, glm::vec3 position, model_t *model);

void draw_entity(entity_t *entity);

#endif