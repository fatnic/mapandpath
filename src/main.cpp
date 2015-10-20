#include <SFML/Graphics.hpp>
#include "mapparse.hpp"
#include "pathfind.hpp"
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
    Pathfind pf(&mp);

    vs.setColour(sf::Color(255, 255, 255, 60));

    float fov = 60.f;
    vs.setFOV(fov);

    sf::CircleShape player(4);
    player.setOrigin(player.getRadius(), player.getRadius());
    player.setFillColor(sf::Color::Green);
    player.setPosition(mp.getSpawnPoint("player").x, mp.getSpawnPoint("player").y);

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
            vs.setFOV(fov);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            fov -= 0.2f;
            vs.setFOV(fov);
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
        vs.setHeading(heading);

        window.clear(sf::Color::Red);
        mp.draw(&window);

        vs.setSource(Point(light.getPosition().x, light.getPosition().y));
        lightShape = vs.run();

        window.draw(lightShape);

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
