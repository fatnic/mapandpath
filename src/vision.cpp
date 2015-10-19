#include "vision.hpp"
#include "tools/tools.hpp"
#include "tools/drawtools.hpp"
#include <iostream>

Vision::Vision(MapParse* mp, sf::RenderWindow* window)
    :_mp(mp)
    ,_window(window)
    ,_fov(360)
{
    raylineMax = std::sqrt((_mp->getMapSize().x * _mp->getMapSize().x) + (_mp->getMapSize().y * _mp->getMapSize().y));
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

            _angles.push_back(Tools::normalizeAngle(a) - 0.000001);
            _angles.push_back(Tools::normalizeAngle(a));
            _angles.push_back(Tools::normalizeAngle(a) + 0.000001);
        }
    }

    std::sort(_angles.begin(), _angles.end());
    auto it = std::find(_angles.begin(), _angles.end(), min_fov);
    std::rotate(_angles.begin(), it, _angles.end());

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

        // TODO: Get rid of this.. refactor to simple closestIntersection = least distance
        std::unique(intersections.begin(), intersections.end());
        std::sort(intersections.begin(), intersections.end(), [ray](Point& p1, Point& p2){ return Tools::distance(p1, ray.start) < Tools::distance(p2, ray.start); });

        _drawPoints.push_back(intersections[0]);
    }

    sf::Vertex tripoint;
    tripoint.position = sf::Vector2f(getSource().x, getSource().y);
    tripoint.color = _colour;
    _light.append(tripoint);

    for(std::size_t i = 0; i < _drawPoints.size(); i++)
    {
        tripoint.position = sf::Vector2f(_drawPoints[i].x, _drawPoints[i].y);
        _light.append(tripoint);
    }

    /* tripoint.position = sf::Vector2f(_drawPoints[0].x, _drawPoints[0].y); */
    /* _light.append(tripoint); */

    return _light;
}

Vision::~Vision()
{
}
