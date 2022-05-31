#ifndef _MAP_H_
#define _MAP_H_

#include "sdl/rsdl.hpp"
#include "entity.hpp"
#include <vector>
#include <fstream>

class Map
{
private:
    static const std::string MAPFILE;
    std::vector<Entity*> entities; 
    int height;
    int width;

    void load_entity(std::ifstream& file);

public:
    Map(int width_, int height_);
    std::vector<Entity*> get_entities(int base);
};

#endif