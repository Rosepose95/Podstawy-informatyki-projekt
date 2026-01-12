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
    // status infestation
    int infestationStacks = 0;
    float baseDamage = 0.f;   // zapamiêtujemy oryginalny damage
    bool destroyed = false;
    sf::RectangleShape shape;

public:
    Tower(int dmg, float x, float y);
    void addInfestation(int stacks = 1);
    float getDamageMultiplier() const;
    bool isDestroyedByInfestation() const;
	void updateAttack(std::vector<Enemy>& enemies, float dt, std::vector<Bullet>& bullets);  //zmiana na referencje
    void upgrade();
    bool isDestroyed() const { return destroyed; }

    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
};


#endif

