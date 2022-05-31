#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "sdl/rsdl.hpp"
#include "entity.hpp"
#include "env.hpp"
#include <vector>

class GUI
{
private:

    static const std::string BACKGROUND_IMG;
    static const std::string WINDOW_NAME;

    int width;
    int height;
    int base;
    Window* window;

    Rectangle area_todisp(Entity* entity);
    Rectangle effect_area();
    void display_background();
    void display_entity(Entity* entity);
    void display_entities(std::vector<Entity*> entities);
    void display_blinking_entity(Entity* player);

public:

    GUI(int width_, int height_);
    void set_base(int base_);
    void display_welcome_screen(Entity* player);
    void display_losing_screen(int point);
    void display(std::vector<Entity*> entities, Entity* player);
    bool has_pending_event();
    Event poll_for_event();
};

#endif