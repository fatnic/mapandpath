#ifndef _STRUCTS_HPP
#define _STRUCTS_HPP

#include <SFML/Graphics.hpp>

typedef sf::Vector2<double> Point;

struct Segment
{
    Segment(const Point p1, const Point p2)
        : p1(p1)
	    , p2(p2)
    {}
    Point p1, p2;
};

struct Wall
{
	Segment* segments[4];
    sf::IntRect AABB;

    Wall(const int x, const int y, const int width, const int height)
    {
	    segments[0] = new Segment(Point(x, y), Point(x + width, y));
        segments[1] = new Segment(Point(x + width, y), Point(x + width, y + height));
        segments[2] = new Segment(Point(x + width, y + height), Point(x, y + height));
        segments[3] = new Segment(Point(x, y + height), Point(x, y)); 

        AABB.left = x;
        AABB.top = y;
        AABB.width = width;
        AABB.height = height;
    }

    void draw(sf::RenderTarget* window)
    {
        sf::RectangleShape wall;
        wall.setPosition(AABB.left, AABB.top);
        wall.setSize(sf::Vector2f(AABB.width, AABB.height));
        wall.setFillColor(sf::Color::Green);
        window->draw(wall);
    }
};


#endif
