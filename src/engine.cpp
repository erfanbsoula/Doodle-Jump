#include "engine.hpp"

const int Engine::WINDOW_WIDTH = 640;
const int Engine::WINDOW_HEIGHT = 1004;
const int Engine::TIME_FRAME = 15;
const int Engine::HUNIT = WINDOW_WIDTH/8;
const char ENTER_KEY = 13;

Engine::Engine()
{
    ui = new GUI(WINDOW_WIDTH, WINDOW_HEIGHT);
    map = new Map(WINDOW_WIDTH, WINDOW_HEIGHT);
    player = new Player(3*HUNIT, WINDOW_WIDTH);
    exit_flag = false;
    base = 0;
    max_points = 0;
}

void Engine::update_entities()
{
    for (int index = 0; index < entities.size(); index++)
        entities[index]->update();
}

void Engine::key_press_handler(char key)
{
    if (key == 'a')
        player->go_left();
    else if (key == 'd')
        player->go_right();
}

void Engine::key_release_handler()
{
    player->stop();
}

void Engine::main_event_handler()
{
    typedef Event::EventType Etype;
    while (ui->has_pending_event())
    {
        Event event = ui->poll_for_event();
        Etype type = event.get_type();
        if (type == Etype::QUIT)
            exit_flag = true;
        else if (type == Etype::KEY_PRESS)
            key_press_handler(event.get_pressed_key());
        else if (type == Etype::KEY_RELEASE)
            key_release_handler();
    }
}

bool Engine::are_equal_points(Point p1, Point p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

void Engine::impact_execution(Point colp, Entity::Impact impact)
{
    typedef Entity::Impact Impact;
    if (impact == Impact::DIE)
        end_game();
    else if (impact == Impact::JUMP1)
        player->jump1(colp);
    else if (impact == Impact::JUMP2)
        player->jump2(colp);
}

void Engine::check_impact(Entity* entity, Entity::Surface surf, Point colp)
{
    Entity::Direction dir = player->get_direction();
    if (!are_equal_points(colp, Movement::NO_COLLISION))
        impact_execution(colp, entity->collision_impact(surf, dir));
}

void Engine::collision_detect(Entity* entity)
{
    typedef Entity::Surface Surface;
    Movement movement = player->get_movement();
    Point colp = movement.hline_collision(entity->up_surface_hline());
    check_impact(entity, Surface::UP, colp);
    colp = movement.hline_collision(entity->down_surface_hline());
    check_impact(entity, Surface::DOWN, colp);
    colp = movement.vline_collision(entity->left_surface_vline());
    check_impact(entity, Surface::LEFT, colp);
    colp = movement.vline_collision(entity->right_surface_vline());
    check_impact(entity, Surface::RIGHT, colp);
}

void Engine::check_death(Entity* entity)
{
    typedef Entity::Surface Surface;
    Movement movement = player->get_head_movement();
    Point colp = movement.hline_collision(entity->up_surface_hline());
    if (!are_equal_points(colp, Movement::NO_COLLISION))
        end_game();
    colp = movement.hline_collision(entity->down_surface_hline());
    if (!are_equal_points(colp, Movement::NO_COLLISION))
        end_game();
    colp = movement.vline_collision(entity->left_surface_vline());
    if (!are_equal_points(colp, Movement::NO_COLLISION))
        end_game();
    colp = movement.vline_collision(entity->right_surface_vline());
    if (!are_equal_points(colp, Movement::NO_COLLISION))
        end_game();
}

void Engine::collision_handler()
{
    for (int i = 0; i< entities.size(); i++)
    {
        collision_detect(entities[i]);
        if (entities[i]->is_enemy())
            check_death(entities[i]);
    }
}

bool Engine::welcome_event_handler()
{
    typedef Event::EventType Etype;
    static char key_pressed = 'v';

    while (ui->has_pending_event())
    {
        Event event = ui->poll_for_event();
        Etype type = event.get_type();
        if (type == Etype::QUIT)
        {
            exit_flag = true;
            return true;
        }
        else if (type == Etype::KEY_PRESS)
            key_pressed = event.get_pressed_key();
        else if (type == Etype::KEY_RELEASE)
            if (key_pressed == ENTER_KEY)
                return true;
    }
    return false;
}

void Engine::welcome()
{
    bool start = false;
    while (!start)
    {
        ui->display_welcome_screen(player);
        start = welcome_event_handler();
        delay(TIME_FRAME);
    }
}

void Engine::set_base()
{
    int y_pos = player->get_position().y;
    int dif = y_pos - (base + WINDOW_HEIGHT/2);
    if (dif > 0)
    {
        base += dif;
        ui->set_base(base);
    }
}

void Engine::point_tracker()
{
    int y = player->get_position().y;
    if (y > max_points)
        max_points = y;
}

void Engine::check_player_pos()
{
    if (player->get_position().y < base)
        end_game();
}

void Engine::wait_to_quit()
{
    typedef Event::EventType Etype;
    while (ui->has_pending_event())
    {
        Event event = ui->poll_for_event();
        if (event.get_type() == Etype::QUIT)
            exit_flag = true;
    }
}

void Engine::end_game()
{
    ui->display_losing_screen(max_points);
    while(!exit_flag)
        wait_to_quit();
}

void Engine::run()
{
    welcome();
    while(!exit_flag)
    {
        main_event_handler();
        player->update();
        update_entities();
        collision_handler();
        check_player_pos();
        set_base();
        point_tracker();
        entities = map->get_entities(base);
        ui->display(entities, player);
        delay(TIME_FRAME);
    }
}
