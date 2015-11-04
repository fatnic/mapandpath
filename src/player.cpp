#include "player.hpp"
#include <structs.hpp>
#include <tools/drawtools.hpp>
#include <tools/tools.hpp>

#include <iostream>

Player::Player()
    : _circle()
    , _speed(80.f)
{
    _circle.setRadius(5);
    _circle.setOrigin(_circle.getRadius(), _circle.getRadius());
    _circle.setFillColor(sf::Color::Transparent);
    _circle.setOutlineColor(sf::Color::White);
    _circle.setOutlineThickness(1.f);
}

void Player::update(float dT)
{
    _dx = 0; _dy = 0;
    _dT = dT;

    bool movingUp    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool movingDown  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool movingLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

    if(movingUp)    moveUp();
    if(movingDown)  moveDown();
    if(movingLeft)  moveLeft();
    if(movingRight) moveRight();

    _heading = std::atan2(_dy, _dx) * 180 / M_PI;

    move(_dx * _speed * _dT, _dy * _speed * _dT);
    _circle.setPosition(getPosition().x, getPosition().y);

}

void Player::moveUp()
{
    if(!wallCollision(Direction::UP)) _dy = -1;
}

void Player::moveDown()
{
    if(!wallCollision(Direction::DOWN)) _dy = 1;
}

void Player::moveLeft()
{
    if(!wallCollision(Direction::LEFT)) _dx = -1;
}

void Player::moveRight()
{
    if(!wallCollision(Direction::RIGHT)) _dx = 1;
}

bool Player::wallCollision(Direction direction)
{
    sf::Vector2f prevPos(getPosition());

    switch(direction)
    {
        case(Direction::UP):
            move(0, -1 * _speed * _dT);
            _circle.setPosition(getPosition().x, getPosition().y);
            if(wallIntersects(getCollisionBox()))
            {
                setPosition(prevPos.x, prevPos.y);
                _circle.setPosition(getPosition().x, getPosition().y);
                return true;
            }
            break;

        case(Direction::DOWN):
            move(0, 1 * _speed * _dT);
            _circle.setPosition(getPosition().x, getPosition().y);
            if(wallIntersects(getCollisionBox()))
            {
                setPosition(prevPos.x, prevPos.y);
                _circle.setPosition(getPosition().x, getPosition().y);
                return true;
            }
            break;

        case(Direction::LEFT):
            move(-1 * _speed * _dT, 0);
            _circle.setPosition(getPosition().x, getPosition().y);
            if(wallIntersects(getCollisionBox()))
            {
                setPosition(prevPos.x, prevPos.y);
                _circle.setPosition(getPosition().x, getPosition().y);
                return true;
            }
            break;

        case(Direction::RIGHT):
            move(1 * _speed * _dT, 0);
            _circle.setPosition(getPosition().x, getPosition().y);
            if(wallIntersects(getCollisionBox()))
            {
                setPosition(prevPos.x, prevPos.y);
                _circle.setPosition(getPosition().x, getPosition().y);
                return true;
            }
            break;

        default:
            return false;
            break;
    }

    return false;
}

bool Player::wallIntersects(sf::FloatRect bbox)
{
    for(Wall* wall : _mp->getWalls())
    {
        if(wall->AABB.intersects(bbox))
            return true;
    }

    return false; 
}

sf::FloatRect Player::getCollisionBox()
{
    return _circle.getGlobalBounds();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    Ray ray;
    ray.start = Point(getPosition().x, getPosition().y);
    ray.end.x = ray.start.x + _circle.getRadius() * std::cos(Tools::deg2rad(_heading));
    ray.end.y = ray.start.y + _circle.getRadius() * std::sin(Tools::deg2rad(_heading));
    DrawTools::drawLine(ray.start, ray.end, sf::Color::White, _window);

    target.draw(_circle, states); 
}
