#include "entity.hpp"

const std::string Player::IMG[] = {

    "assets/left.png", 
    "assets/right.png",
    "assets/left_jumping.png",
    "assets/right_jumping.png"
};
const std::string Enemy::IMG = "assets/enemy.png";
const std::string Nplatform::IMG = "assets/platform.png";
const std::string Mplatform::IMG = "assets/mplatform.png";
const std::string Bplatform::IMG[] = {

    "assets/bplatform.png",
    "assets/bplatform_break1.png",
    "assets/bplatform_break2.png",
    "assets/bplatform_break3.png"
};
const std::string Spring::IMG[] = {
    
    "assets/spring-normal.png",
    "assets/spring_stretched.png"
};

/* ******************************************************* */
Entity::Entity(Point position_, int width_, int height_)
{
    position = position_;
    width = width_;
    height = height_;
}

void Entity::update() {}
bool Entity::is_inside(int base, int top)
{
    return (position.y + height) >= base && position.y < top;
}

int Entity::get_width() { return width; }
int Entity::get_height() { return height; }
Point Entity::get_position() { return position; }

Hline Entity::up_surface_hline()
{
    int y = position.y + height - 1;
    int x2 = position.x + width - 1;
    return Hline(y, position.x, x2);
}
Hline Entity::down_surface_hline()
{
    int x2 = position.x + width - 1;
    return Hline(position.y, position.x, x2); 
}
Vline Entity::left_surface_vline()
{
    int y2 = position.y + height - 1;
    return Vline(position.x, position.y, y2);
}
Vline Entity::right_surface_vline()
{
    int y2 = position.y + height - 1;
    int x = position.x + width - 1;
    return Vline(x, position.y, y2);
}

bool Entity::is_enemy() { return false; }

/* ******************************************************* */
const int Player::WIDTH = 124;
const int Player::HEIGHT = 120;
const int Player::POTENTIAL = 369;
const int Player::POTENTIAL2 = 623;
const int Player::VSPEED1 = 6;
const int Player::VSPEED2 = 9;
const int Player::HSPEED = 5;

Player::Player(int x_pos, int window_width_) : Entity(Point(x_pos, 0), WIDTH, HEIGHT)
{
    window_width = window_width_;
    image = Imcase::LEFT;
    dir = Direction::UPWARD;
    hdir = Hdir::NONE;
    potential = POTENTIAL;
    previous_position = position;
    vspeed = VSPEED1;
}

void Player::turn_img_left()
{
    if (image == Imcase::RIGHT)
        image = Imcase::LEFT;
    else if (image == Imcase::RIGHT_JMP)
        image = Imcase::LEFT_JMP;
}

void Player::turn_img_right()
{
    if (image == Imcase::LEFT)
        image = Imcase::RIGHT;
    else if (image == Imcase::LEFT_JMP)
        image = Imcase::RIGHT_JMP;
}

void Player::set_jump_img()
{
    if (image == Imcase::RIGHT)
        image = Imcase::RIGHT_JMP;
    else if (image == Imcase::LEFT)
        image = Imcase::LEFT_JMP;
}
void Player::reset_jump_img()
{
    if (image == Imcase::RIGHT_JMP)
        image = Imcase::RIGHT;
    else if (image == Imcase::LEFT_JMP)
        image = Imcase::LEFT;
}

void Player::jump1(Point pos)
{
    position = pos;
    potential = POTENTIAL;
    dir = Direction::UPWARD;
    set_jump_img();
    jmp_timer.set_timer(Timer::SEC/4);
}
void Player::jump2(Point pos)
{
    position = pos;
    potential = POTENTIAL2;
    dir = Direction::UPWARD;
    set_jump_img();
    jmp_timer.set_timer(Timer::SEC/3);
    vspeed = VSPEED2;
}

void Player::go_left()
{
    hdir = Hdir::L;
    turn_img_left();
}
void Player::go_right()
{
    hdir = Hdir::R;
    turn_img_right();
}
void Player::stop() { hdir = Hdir::NONE; }

Movement Player::get_movement()
{
    Line left(previous_position, position, width/4);
    Line mid(previous_position, position, width/2);
    Line right(previous_position, position, 3*width/4);
    return Movement(left, mid, right);
}

Movement Player::get_head_movement()
{
    Point ppos(previous_position.x, previous_position.y + 5*height/8);
    Point cpos(position.x, position.y + 3*height/4);
    Line left(ppos, cpos, width/4);
    Line mid(ppos, cpos, width/2);
    Line right(ppos, cpos, 3*width/4);
    return Movement(left, mid, right);
}

Entity::Direction Player::get_direction() { return dir; }
std::string Player::get_image() { return IMG[image]; }
Entity::Impact Player::collision_impact(Surface sur, Direction dir)
{
    return Impact::NONE; 
}

