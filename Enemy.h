#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <array>
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
    sf::Sprite sprite{s_textures[0]};

    const Map* map = nullptr; //do pathingu 
    sf::Vector2i tilePos;
    sf::Vector2i nextTile;
    sf::Vector2i prevTile;

    float speed;

    static std::array<sf::Texture, 4> s_textures;
    static bool s_texturesLoaded;
    static void loadTextures();

    sf::Vector2i findNextTile() const;
    sf::Vector2f tileCenter(sf::Vector2i tile) const;
    EnemyType type;
    void applyType(EnemyType t, int h);


    static constexpr float ROT_OFFSET_DEG = -90.f;

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
    int getHP() const { return health; }
    EnemyType getType() const { return type; }
    int getLifeDamage() const;

};

#endif



