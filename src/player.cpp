#include "player.hpp"
#include <structs.hpp>
#include <tools/drawtools.hpp>
#include <tools/tools.hpp>

#include <iostream>

Player::Player()
    : _circle()
    , _speed(80.f)
{
    _circle.setRadius(4);
    _circle.setOrigin(_circle.getRadius(), _circle.getRadius());
    _circle.setFillColor(sf::Color::Green);
}

void Player::update(float dT)
{
    _dx = 0; _dy = 0;

    sf::Vector2f prevPos(getPosition().x, getPosition().y);
    bool collision = false;

    bool movingUp    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool movingDown  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool movingLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

    if(movingUp)    moveUp();
    if(movingDown)  moveDown();
    if(movingLeft)  moveLeft();
    if(movingRight) moveRight();

    _heading = std::atan2(_dy, _dx) * 180 / M_PI;

    move(_dx * _speed * dT, _dy * _speed * dT);
    _circle.setPosition(getPosition().x , getPosition().y);

    for(Wall* wall : _mp->getWalls())
    {
        if(wall->AABB.intersects(getCollisionBox()))
        {
            collision = true;
            break;
        }
    }

    if(collision)
    {
        setPosition(prevPos.x, prevPos.y);
        _circle.setPosition(getPosition().x , getPosition().y);
    }

}

void Player::moveUp()
{
    _dy = -1;
}

void Player::moveDown()
{
    _dy = 1;
}

void Player::moveLeft()
{
    _dx = -1;  
}

void Player::moveRight()
{
    _dx = 1;
}

sf::FloatRect Player::getCollisionBox()
{
    return _circle.getGlobalBounds();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

    Ray ray;
    ray.start = Point(getPosition().x, getPosition().y);
    ray.end.x = ray.start.x + 10 * std::cos(Tools::deg2rad(_heading));
    ray.end.y = ray.start.y + 10 * std::sin(Tools::deg2rad(_heading));
    DrawTools::drawLine(ray.start, ray.end, sf::Color::White, _window);

    target.draw(_circle, states); 
}
