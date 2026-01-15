#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <functional>
#include <string>

enum class EnemyType {
    Normal,
    Fast,
    MeadowBoss,
    Infestor,// stary "tank" z infestation

    Crusher,     // FIRE tank – push wież
    Flame,
    FireBoss,
    Fireball,

    IceShard,     // szybki, lekki slow
    FrostWalker, // tank + aura slow
    IceBoss

};

struct EnemyStatus {
    int hp;
    EnemyType type;
    bool rage;
};
enum class BossAbility {
    Slow,
    Freeze,
    Push
};

class Enemy {
public:
    Enemy(int h, float startX, float startY, EnemyType t = EnemyType::Normal); //zmiana metod, wektorów i typów
    bool reachedGoal() const;
    void setMap(const Map* m);
    void update(float dt);
    void takeDamage(int dmg);
    bool isDead() const;

    using InfestCallback = std::function<void(sf::Vector2f, float, int)>;
    void setInfestCallback(InfestCallback cb);

    using BossAbilityCallback = std::function<void(sf::Vector2f)>;
    BossAbilityCallback bossCallback;
    void setBossCallback(BossAbilityCallback cb);

    sf::Vector2f getPosition() const;
    float getRadius() const;
    void setSpeed(float s) { speed = s; }
    void recalculatePath();

    // Ustawienie pozycji kafelka i sąsiednich kafelków po wczytaniu
    void forceWorldPosition(sf::Vector2f pos);

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
    void applyWaveSpeed(float multiplier);
    void restorePath(sf::Vector2i tile, sf::Vector2i prev) {
        tilePos = tile;
        prevTile = prev;
        nextTile = findNextTile();
        shape.setPosition(tileCenter(tile));
    }
    

    bool isBoss() const { return isBossEnemy; }
    bool isRaging() const { return rage; }
    using BurnCallback = std::function<void(sf::Vector2f, float, int)>;
    void setBurnCallback(BurnCallback cb);
    void applySlow(float factor, float duration);
    void applyFreeze(float duration);
    bool isFrozen() const { return frozen; }

    
    BurnCallback burnCallback;
    EnemyStatus getStatus() const;
    void setStatus(const EnemyStatus& s);
    void setColor(sf::Color c) {
        shape.setFillColor(c);
    }
    std::function<void(sf::Vector2f, BossAbility)> bossCallback;

private:
    int health;  //życie
    int maxHealth;
    float bossAbilityTimer = 0.f;

    sf::CircleShape shape;
    float infestationTimer = 0.f;

    float baseSpeed = 0.f;

    const Map* map; //do pathingu 
    sf::Vector2i tilePos;
    sf::Vector2i nextTile;
    sf::Vector2i prevTile;
    int bossPhase = 0; // 0–3


    float speed;

    sf::Vector2i findNextTile() const;
    sf::Vector2f tileCenter(sf::Vector2i tile) const;
    EnemyType type;

    bool warningActive = false;
    sf::Clock warningClock;
    sf::Vector2f warningPos;

    bool isBossEnemy = false;
    bool rage = false;
    InfestCallback infestCallback;

    float bossSkillTimer = 0.f;
    sf::Color baseColor;


    float burnTimer = 0.f;
    // --- ICE STATUS ---
    float slowTimer = 0.f;
    float slowFactor = 1.f;   // np. 0.5 = 50% speed

    float freezeTimer = 0.f;
    bool frozen = false;
    float specialTimer = 0.f;

};


#endif





