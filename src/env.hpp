#ifndef _ENV_H_
#define _ENV_H_

#include "sdl/rsdl.hpp"

class Timer
{
private:
    unsigned int time;
    unsigned int time_point;
    unsigned int duration;
    bool timer_on;

public:
    static const unsigned int SEC;
    Timer(unsigned int dur);
    Timer();
    void time_passed();
    void set_timer(unsigned int dur);
    bool timer_alarm();
};

struct Hline {
    Hline(int y_, int x1_, int x2_);
    int y;
    int x1;
    int x2;
};

struct Vline {
    Vline(int x_, int y1_, int y2_);
    int x;
    int y1;
    int y2;
};

class Line
{
private:
    Point p1;
    Point p2;
    float yslope;
    float xslope;
    int x_bias;

    void calc_slope();
    float find_x(int y);
    float find_y(int x);
    bool is_between(float b, int a1, int a2);

public:
    static const Point NO_COLLISION;
    Line(Point p1_, Point p2_, int x_bias_);
    Line();
    Point hline_collision(Hline hline);
    Point vline_collision(Vline vline); 
};

class Movement
{
private:
    Line mleft;
    Line mmid;
    Line mright;

    bool are_equal_points(Point p1, Point p2);

public:
    static const Point NO_COLLISION;
    Movement(Line mleft_, Line mid_, Line mright_);
    Point hline_collision(Hline hline);
    Point vline_collision(Vline vline); 
};

#endif