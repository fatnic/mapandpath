#include <SFML/Graphics.hpp>
#include "mapparse.hpp"
#include "pathfind.hpp"
#include "vision.hpp"
#include "tools/drawtools.hpp"

#include <iostream>

class Player : public sf::RectangleShape
{
public:
    Player()
    {
        setSize(sf::Vector2f(10,10));
        setFillColor(sf::Color::Green);
        setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
    }
};

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    MapParse mp;
    mp.load("assets/large.tmx");

    sf::RenderWindow window(sf::VideoMode(mp.getMapSize().x, mp.getMapSize().y), "GameWindow", sf::Style::Default, settings);

    sf::Shader shader;
    shader.loadFromFile("assets/shader.frag", sf::Shader::Fragment);
    shader.setParameter("opacity", 0.3);

    sf::CircleShape light(4);
    light.setOrigin(light.getRadius(), light.getRadius());
    light.setFillColor(sf::Color::Yellow);
    light.setPosition(mp.getSpawnPoint("light").x, mp.getSpawnPoint("light").y);

    Player player;
    player.setPosition(mp.getSpawnPoint("player").x, mp.getSpawnPoint("player").y);

    Pathfind pf(&mp);

    Vision vision(&mp, &window);
    vision.setColour(sf::Color(255, 255, 255));

    float fov = 60.f;
    vision.setFOV(fov);

    std::vector<Point*> waypoints;
    bool moved{true};

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed)
                if(event.key.code == sf::Keyboard::Escape)
                    window.close();

        }

        int x = sf::Mouse::getPosition(window).x;
        int y = sf::Mouse::getPosition(window).y;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            light.move(-1,0);
            moved = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            light.move(1,0);
            moved = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            light.move(0,-1);
            moved = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            light.move(0,1);
            moved = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            pf.setStart(light.getPosition().x, light.getPosition().y);
            pf.setGoal(player.getPosition().x, player.getPosition().y);
            waypoints.clear();
            waypoints = pf.run();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            fov += 0.2f;
            vision.setFOV(fov);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            fov -= 0.2f;
            vision.setFOV(fov);
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            player.setPosition(x, y);
            moved = true;
        }

        if(moved)
        {
            waypoints.clear();
            moved = false;
        }


        float heading = std::atan2(y - light.getPosition().y, x - light.getPosition().x);
        vision.setHeading(heading);

        window.clear(sf::Color::Red);
        window.draw(mp);

        vision.setSource(Point(light.getPosition().x, light.getPosition().y));

        sf::Color pColour = vision.collision(player) ? sf::Color::Red : sf::Color::Green;
        player.setFillColor(pColour);

        window.draw(vision, &shader);

        Point wp1(light.getPosition().x, light.getPosition().y);
        for(Point*point : waypoints)
        {
            DrawTools::drawLine(wp1, *point, sf::Color::Magenta, &window);
            wp1 = *point;
        }

        window.draw(light);
        window.draw(player);
        window.display();
    }
}
