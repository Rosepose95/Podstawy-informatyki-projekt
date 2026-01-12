#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::CircleShape shape;
    sf::Vector2f direction; //zmiana zmiennych 
    float speed;
    float lifetime;
    int damage;
    bool dead;

public:
	Bullet(sf::Vector2f start, sf::Vector2f target, int dmg);  //poprawka do konstruktora

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    bool isDead() const;
    void kill(); //usuwanie u¿ytych pocisków i ich pozycja

    sf::Vector2f getPosition() const;
    int getDamage() const;
};

#endif


