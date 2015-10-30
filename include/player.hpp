#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <mapparse.hpp>

class Player : public sf::Drawable, public sf::Transformable
{
public:
    enum Direction { UP, DOWN, LEFT, RIGHT };

    Player();
    void update(float dT);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    sf::FloatRect getCollisionBox();
    sf::RenderWindow* _window;
    MapParse* _mp;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::CircleShape _circle;
    float _speed;
    float _dx, _dy;
    float _heading;
};

#endif /* !_PLAYER_HPP */
