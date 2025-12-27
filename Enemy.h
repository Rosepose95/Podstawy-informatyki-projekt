#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy {
private:
    int health;
    sf::CircleShape shape;
    int maxHealth;  //życie
public:
    Enemy(int hp, float x, float y);
    float getRadius() const;  //by znać średnicę

    void move(float dx, float dy);
    void takeDamage(int dmg);
    bool isDead() const;
    int getMaxHealth() const;

    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
};

#endif

