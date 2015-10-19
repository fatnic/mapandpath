#include <SFML/Graphics.hpp>
#include "mapparse.hpp"
#include "vision.hpp"
#include "tools/drawtools.hpp"

#include <iostream>

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    MapParse mp;
    mp.load("assets/lightmap.tmx");

    sf::RenderWindow window(sf::VideoMode(mp.getMapSize().x, mp.getMapSize().y), "GameWindow", sf::Style::Default, settings);

    sf::CircleShape light(4);
    light.setOrigin(light.getRadius(), light.getRadius());
    light.setFillColor(sf::Color::Yellow);
    light.setPosition(mp.getSpawnPoint("light").x, mp.getSpawnPoint("light").y);

    sf::VertexArray lightShape;

    Vision vs(&mp, &window);
    vs.setColour(sf::Color(0, 0, 255, 60));

    float fov = 60.f;

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

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            light.move(-1,0);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            light.move(1,0);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            light.move(0,-1);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            light.move(0,1);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            fov += 0.2f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            fov -= 0.2f;

        vs.setFOV(fov);

        int x = sf::Mouse::getPosition(window).x;
        int y = sf::Mouse::getPosition(window).y;

        float heading = std::atan2(y - light.getPosition().y, x - light.getPosition().x);
        vs.setHeading(heading);

        window.clear(sf::Color::Red);
        mp.draw(&window);

        vs.setSource(Point(light.getPosition().x, light.getPosition().y));

        lightShape = vs.run();

        window.draw(lightShape);
        window.draw(light);
        window.display();
    }
}
