#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include "Enemy.h"
#include "Bullet.h"

class Tower {
private:
    int damage;
    float cooldown;
    float timeSinceLastShot;
    int level;
    float range;
    int type;

    sf::Vector2f pos;

    sf::Sprite sprite; // OK, bo zainicjalizujemy go w konstruktorze

    const std::array<sf::Texture, 3>* textures = nullptr;
    int tileSize = 0;

    void applyType(int newType);

public:
    Tower(int dmg, float x, float y, int Towertype,
        const std::array<sf::Texture, 3>& tex, int tileSz);

    void setType(int newType);
    int getType() const { return type; }

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return pos; }

    void updateAttack(std::vector<Enemy>& enemies, float dt, std::vector<Bullet>& bullets);
    void draw(sf::RenderWindow& window) const;
};

#endif





