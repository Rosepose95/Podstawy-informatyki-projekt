#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include "Map.h" //dodanie nagłówka

struct WaveConfig { //kostrukcja do fal
    int count;
    int enemyHP;
    float speed;
    float spawnDelay;
};

class Game {
private:
    std::vector<Enemy> enemies;
    std::vector<Tower> towers;
    std::vector<Bullet> bullets;
    Map* map = nullptr; //zmiany by działała mapa, życia, fale i spawner
    int playerLives = 20;
    int baseHP;
    bool gameOver;
    // WAVES
    int currentWave = 0;
    int enemiesToSpawn = 0;
    int enemiesAlive = 0;
    WaveConfig currentWaveConfig;

    float spawnTimer = 0.f;
    float spawnDelay = 0.6f; // co ile sekund enemy

     // UI
    sf::Font font;
    sf::Text livesText;
    sf::Text waveText;
    sf::Text enemiesText;

public:
    Game();
    void setMap(Map* m);  //ustawianie mapy i blokady stawiania wież i rozpoczęcie nowej fali
    void addEnemy(Enemy enemy);
    void startNextWave();

    void addTower(Tower tower);
    void placeTower(sf::Vector2f position);
    bool canPlaceTower(sf::Vector2f pos) const;
   

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    void drawUI(sf::RenderWindow& window) const; //do ui

    int getBaseHP() const;
    bool isGameOver() const;
};

#endif




