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
    Point getSource();
    bool collision(sf::FloatRect collisionBox);
    void run();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    ~Vision();
private:
    MapParse* _mp;
    sf::RenderWindow* _window; // Delete this when done debugging... and the one passed to the constructor
    sf::VertexArray _light;
    Point _source;
    sf::Color _colour;
    std::vector<Point> _drawPoints;
    float _raylineMax;
    float _fov;
    float _heading;
};
#endif /* !_VISION_HPP */
