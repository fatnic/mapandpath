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

    sf::ConvexShape lightShape;

    Vision vs(&mp, &window);

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

        window.clear(sf::Color::Red);
        mp.draw(&window);

        int x = sf::Mouse::getPosition(window).x;
        int y = sf::Mouse::getPosition(window).y;
        light.setPosition(x, y);
        vs.setSource(Point(x,y));

        lightShape = vs.run();
        window.draw(light);
        window.display();
    }
}
