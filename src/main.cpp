#include <SFML/Graphics.hpp>
#include "mapparse.hpp"

int main()
{
    MapParse mp;
    mp.load("assets/tilemap.tmx");

    sf::RenderWindow window(sf::VideoMode(mp.getMapSize().x, mp.getMapSize().y), "GameWindow");

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
        window.display();
    }
}
