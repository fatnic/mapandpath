#include "mapparse.hpp"
#include "pugixml.hpp"
#include <iostream>

MapParse::MapParse()
    :tmx()
{ }

void MapParse::load(const char* filename)
{
    tmx.load_file(filename);

    _tile.x = tmx.child("map").child("tileset").attribute("tilewidth").as_int();
    _tile.y = tmx.child("map").child("tileset").attribute("tileheight").as_int();

    _grid.x = tmx.child("map").attribute("width").as_int();
    _grid.y = tmx.child("map").attribute("height").as_int();

    _tileset.x = tmx.child("map").child("tileset").child("image").attribute("width").as_int() / _tile.x;
    _tileset.y = tmx.child("map").child("tileset").child("image").attribute("height").as_int() / _tile.y;

    _map.x = _grid.x * _tile.x;
    _map.y = _grid.y * _tile.y;

	pugi::xml_node floor = tmx.child("map").find_child_by_attribute("layer", "name", "floor").child("data");
	for(pugi::xml_node tile : floor.children())
		_floor.push_back(tile.attribute("gid").as_int());

    sf::Image imgTileset;
    std::string tilesetfile = tmx.child("map").child("tileset").child("image").attribute("source").as_string();
    imgTileset.loadFromFile("assets/" + tilesetfile);

    texture = new sf::Texture();
    texture->create(_map.x, _map.y);

    for(unsigned int i = 0; i < _floor.size(); i++)
    {
        sf::Image imgTile;
        imgTile.create(_tile.x, _tile.y);

        int sx = ((_floor[i]-1) % _tileset.x) * _tile.x;
        int sy = ((_floor[i]-1) / _tileset.y) * _tile.y;

        imgTile.copy(imgTileset, 0, 0, sf::IntRect(sx, sy, _tile.x, _tile.y), false);

        int tx = (i % _grid.x) * _tile.x;
        int ty = (i / _grid.x) * _tile.y;

        texture->update(imgTile, tx, ty);
    }

    pugi::xml_node walls = tmx.child("map").find_child_by_attribute("objectgroup", "name", "walls");
    for(pugi::xml_node wall : walls.children())
    {
       int x      = wall.attribute("x").as_int();
       int y      = wall.attribute("y").as_int();
       int width  = wall.attribute("width").as_int();
       int height = wall.attribute("height").as_int();
       Wall* w = new Wall(x, y, width, height);
       _walls.push_back(w);
    }
    
}

sf::Vector2i MapParse::getMapSize()
{
    return _map;
}

sf::Vector2i MapParse::getTileSize()
{
    return _tile;
}

sf::Vector2i MapParse::getGridSize()
{
    return _grid;
}

std::vector<Wall*> MapParse::getWalls()
{
    return _walls;
}

Point MapParse::getSpawnPoint(const char* name)
{
    pugi::xml_node spawnNode = tmx.child("map").find_child_by_attribute("objectgroup", "name", "spawn").find_child_by_attribute("object", "name", name);
    Point spawn;
    spawn.x = spawnNode.attribute("x").as_int();
    spawn.y = spawnNode.attribute("y").as_int();
    return spawn;
}

void MapParse::draw(sf::RenderWindow* window)
{
    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(_map.x, _map.y));
    bg.setTexture(texture);
    window->draw(bg);
}
