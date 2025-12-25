#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy {
private:
    int health;
    sf::CircleShape shape;

public:
    Enemy(int hp, float x, float y);

    void move(float dx, float dy);
    void takeDamage(int dmg);
    bool isDead() const;

    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
};

#endif
