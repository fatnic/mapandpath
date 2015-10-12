#include <SFML/Graphics.hpp>
#include "mapparse.hpp"
#include "pathfind.hpp"
#include "tools/drawtools.hpp"

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    MapParse mp;
    mp.load("assets/tilemap.tmx");

    Pathfind pf(&mp);

    sf::RenderWindow window(sf::VideoMode(mp.getMapSize().x, mp.getMapSize().y), "GameWindow", sf::Style::Default, settings);

    sf::CircleShape guard(8);
    guard.setOrigin(guard.getRadius(), guard.getRadius());
    guard.setFillColor(sf::Color::Blue);
    guard.setPosition(40,40);

    sf::CircleShape goal(8);
    goal.setOrigin(goal.getRadius(), goal.getRadius());
    goal.setFillColor(sf::Color::Green);


    std::vector<Point*> waypoints;
    bool goalSet = false;

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

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int x = sf::Mouse::getPosition(window).x;
            int y = sf::Mouse::getPosition(window).y;
            goalSet = true;
            goal.setPosition(x, y);

            pf.setStart(guard.getPosition().x, guard.getPosition().y);
            pf.setGoal(x, y);
            waypoints.clear();
            waypoints = pf.run();
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

        if(goalSet)
            window.draw(goal);

        window.display();
    }
}
