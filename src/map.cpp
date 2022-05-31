#include "map.hpp"

const std::string Map::MAPFILE = "map.txt";

Map::Map(int width_, int height_)
{
    width = width_;
    height = height_;
    std::ifstream file(MAPFILE);
    int entity_count;
    file >> entity_count;
    for (int i = 0; i < entity_count; i++)
        load_entity(file);
}

void Map::load_entity(std::ifstream& file)
{
    int x, y;
    std::string entity_type;
    file >> x >> y >> entity_type;
    Point position_ = Point(x, y);

    if (entity_type == "platform")
    {
        Nplatform* pl = new Nplatform(position_);
        entities.push_back(pl);
    }
    else if (entity_type == "mplatform")
    {
        Mplatform* pl = new Mplatform(position_, width);
        entities.push_back(pl);
    }
    else if (entity_type == "bplatform")
    {
        Bplatform* pl = new Bplatform(position_);
        entities.push_back(pl);
    }
    else if (entity_type == "spring")
    {
        Spring* sp = new Spring(position_);
        entities.push_back(sp);
    }
    else if (entity_type == "enemy")
    {
        Enemy* enm = new Enemy(position_);
        entities.push_back(enm);
    }
}

std::vector<Entity*> Map::get_entities(int base)
{
    std::vector<Entity*> appeared;
    for (int index = 0; index < entities.size(); index++)
        if (entities[index]->is_inside(base , base + height))
            appeared.push_back(entities[index]);

    return appeared;
}