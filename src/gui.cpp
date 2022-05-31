#include "gui.hpp"
#include <sstream>

const std::string GUI::WINDOW_NAME = "doodlejump";
const std::string GUI::BACKGROUND_IMG = "assets/background.png";

GUI::GUI(int width_, int height_)
{
    width = width_;
    height = height_;
    window = new Window(width, height, WINDOW_NAME);
    base = 0;
    display_background();
}

void GUI::display_background()
{
    window->draw_img(BACKGROUND_IMG, Rectangle(0, 0, width, height));
}

Rectangle GUI::area_todisp(Entity* entity)
{
    Rectangle area(0, 0, 0, 0);
    Point down_left = entity->get_position();
    area.h = entity->get_height();
    area.w = entity->get_width();
    area.y = base + height - down_left.y - area.h;
    area.x = down_left.x;
    return area;
}

void GUI::display_entity(Entity* entity)
{
    window->draw_img(entity->get_image(), area_todisp(entity));
}

void GUI::display_entities(std::vector<Entity*> entities)
{
    for (int index = 0; index < entities.size(); index++)
        display_entity(entities[index]);
}

void GUI::display(std::vector<Entity*> entities, Entity* player)
{
    window->clear();
    display_background();
    display_entities(entities);
    display_entity(player);
    window->update_screen();
}

void GUI::set_base(int base_) { base = base_; }
bool GUI::has_pending_event() { return window->has_pending_event(); }
Event GUI::poll_for_event() { return window->poll_for_event(); }

void GUI::display_losing_screen(int point)
{
    window->clear();
    display_background();
    Point p(25, 10);
    window->show_text("OOPS!! You Lost...", p, RED, "FreeSans.ttf", 40);
    std::stringstream msg;
    msg << "Your point was " << point;
    p.y += 40;
    window->show_text(msg.str(), p, BLACK, "FreeSans.ttf", 30);
    window->update_screen();
}

void GUI::display_blinking_entity(Entity* player)
{
    static bool visible = true;
    static Timer timer(Timer::SEC/3);
    timer.time_passed();
    if (timer.timer_alarm())
    {
        visible = (visible + 1) % 2;
        timer.set_timer(Timer::SEC/3);
    }
    if (visible)
        display_entity(player);
}

void GUI::display_welcome_screen(Entity* player)
{
    window->clear();
    display_background();
    display_blinking_entity(player);
    window->update_screen();
}