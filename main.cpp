#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Enemy.h"
#include "Tower.h"
#include "Map.h"


int main() {
    sf::RenderWindow window(
        sf::VideoMode({ 1240,840 }),
        "Tower Defense SFML 3"
    );

    Game game;
    Map map;    //

    game.addEnemy(Enemy(200, 0.f, 80.f));
    game.addEnemy(Enemy(300, 0.f, 720.f));  //pozmienialem pozycje zeby nie byly pomiedzy bloczkami 
    game.addEnemy(Enemy(200, 0.f, 80.f));
    game.addEnemy(Enemy(200, 1040.f, 800.f));


    game.addTower(Tower(20, 560.f, 440.f));
    //game.addTower(Tower(30, 600.f, 300.f));

    sf::Clock clock;

    while (window.isOpen()) {

        while (auto ev = window.pollEvent()) {

            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouse =
                ev->getIf<sf::Event::MouseButtonPressed>()) {

                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2f worldPos =
                        window.mapPixelToCoords(mouse->position);

                    //zaokroglam aby wieze nie mozna bylo postawic pomiedzy kratkami
                    int tileSize = map.tileSize;
                    float gridX = (int)worldPos.x / tileSize * tileSize;
                    float gridY = (int)worldPos.y / tileSize * tileSize;

                    game.placeTower({ gridX, gridY });

                }
            }
        }


        float dt = clock.restart().asSeconds();
        game.update(dt);

        window.clear(sf::Color::White);

        map.draw(window);//

        game.draw(window);
        window.display();
    }

    return 0;
}

