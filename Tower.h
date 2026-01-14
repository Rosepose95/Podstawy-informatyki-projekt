#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Bullet.h"
struct TowerStatus {
    int infestation = 0;
    int burn = 0;
    int slow = 0;
    float freeze = 0.f;
    bool destroyed = false;
};


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
    static constexpr int MAX_BURN = 4;

    int burnStacks = 0;
    float baseCooldown = 0.f;
    
    float frozenTimer = 0.f;
    int slowStacks = 0;
    float freezeTimer = 0.f;




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
    
    float getFireRateMultiplier() const;
    // burn
    void addBurn(int stacks);
    int getBurnStacks() const;

    // destroy
    void forceDestroy();

    // movement
    void setPosition(sf::Vector2f pos);
    TowerStatus getStatus() const;
    void setStatus(const TowerStatus& s);



    void setInfestationStacks(int v);
    void setBurnStacks(int v);
    void setDestroyed(bool v);

    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
// slow
void addSlow(int stacks);
int getSlowStacks() const;

// freeze
void freeze(float time);
bool isFrozen() const;

};


#endif






