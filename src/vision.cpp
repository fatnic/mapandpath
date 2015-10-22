#include "vision.hpp"
#include "tools/tools.hpp"
#include "tools/drawtools.hpp"
#include <iostream>
#include <cmath>

Vision::Vision(MapParse* mp, sf::RenderWindow* window)
    :_mp(mp)
    ,_window(window)
    ,_fov(360)
{
    _raylineMax = std::sqrt((_mp->getMapSize().x * _mp->getMapSize().x) + (_mp->getMapSize().y * _mp->getMapSize().y));
}

void Vision::setSource(Point source)
{
    _source = source;
}

void Vision::setHeading(float angle)
{
    _heading = angle;
}

void Vision::setFOV(float angle)
{
    _fov = angle;
}

void Vision::setColour(sf::Color colour)
{
    _colour = colour;
}

Point Vision::getSource()
{
    return _source;
}

sf::VertexArray Vision::run()
{
    sf::VertexArray _light;
    _light.setPrimitiveType(sf::TrianglesFan);

    if(getSource().x < 0 || getSource().x > _mp->getMapSize().x) return _light;
    if(getSource().y < 0 || getSource().y > _mp->getMapSize().y) return _light;

    float fov = Tools::deg2rad(_fov);
    float half_fov = fov / 2;

    float min_fov = Tools::normalizeAngle(_heading - half_fov);
    float max_fov = Tools::normalizeAngle(_heading + half_fov);
    
    _drawPoints.clear();

    std::vector<float> _angles;

    _angles.push_back(min_fov);
    _angles.push_back(max_fov);

    for(Wall* wall : _mp->getWalls())
    {
        for(std::size_t i = 0; i < 4; i++)
        {
            float a = std::atan2(wall->points[i].y - getSource().y, wall->points[i].x - getSource().x);

            if(!Tools::angleBetween(a, min_fov, max_fov)) continue;

            _angles.push_back(Tools::normalizeAngle(a) - 0.00001);
            _angles.push_back(Tools::normalizeAngle(a));
            _angles.push_back(Tools::normalizeAngle(a) + 0.00001);
        }
    }

    std::unique(_angles.begin(), _angles.end());
    std::sort(_angles.begin(), _angles.end());
    auto it = std::find(_angles.begin(), _angles.end(), min_fov);
    std::rotate(_angles.begin(), it, _angles.end());

    Ray ray;
    ray.start = getSource();

    Point intersection;
    for(float angle : _angles)
    {
        ray.end.x = (int)ray.start.x + _raylineMax * std::cos(angle);
        ray.end.y = (int)ray.start.y + _raylineMax * std::sin(angle);

        Point closestIntersection(-1000,-1000);
        for(Wall* wall : _mp->getWalls())
        {
            for(std::size_t i = 0; i < 4; i++)
            {
                if(Tools::getIntersection(ray, wall->segments[i], intersection))
                {
                    if(Tools::distance(intersection, ray.start) < Tools::distance(closestIntersection, ray.start))
                        closestIntersection = intersection;
                }
            }
        }

        _drawPoints.push_back(closestIntersection);
    }
    
    sf::Vertex tripoint;
    tripoint.position = sf::Vector2f(getSource().x, getSource().y);
    tripoint.color = _colour;
    _light.append(tripoint);

    for(Point& p : _drawPoints)
    {
        tripoint.position = sf::Vector2f(p.x, p.y);
        _light.append(tripoint);
    }

    return _light;
}

Vision::~Vision()
{
}
