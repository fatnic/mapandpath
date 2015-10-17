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
    _raylines.clear();
    _drawPoints.clear();

    for(Wall* wall : _mp->getWalls())
    {
        for(std::size_t i = 0; i < 4; i++)
        {
            _raylines.push_back(calcRayLine(wall, i));
        }
    }

    std::sort(_raylines.begin(), _raylines.end(), [](RayLine* rl1, RayLine* rl2){ return rl1->angle < rl2->angle; });

    for(Point p : _drawPoints)
        DrawTools::drawCircle(5, p, sf::Color::White, _window);

    for(RayLine* rl : _raylines)
    {
        DrawTools::drawLine(rl->ray.start, rl->ray.end, sf::Color(255,255,255,10), _window);
        for(Point p : rl->intersections)
            DrawTools::drawCircle(3, p, sf::Color::Magenta, _window);
    }

    sf::ConvexShape _light(_drawPoints.size());

    for(std::size_t i = 0; i < _drawPoints.size(); i++)
        _light.setPoint(i, sf::Vector2f(_drawPoints[i].x, _drawPoints[i].y));

    _light.setFillColor(sf::Color(255,255,255,50));

    return _light;
}

RayLine* Vision::calcRayLine(Wall* wall, int pointNum)
{
    RayLine* rl = new RayLine;

    rl->point.x = (int)wall->points[pointNum].x;
    rl->point.y = (int)wall->points[pointNum].y;
    rl->ray.start = getSource();
    rl->angle = std::atan2(rl->point.y - rl->ray.start.y, rl->point.x - rl->ray.start.x);
    rl->ray.end.x = (int)rl->ray.start.x + raylineMax * std::cos(rl->angle);
    rl->ray.end.y = (int)rl->ray.start.y + raylineMax * std::sin(rl->angle);
    rl->isBoundary = isPointBoundary(wall, pointNum);

    for(Wall* w : _mp->getWalls())
    {
        for(Segment* segment : w->segments)
        {
            Point intersect;
            if(Tools::getIntersection(rl->ray, segment, intersect))
                rl->intersections.push_back(Point((int)intersect.x, (int)intersect.y));
        }
    }

    std::unique(rl->intersections.begin(), rl->intersections.end());
    std::sort(rl->intersections.begin(), rl->intersections.end(), [rl](Point& p1, Point& p2){ return Tools::distance(p1, rl->ray.start) < Tools::distance(p2, rl->ray.start); });
    return rl;
}

bool Vision::isSegmentFacing(Segment* segment, int segmentNum)
{
    switch(segmentNum)
    {
        case 0:
            if(_source.y < segment->p1.y) return true;
            break;
        case 1:
            if(_source.x > segment->p1.x) return true;
            break;
        case 2:
            if(_source.y > segment->p1.y) return true;
            break;
        case 3:
            if(_source.x < segment->p1.x) return true;
            break;
    }
    return false;
}

bool Vision::isPointBoundary(Wall* wall, int pointNum)
{
    int prevPoint = (pointNum == 0) ? 3 : pointNum - 1;
    bool s1 = isSegmentFacing(wall->segments[prevPoint], prevPoint);
    bool s2 = isSegmentFacing(wall->segments[pointNum], pointNum);
    if(s1 != s2) return true;
    return false;
}

Vision::~Vision()
{
    delete _mp;
    delete _window;
}
