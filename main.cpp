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
    game.setMap(&map);
    game.startNextWave(); //nowa fala

    int ts = map.tileSize;
   game.addEnemy(
    Enemy(
        200,
        0 * ts + ts / 2.f,
        2 * ts + ts / 2.f
    )
);


// TOWER (na trawie) 
game.addTower(
    Tower(
        20,
        14 * ts + ts / 2.f,
        11 * ts + ts / 2.f
    )
);

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
                 float tileX = static_cast<int>(worldPos.x) / tileSize;
                 float tileY = static_cast<int>(worldPos.y) / tileSize;
               //by było wyśrodkowane
                 float centerX = tileX * tileSize + tileSize / 2.f;
                 float centerY = tileY * tileSize + tileSize / 2.f;

                 game.placeTower({ centerX, centerY });
             }
         }
     }

   

        float dt = clock.restart().asSeconds();
        game.update(dt);

        window.clear(sf::Color::White);
        map.draw(window);//

        game.draw(window);
        game.drawUI(window); //dodatek ui
        window.display();
    }

    return 0;
}