void Player::move_up()
{
    if (potential - vspeed >= 0)
    {
        position.y += vspeed;
        potential -= vspeed;
    }
    else
    {
        position.y += potential;
        potential = 0;
        dir = Direction::DOWNWARD;
        vspeed = VSPEED1;
    }
}

void Player::move_down()
{
    position.y -= vspeed;
}

void Player::move_left()
{
    if (position.x - HSPEED + width >= 0)
        position.x -= HSPEED;
    else
        position.x = window_width - HSPEED;
}

void Player::move_right()
{
    if (position.x + HSPEED < window_width)
        position.x += HSPEED;
    else
        position.x = HSPEED - width;
}

void Player::update()
{
    jmp_timer.time_passed();
    if (jmp_timer.timer_alarm())
        reset_jump_img();

    previous_position = position;
    if (dir == UPWARD)
        move_up();
    else if (dir == DOWNWARD)
        move_down();

    if (hdir == Hdir::L)
        move_left();
    else if (hdir == Hdir::R)
        move_right();
}

/* ******************************************************* */
const int Enemy::WIDTH = 136;
const int Enemy::HEIGHT = 184;
Enemy::Enemy(Point position_) : Entity(position_, WIDTH, HEIGHT) {}
std::string Enemy::get_image() { return IMG; }
Entity::Impact Enemy::collision_impact(Surface sur, Direction dir)
{
    return Impact::DIE;
}
bool Enemy::is_enemy() { return true; }

/* ******************************************************* */
const int Platform::WIDTH = 120;
const int Platform::HEIGHT = 36;
Platform::Platform(Point position_) : Entity(position_, WIDTH, HEIGHT) {}

Nplatform::Nplatform(Point position_) : Platform(position_) {}
std::string Nplatform::get_image() { return IMG; }
Entity::Impact Nplatform::collision_impact(Surface sur, Direction dir)
{
    if (sur == Surface::UP && dir == Direction::DOWNWARD)
        return Impact::JUMP1;

    return Impact::PASS;
}

const int Mplatform::SPEED = 3;
Mplatform::Mplatform(Point position_, int window_width_) :
    Platform(position_)
{
    window_width = window_width_;
}
std::string Mplatform::get_image() { return IMG; }
Entity::Impact Mplatform::collision_impact(Surface sur, Direction dir)
{
    if (sur == Surface::UP && dir == Direction::DOWNWARD)
        return Impact::JUMP1;

    return Impact::PASS;
}
void Mplatform::update()
{
    static bool flag = true;
    if (flag == true)
    {
        if (position.x + width + SPEED < window_width)
            position.x += SPEED;
        else
            flag = false;
    }
    else
    {
        if (position.x - SPEED >= 0)
            position.x -= SPEED;
        else
            flag = true;
    }
}

const int Bplatform::FALL_SPEED = 8;
const int Bplatform::BHEIGHT[] = { 45, 59, 68 };
Bplatform::Bplatform(Point position_) : Platform(position_)
{
    vspeed = 0;
    image = 0;
    bh = 0;
}
std::string Bplatform::get_image() { return IMG[image]; }
Entity::Impact Bplatform::collision_impact(Surface sur, Direction dir)
{
    static bool broken = false;
    if (dir == Direction::DOWNWARD && !broken)
    {
        broken = true;
        vspeed = FALL_SPEED;
        image += 1;
        height = BHEIGHT[bh];
        break_timer.set_timer(Timer::SEC/6);
    }

    return Impact::PASS;
}
void Bplatform::update()
{
    break_timer.time_passed();
    if (break_timer.timer_alarm() && image < 3)
    {
        break_timer.set_timer(Timer::SEC/10);
        image += 1;
        height = BHEIGHT[bh];
        bh += 1;
    }

    position.y -= vspeed;
}

/* ******************************************************* */
const int Spring::WIDTH = 56;
const int Spring::HEIGHT = 46;
const int Spring::HEIGHT2 = 64;
Spring::Spring(Point position_) : Entity(position_, WIDTH, HEIGHT)
{
    image = 0;
}
std::string Spring::get_image() { return IMG[image]; }
Entity::Impact Spring::collision_impact(Surface sur, Direction dir)
{
    if (sur == Surface::UP && dir == Direction::DOWNWARD)
    {
        stretch_timer.set_timer(Timer::SEC/3);
        image = 1;
        height = HEIGHT2;
        return Impact::JUMP2;
    }

    return Impact::PASS;
}
void Spring::update()
{
    stretch_timer.time_passed();
    if (stretch_timer.timer_alarm())
    {
        height = HEIGHT;
        image = 0;
    }
}