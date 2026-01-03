#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Bullet.h"

class Tower {
private:
    int damage;
    float cooldown;
    float timeSinceLastShot;
    int level;
    float range;

    sf::RectangleShape shape;

public:
    Tower(int dmg, float x, float y);

	void updateAttack(std::vector<Enemy>& enemies, float dt, std::vector<Bullet>& bullets);  //zmiana na referencje
    void upgrade();

    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
};


#endif






