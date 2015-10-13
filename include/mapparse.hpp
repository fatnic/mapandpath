#ifndef _MAPPARSE_HPP
#define _MAPPARSE_HPP

#include <SFML/System/Vector2.hpp>
#include "pugixml.hpp"
#include "structs.hpp"

class MapParse
{
public:
    MapParse();
    void load(const char* filename);
    sf::Vector2i getMapSize();
    sf::Vector2i getTileSize();
    sf::Vector2i getGridSize();
    std::vector<Wall> getWalls();
    Point getSpawnPoint(const char* name);
    void draw(sf::RenderWindow* window);
private:
    pugi::xml_document tmx;
    sf::Vector2i _tile;
    sf::Vector2i _grid;
    sf::Vector2i _tileset;
    sf::Vector2i _map;
	std::vector<int> _floor;
	std::vector<Wall> _walls;
    sf::Texture* texture;
};

#endif
