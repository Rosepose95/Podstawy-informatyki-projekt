#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Enemy.h"
#include "Tower.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode({ 800, 600 }),
        "Tower Defense SFML 3"
    );

    Game game;

    game.addEnemy(Enemy(100, 0.f, 100.f));
    game.addEnemy(Enemy(120, 0.f, 200.f));


    game.addTower(Tower(20, 400.f, 150.f));
    game.addTower(Tower(30, 600.f, 300.f));

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

                    game.placeTower(worldPos);
                }
            }
        }


        float dt = clock.restart().asSeconds();
        game.update(dt);

        window.clear(sf::Color::White);
        game.draw(window);
        window.display();
    }

    return 0;
}
