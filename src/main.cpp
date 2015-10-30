#include <SFML/Graphics.hpp>
#include "mapparse.hpp"
#include "vision.hpp"
#include "player.hpp"

#include <iostream>

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    MapParse mp;
    mp.load("assets/large.tmx");

    sf::RenderWindow window(sf::VideoMode(mp.getMapSize().x, mp.getMapSize().y), "GameWindow", sf::Style::Default, settings);

    sf::CircleShape light(4);
    light.setOrigin(light.getRadius(), light.getRadius());
    light.setFillColor(sf::Color::Yellow);
    light.setPosition(mp.getSpawnPoint("light").x, mp.getSpawnPoint("light").y);

    Vision vision(&mp, &window);

    sf::Shader shader;
    shader.loadFromFile("assets/shader.frag", sf::Shader::Fragment);
    shader.setParameter("color", sf::Vector3f(1.0, 1.0, 1.0));
    shader.setParameter("opacity", 0.4f);

    float fov = 60.f;
    vision.setFOV(fov);

    Player player;
    player.setPosition(mp.getSpawnPoint("player").x, mp.getSpawnPoint("player").y);
    player._window = &window;
    player._mp = &mp;

    sf::Clock clock;

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

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            bool blocked{false};

            for(Wall* wall : mp.getWalls())
            {
                if(wall->AABB.contains(x, y))
                    blocked = true;
            }

            if(!blocked)
                light.setPosition(x, y);
        }

        float delta = clock.restart().asSeconds();

        float heading = std::atan2(y - light.getPosition().y, x - light.getPosition().x);
        vision.setHeading(heading);

        window.clear(sf::Color::Red);
        window.draw(mp);

        vision.setSource(Point(light.getPosition().x, light.getPosition().y));

        if(vision.collision(player.getCollisionBox()))
            shader.setParameter("color", sf::Vector3f(1.0,0.0,0.0));
        else
            shader.setParameter("color", sf::Vector3f(1.0,1.0,1.0));

        player.update(delta);
        window.draw(player);

        window.draw(vision, &shader);
        window.draw(light);
        window.display();
    }
}
