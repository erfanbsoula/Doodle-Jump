#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "sdl/rsdl.hpp"
#include "env.hpp"

class Entity
{
protected:

    Point position;
    int width;
    int height;

public:

    enum Impact {
        DIE,
        PASS,
        JUMP1,
        JUMP2,
        NONE
    };
    enum Direction {
        UPWARD,
        DOWNWARD
    };
    enum Surface {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    Entity(Point position_, int width_, int height_);
    bool is_inside(int base, int top);
    Point get_position();
    int get_width();
    int get_height();
    Hline up_surface_hline();
    Hline down_surface_hline();
    Vline left_surface_vline();
    Vline right_surface_vline();
    virtual std::string get_image() = 0;
    virtual Impact collision_impact(Surface sur, Direction dir) = 0;
    virtual void update();
    virtual bool is_enemy();
};

/* ******************************************************* */
class Player : public Entity
{
private:
    static const int WIDTH;
    static const int HEIGHT;
    static const std::string IMG[];
    int window_width;
    enum Imcase
    {
        LEFT = 0,
        RIGHT = 1,
        LEFT_JMP = 2,
        RIGHT_JMP = 3
    };
    enum Hdir {
        L,
        R,
        NONE
    };
    Imcase image;
    Direction dir;
    Hdir hdir;
    Timer jmp_timer;
    static const int VSPEED1;
    static const int VSPEED2;
    static const int HSPEED;
    static const int POTENTIAL;
    static const int POTENTIAL2;
    int vspeed;
    int potential;
    Point previous_position;

    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void turn_img_left();
    void turn_img_right();
    void set_jump_img();
    void reset_jump_img();

public:
    
    Player(int x_pos, int window_width_);
    void update();
    std::string get_image();
    void go_right();
    void go_left();
    void stop();
    void jump1(Point pos);
    void jump2(Point pos);
    Impact collision_impact(Surface sur, Direction dir);
    Direction get_direction();
    Movement get_movement();
    Movement get_head_movement();
};

/* ******************************************************* */
class Enemy : public Entity
{
private:
    static const int WIDTH;
    static const int HEIGHT;
    static const std::string IMG;

public:
    Enemy(Point position_);
    std::string get_image();
    Impact collision_impact(Surface sur, Direction dir);
    bool is_enemy();
};

/* ******************************************************* */
class Platform : public Entity
{
protected:
    static const int WIDTH;
    static const int HEIGHT;

public:
    Platform(Point position_);
    virtual std::string get_image() = 0;
    virtual Impact collision_impact(Surface sur, Direction dir) = 0;
};

/* ******************************************************* */
class Nplatform : public Platform
{
private:
    static const std::string IMG;

public:
    Nplatform(Point position_);
    std::string get_image();
    Impact collision_impact(Surface sur, Direction dir);
};

/* ******************************************************* */
class Mplatform : public Platform
{
private:
    static const std::string IMG;
    static const int SPEED;
    int window_width;

public:
    Mplatform(Point position_, int window_width_);
    void update();
    std::string get_image();
    Impact collision_impact(Surface sur, Direction dir);
};

/* ******************************************************* */
class Bplatform : public Platform
{
private:
    static const std::string IMG[];
    static const int FALL_SPEED;
    static const int BHEIGHT[];
    int vspeed;
    int image;
    int bh;
    Timer break_timer;

public:
    Bplatform(Point position_);
    void update();
    std::string get_image();
    Impact collision_impact(Surface sur, Direction dir);
};

/* ******************************************************* */

class Spring : public Entity
{
private:
    static const int WIDTH;
    static const int HEIGHT;
    static const int HEIGHT2;
    static const std::string IMG[];
    int image;
    Timer stretch_timer;

public:
    Spring(Point position_);
    void update();
    std::string get_image();
    Impact collision_impact(Surface sur, Direction dir);
};

#endif