#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy {
private:
    int health;
    sf::CircleShape shape;
    int maxHealth;  //życie
    const Map* map; //do pathing
    sf::Vector2i tilePos;
    sf::Vector2i nextTile;
    sf::Vector2i prevTile;

    float speed;

    sf::Vector2i findNextTile() const;
    sf::Vector2f tileCenter(sf::Vector2i tile) const;
public:
    Enemy(int h, float startX, float startY); //zmiana metod i wektorów
    bool reachedGoal() const;
    void setMap(const Map* m);
    void update(float dt);

    void takeDamage(int dmg);
    bool isDead() const;

    sf::Vector2f getPosition() const;
    float getRadius() const;
    void setSpeed(float s) { speed = s; }

    void draw(sf::RenderWindow& window) const
};

#endif


