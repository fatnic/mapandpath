#include "vision.hpp"
#include "tools/tools.hpp"
#include "tools/drawtools.hpp"
#include <iostream>

Vision::Vision(MapParse* mp, sf::RenderWindow* window)
    :_mp(mp)
    ,_window(window)
{
    raylineMax = std::sqrt((_mp->getMapSize().x * _mp->getMapSize().x) + (_mp->getMapSize().y * _mp->getMapSize().y));
}

void Vision::setSource(Point source)
{
    _source = source;
}

Point Vision::getSource()
{
    return _source;
}

sf::ConvexShape Vision::run()
{
    sf::ConvexShape _light;

    _drawPoints.clear();

    std::vector<float> _angles;

    for(Wall* wall : _mp->getWalls())
    {
        for(std::size_t i = 0; i < 4; i++)
        {
            float a = std::atan2(wall->points[i].y - getSource().y, wall->points[i].x - getSource().x);
            _angles.push_back(a - 0.00001);
            _angles.push_back(a);
            _angles.push_back(a + 0.00001);
        }
    }

    std::sort(_angles.begin(), _angles.end());

    Ray ray;
    ray.start = getSource();

    for(float angle : _angles)
    {
        ray.end.x = (int)ray.start.x + raylineMax * std::cos(angle);
        ray.end.y = (int)ray.start.y + raylineMax * std::sin(angle);

        std::vector<Point> intersections;
        for(Wall* wall : _mp->getWalls())
        {
            for(std::size_t i = 0; i < 4; i++)
            {
                Point intersection;
                if(Tools::getIntersection(ray, wall->segments[i], intersection))
                {
                    intersections.push_back(intersection);
                }
            }
        }
        std::unique(intersections.begin(), intersections.end());
        std::sort(intersections.begin(), intersections.end(), [ray](Point& p1, Point& p2){ return Tools::distance(p1, ray.start) < Tools::distance(p2, ray.start); });
        _drawPoints.push_back(intersections[0]);
    }

    _light.setPointCount(_drawPoints.size());
    for(std::size_t i = 0; i < _drawPoints.size(); i++)
        _light.setPoint(i, sf::Vector2f(_drawPoints[i].x, _drawPoints[i].y));
    _light.setFillColor(sf::Color(255,255,255,50));

    return _light;
}

Vision::~Vision()
{
    delete _mp;
    delete _window;
}
