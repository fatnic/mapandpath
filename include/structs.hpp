#ifndef _STRUCTS_HPP
#define _STRUCTS_HPP

#include <SFML/Graphics.hpp>

typedef sf::Vector2<double> Point;

/* std::ostream& operator<<(std::ostream& os, const Point& p) */
/* { */
/*     os << p.x << ":" << p.y; */
/*     return os; */
/* }; */

struct Ray
{
    Point start, end;
};

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
    Point points[4];
    sf::FloatRect AABB;

    Wall(const int x, const int y, const int width, const int height)
    {
	    segments[0] = new Segment(Point(x, y), Point(x + width, y));
        segments[1] = new Segment(Point(x + width, y), Point(x + width, y + height));
        segments[2] = new Segment(Point(x + width, y + height), Point(x, y + height));
        segments[3] = new Segment(Point(x, y + height), Point(x, y)); 

        for(std::size_t i = 0; i < 4; i++)
            points[i] = segments[i]->p1;

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


#endif /* !_STRUCTS_HPP */
