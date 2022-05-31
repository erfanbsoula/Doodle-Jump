#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "sdl/rsdl.hpp"
#include "env.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "gui.hpp"
#include <vector>

class Engine
{
private:

    GUI* ui;
    Map* map;
    Player* player;
    std::vector<Entity*> entities;
    static const int TIME_FRAME;
    static const int HUNIT;

    int base;
    bool exit_flag;
    int max_points;

    void welcome();
    bool welcome_event_handler();
    void main_event_handler();
    void collision_handler();
    void key_press_handler(char key);
    void key_release_handler();
    void update_entities();
    void check_impact(Entity* entity, Entity::Surface surf, Point colp);
    void impact_execution(Point colp, Entity::Impact impact);
    void check_death(Entity* entity);
    void collision_detect(Entity* entity);
    bool are_equal_points(Point p1, Point p2);
    void set_base();
    void point_tracker();
    void check_player_pos();
    void wait_to_quit();
    void end_game();

public:

    static const int WINDOW_WIDTH;
    static const int WINDOW_HEIGHT;
    Engine();
    void run();
};

#endif