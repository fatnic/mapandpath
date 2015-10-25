#ifndef _VISION_HPP
#define _VISION_HPP

#include "mapparse.hpp"
#include "structs.hpp"
#include <SFML/Graphics/Drawable.hpp>


class Vision : public sf::Drawable
{
public:
    Vision(MapParse* mp, sf::RenderWindow* window);
    void setSource(Point source);
    void setHeading(float angle);
    void setFOV(float angle);
    void setColour(sf::Color colour);
    std::vector<Segment*> getSegments();
    Point getSource();
    bool contains(Point point);
    void run();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    ~Vision();
private:
    MapParse* _mp;
    sf::RenderWindow* _window;
    sf::VertexArray _light;
    Point _source;
    sf::Color _colour;
    std::vector<Point> _drawPoints;
    std::vector<Segment*> _segments;
    float _raylineMax;
    float _fov;
    float _heading;
};
#endif /* !_VISION_HPP */
