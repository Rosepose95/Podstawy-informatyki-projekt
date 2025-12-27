#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include "Map.h" //dodanie nagłówka

class Game {
private:
    std::vector<Enemy> enemies;
    std::vector<Tower> towers;
    std::vector<Bullet> bullets;
    Map* map; //dodanie mapy by się wyświetlała

    int baseHP;
    bool gameOver;

public:
    Game();

    void addEnemy(const Enemy& e);
    void addTower(const Tower& t);
    void placeTower(sf::Vector2f position);
    void setMap(Map* m);  //ustawianie mapy

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    int getBaseHP() const;
    bool isGameOver() const;
};

#endif



