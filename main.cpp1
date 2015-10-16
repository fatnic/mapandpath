#include <SFML/Graphics.hpp>
#include "mapparse.hpp"
#include "pathfind.hpp"
#include "tools/drawtools.hpp"

#include <iostream>

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    MapParse mp;
    mp.load("assets/tilemap.tmx");

    Pathfind pf(&mp);

    sf::RenderWindow window(sf::VideoMode(mp.getMapSize().x, mp.getMapSize().y), "GameWindow", sf::Style::Default, settings);

    sf::CircleShape guard(4);
    guard.setOrigin(guard.getRadius(), guard.getRadius());
    guard.setFillColor(sf::Color::Blue);
    guard.setPosition(mp.getSpawnPoint("start").x, mp.getSpawnPoint("start").y);

    sf::CircleShape goal(4);
    goal.setOrigin(goal.getRadius(), goal.getRadius());
    goal.setFillColor(sf::Color::Green);
    goal.setPosition(mp.getSpawnPoint("goal").x, mp.getSpawnPoint("goal").y);

    std::vector<Point*> waypoints;
    bool pf_run = true;

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

        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            int x = sf::Mouse::getPosition(window).x;
            int y = sf::Mouse::getPosition(window).y;
            guard.setPosition(x, y);
            pf_run = true;
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int x = sf::Mouse::getPosition(window).x;
            int y = sf::Mouse::getPosition(window).y;
            goal.setPosition(x, y);
            pf_run = true;

        }

        if(pf_run)
        {
            pf.setStart(guard.getPosition().x, guard.getPosition().y);
            pf.setGoal(goal.getPosition().x, goal.getPosition().y);
            waypoints.clear();
            waypoints = pf.run();
            pf_run = false;
        }

        window.clear(sf::Color::Red);
        mp.draw(&window);

        Point wp1(guard.getPosition().x, guard.getPosition().y);
        for(Point* point : waypoints)
        {
            DrawTools::drawLine(wp1, *point, sf::Color::Green, &window);
            wp1 = *point;
        }

        window.draw(guard);
        window.draw(goal);

        window.display();
    }
}
