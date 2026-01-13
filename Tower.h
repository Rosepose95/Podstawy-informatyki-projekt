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
    
    // infestation
	int infestationStacks = 0;
	static constexpr int MAX_INFESTATION = 10;
    int baseDamage = 0;
  
    bool destroyed = false;
    sf::RectangleShape shape;

public:
    Tower(int dmg, float x, float y);
	void updateAttack(std::vector<Enemy>& enemies, float dt, std::vector<Bullet>& bullets);  //zmiana na referencje
    void upgrade();

    void addInfestation(int stacks);
	bool isDestroyed() const;
	int getInfestationStacks() const;
	float getDamageMultiplier() const;


    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
};


#endif



