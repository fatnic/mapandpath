#include "vision.hpp"
#include "tools/tools.hpp"
#include "tools/drawtools.hpp"
#include <iostream>
#include <cmath>

Vision::Vision(MapParse* mp)
    :_mp(mp)
    , _light()
    ,_fov(360)
{
    _light.setPrimitiveType(sf::TrianglesFan);
    _raylineMax = std::sqrt((_mp->getMapSize().x * _mp->getMapSize().x) + (_mp->getMapSize().y * _mp->getMapSize().y));
}

void Vision::setSource(Point source)
{
    _source = source;
    run();
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

void Vision::run()
{
    _drawPoints.clear();
    _light.clear();

    if(getSource().x < 0 || getSource().x > _mp->getMapSize().x) return;
    if(getSource().y < 0 || getSource().y > _mp->getMapSize().y) return;

    float fov = Tools::deg2rad(_fov);
    float half_fov = fov / 2;

    float min_fov = Tools::normalizeAngle(_heading - half_fov);
    float max_fov = Tools::normalizeAngle(_heading + half_fov);

    std::vector<float> _angles;

    _angles.push_back(min_fov);
    _angles.push_back(max_fov);

    for(Wall* wall : _mp->getWalls())
    {
        for(std::size_t i = 0; i < 4; i++)
        {
            float a = std::atan2(wall->points[i].y - getSource().y, wall->points[i].x - getSource().x);

            if(!Tools::radiansBetween(a, min_fov, max_fov)) continue;

            _angles.push_back(Tools::normalizeAngle(a) - 0.0001);
            _angles.push_back(Tools::normalizeAngle(a));
            _angles.push_back(Tools::normalizeAngle(a) + 0.0001);
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

    for(auto* segment : _segments)
        delete segment;
    _segments.clear();

    Point a = getSource();
    Point b = _drawPoints.front();
    Segment* segment = new Segment(a, b);
    _segments.push_back(segment);

    for(std::size_t i = 0; i < _drawPoints.size() - 1; i++)
    {
        Point a = _drawPoints[i];
        Point b = (i+1 > _drawPoints.size()) ? getSource() : _drawPoints[i+1];
        Segment* segment = new Segment(a, b);
        _segments.push_back(segment);
    }

    a = _drawPoints.back();
    b = getSource();
    segment = new Segment(a, b);
    _segments.push_back(segment);

}

std::vector<Segment*> Vision::getSegments()
{
    return _segments;
}

bool Vision::contains(Point point)
{
    if(_light.getBounds().contains(point.x, point.y))
    {
        return true;
    }

    return false;
}

void Vision::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_light, states);    

    /* sf::RectangleShape rect; */
    /* rect.setPosition(_light.getBounds().left, _light.getBounds().top); */
    /* rect.setSize(sf::Vector2f(_light.getBounds().width, _light.getBounds().height)); */
    /* rect.setFillColor(sf::Color(0,0,0,0)); */
    /* rect.setOutlineColor(sf::Color::Green); */
    /* rect.setOutlineThickness(1.0f); */
    /* target.draw(rect); */
}

Vision::~Vision()
{
}
