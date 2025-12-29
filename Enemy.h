#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Map.h"
enum class EnemyType {  //typy enemy
    Normal,
    Fast,
    Tank,
    Boss
};

class Enemy {
private:
    int health;  //życie
    int maxHealth;

    sf::CircleShape shape;

	const Map* map; //do pathingu 
    sf::Vector2i tilePos;
    sf::Vector2i nextTile;
    sf::Vector2i prevTile;

    float speed;

    sf::Vector2i findNextTile() const;
    sf::Vector2f tileCenter(sf::Vector2i tile) const;
    EnemyType type;


public:
    Enemy(int h, float startX, float startY, EnemyType t = EnemyType::Normal); //zmiana metod, wektorów i typów
    bool reachedGoal() const;
    void setMap(const Map* m);
    void update(float dt);

    void takeDamage(int dmg);
    bool isDead() const;

    sf::Vector2f getPosition() const;
    float getRadius() const;
    void setSpeed(float s) { speed = s; }

    void draw(sf::RenderWindow& window) const;
};

#endif



