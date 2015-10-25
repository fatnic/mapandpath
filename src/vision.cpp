#include "vision.hpp"
#include "tools/tools.hpp"
#include "tools/drawtools.hpp"
#include <iostream>
#include <cmath>

Vision::Vision(MapParse* mp, sf::RenderWindow* window)
    :_mp(mp)
    , _window(window)
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

    float min_fov = Tools::normalizeRad(_heading - half_fov);
    float max_fov = Tools::normalizeRad(_heading + half_fov);

    /* Ray minf; */
    /* minf.start = getSource(); */
    /* minf.end.x = (int)minf.start.x + _raylineMax * std::cos(min_fov); */
    /* minf.end.y = (int)minf.start.y + _raylineMax * std::sin(min_fov); */
    /* DrawTools::drawLine(minf.start, minf.end, sf::Color::Blue, _window); */

    /* Ray maxf; */
    /* maxf.start = getSource(); */
    /* maxf.end.x = (int)maxf.start.x + _raylineMax * std::cos(max_fov); */
    /* maxf.end.y = (int)maxf.start.y + _raylineMax * std::sin(max_fov); */
    /* DrawTools::drawLine(maxf.start, maxf.end, sf::Color::Blue, _window); */

    std::vector<float> _angles;

    _angles.push_back(min_fov);
    _angles.push_back(max_fov);

    for(Wall* wall : _mp->getWalls())
    {
        for(std::size_t i = 0; i < 4; i++)
        {
            float a = std::atan2(wall->points[i].y - getSource().y, wall->points[i].x - getSource().x);

            if(!Tools::radiansBetween(a, min_fov, max_fov)) continue;

            _angles.push_back(Tools::normalizeRad(a) - 0.0001);
            _angles.push_back(Tools::normalizeRad(a));
            _angles.push_back(Tools::normalizeRad(a) + 0.0001);
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

}

std::vector<Segment*> Vision::getSegments()
{
    return _segments;
}

bool Vision::collision(sf::Shape& other)
{
    Point p(other.getPosition().x, other.getPosition().y);

    if(!_light.getBounds().contains(p.x, p.y)) return false;

    for(std::size_t i = 0; i < _drawPoints.size(); i++)
    {
        if (i == _drawPoints.size() - 1)  return false;

        Point t1 = getSource();
        Point t2 = _drawPoints[i];
        Point t3 = _drawPoints[i+1];

        if(Tools::pointInTriangle(t1, t2, t3, p))
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
