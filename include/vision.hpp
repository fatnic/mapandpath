#ifndef _VISION_HPP
#define _VISION_HPP

#include "mapparse.hpp"
#include "structs.hpp"


class Vision
{
public:
    Vision(MapParse* mp, sf::RenderWindow* window);
    void setSource(Point source);
    void setHeading(float angle);
    void setFOV(float angle);
    void setColour(sf::Color colour);
    Point getSource();
    sf::VertexArray run();
    ~Vision();
private:
    MapParse* _mp;
    sf::RenderWindow* _window;
    Point _source;
    sf::Color _colour;
    std::vector<Point> _drawPoints;
    float _raylineMax;
    float _fov;
    float _heading;
};
#endif /* !_VISION_HPP */
