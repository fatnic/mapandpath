#ifndef _VISION_HPP
#define _VISION_HPP

#include "mapparse.hpp"
#include "structs.hpp"


class Vision
{
public:
    Vision(MapParse* mp, sf::RenderWindow* window);
    void setSource(Point source);
    Point getSource();
    sf::VertexArray run();
    ~Vision();
private:
    MapParse* _mp;
    sf::RenderWindow* _window;
    Point _source;
    std::vector<RayLine*> _raylines;
    std::vector<Point> _drawPoints;
    float raylineMax;

    bool isSegmentFacing(Segment* segment, int segmentNum);
    bool isPointBoundary(Wall* wall, int pointNum); 
    RayLine* calcRayLine(Wall* wall, int pointNum);
};
#endif /* !_VISION_HPP */
