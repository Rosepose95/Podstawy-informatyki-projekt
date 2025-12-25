#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Bullet.h"

class Tower {
private:
    int damage;
    float range;
    sf::RectangleShape shape;
    float cooldown;
    float timeSinceLastShot;

public:
    Tower(int dmg, float x, float y);
    void updateAttack(Enemy& enemy, float dt,
        std::vector<Bullet>& bullets);
    void draw(sf::RenderWindow& window) const;
};

#endif

