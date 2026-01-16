#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
enum class BulletEffect {
    Normal,
    Ice
};



class Bullet {
public:
    Bullet(sf::Vector2f start, sf::Vector2f target, int dmg, BulletEffect eff);  //poprawka do konstruktora

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    bool isDead() const;
    void kill(); //usuwanie użytych pocisków i ich pozycja

    sf::Vector2f getPosition() const;
    int getDamage() const;
    BulletEffect getEffect() const { 
        return effect; }
    float getRadius() const { return shape.getRadius(); }

private:
    sf::CircleShape shape;
    sf::Vector2f direction; //zmiana zmiennych 
    float speed;
    float lifetime;
    int damage;
    bool dead;
    BulletEffect effect = BulletEffect::Normal;

    

};

#endif


