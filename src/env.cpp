#include "env.hpp"

const unsigned int Timer::SEC = 60;
Timer::Timer(unsigned int dur)
{
    time = 0;
    time_point = 0;
    duration = dur;
    timer_on = true;
}
Timer::Timer()
{
    time = 0;
    time_point = 0;
    duration = 0;
    timer_on = false;
}
void Timer::time_passed() { time++; }
void Timer::set_timer(unsigned int dur)
{
    timer_on = true;
    time_point = time;
    duration = dur;
}
bool Timer::timer_alarm()
{
    if (time - time_point >= duration && timer_on)
    {
        timer_on = false;
        return true;
    }
    
    return false;
}

Hline::Hline(int h_, int x1_, int x2_)
{
    y = h_;
    x1 = x1_;
    x2 = x2_;
}

Vline::Vline(int w_, int y1_, int y2_)
{
    x = w_;
    y1 = y1_;
    y2 = y2_;
}

const Point Line::NO_COLLISION = Point(-1, -1);
Line::Line(Point p1_, Point p2_, int x_bias_)
{
    x_bias = x_bias_;
    int pr_x = p1_.x + x_bias - 1;
    p1 = Point(pr_x, p1_.y);
    int cur_x = p2_.x + x_bias - 1;
    p2 = Point(cur_x, p2_.y);
}
Line::Line() {}
void Line::calc_slope()
{
    yslope = (p2.x - p1.x) / (p2.y - p1.y);
    xslope = 1 / yslope;
}
bool Line::is_between(float b, int a1, int a2)
{
    if (a2 < a1)
        return a2 <= b && b <= a1;
    
    return a1 <= b && b <= a2;
}
float Line::find_x(int y)
{
    int dif = y - p1.y;
    return p1.x + dif * yslope;
}
float Line::find_y(int x)
{
    int dif = x - p1.x;
    return p1.y + dif * xslope;
}
Point Line::hline_collision(Hline hline)
{
    if (p1.y > hline.y && p2.y > hline.y)
        return NO_COLLISION;
    if (p1.y < hline.y && p2.y < hline.y)
        return NO_COLLISION;

    float x = find_x(hline.y);
    if (is_between(x, hline.x1, hline.x2))
        return Point(int(x)-x_bias+1, hline.y);
    
    return NO_COLLISION;
}
Point Line::vline_collision(Vline vline)
{
    if (p1.x > vline.x && p2.x > vline.x)
        return NO_COLLISION;
    if (p1.x < vline.x && p2.x < vline.x)
        return NO_COLLISION;

    float y = find_y(vline.x);
    if (is_between(y, vline.y1, vline.y2))
        return Point(vline.x-x_bias+1, int(y));
    
    return NO_COLLISION;
}

/* ******************************************************* */
const Point Movement::NO_COLLISION = Line::NO_COLLISION;
Movement::Movement(Line mleft_, Line mid_, Line mright_)
{
    mleft = mleft_;
    mmid = mid_;
    mright = mright_;
}
bool Movement::are_equal_points(Point p1, Point p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

Point Movement::hline_collision(Hline hline)
{
    Point colp_left = mleft.hline_collision(hline);
    Point colp_mid = mmid.hline_collision(hline);
    Point colp_right = mright.hline_collision(hline);
    if (!are_equal_points(colp_left, NO_COLLISION))
        return colp_left;
    if (!are_equal_points(colp_mid, NO_COLLISION))
        return colp_mid;
    if (!are_equal_points(colp_right, NO_COLLISION))
        return colp_right;
    
    return NO_COLLISION;
}

Point Movement::vline_collision(Vline vline)
{
    Point colp_left = mleft.vline_collision(vline);
    Point colp_mid = mmid.vline_collision(vline);
    Point colp_right = mright.vline_collision(vline);
    if (!are_equal_points(colp_left, NO_COLLISION))
        return colp_left;
    if (!are_equal_points(colp_mid, NO_COLLISION))
        return colp_mid;
    if (!are_equal_points(colp_right, NO_COLLISION))
        return colp_right;
    
    return NO_COLLISION;
}