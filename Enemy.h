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
    int health;  //¿ycie
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
    void recalculatePath();

    // Ustawienie pozycji kafelka i s¹siednich kafelków po wczytaniu

    void draw(sf::RenderWindow& window) const;
    int getHP() const { return health; }
    EnemyType getType() const { return type; }
    int getLifeDamage() const;
    int getMaxHP() const { return maxHealth; }
    void setHP(int hp) { health = hp; }
    void setMaxHP(int hp) { maxHealth = hp; }
    sf::Vector2i getTilePos() const { return tilePos; }
    sf::Vector2i getPrevTile() const { return prevTile; }
    sf::Vector2i getNextTile() const { return nextTile; }

    void restorePath(sf::Vector2i tile, sf::Vector2i prev) {
        tilePos = tile;
        prevTile = prev;
        nextTile = findNextTile(); 
        shape.setPosition(tileCenter(tile));
    }


};

#endif