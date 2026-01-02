#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include "Map.h" //dodanie nagłówka
struct WaveConfig {  //kostrukcja do fal
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
    Map* map = nullptr;  //zmiany by działała mapa, życia, fale i spawner
    int playerLives = 20;
    int baseHP;
    bool gameOver;

    // WAVES
    int currentWave = 0;
    int enemiesToSpawn = 0;
    WaveConfig currentWaveConfig;
    float spawnTimer = 0.f;
    float spawnDelay = 0.6f; // co ile sekund enemy

    // UI
    sf::Font font;
    sf::Text waveText;
    sf::Text enemiesText;
    sf::Text livesText;
    sf::Text GameOverText; //
    sf::Text NextWaveText; //

    float waveBreakTimer = 0.f;
    const float breakDuration = 3.0f; // 3 sekundy przerwy między falami aby wyswietlic tekst

    bool isBossWave = false;
    bool waveInProgress = false;

    sf::RectangleShape GameOverScreen;//
    sf::RectangleShape RestartButton;//
    sf::RectangleShape ExitButton;
    sf::Text Restart;//
    sf::Text Exit;


public:
    Game();
    void setMap(Map* m); //ustawianie mapy i blokady stawiania wież i rozpoczęcie nowej fali
    void addEnemy(Enemy enemy);
    void startNextWave();
    void updateUI();
    void startGame();
    void addTower(Tower tower);
    void placeTower(sf::Vector2f position);
    bool canPlaceTower(sf::Vector2f pos) const;

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    void drawUI(sf::RenderWindow& window) const;  //do ui

    void tryRestart(sf::Vector2f mousePos); //
    void HandleHover(sf::Vector2i mousePos);//
    bool tryExit(sf::Vector2f mousePos);//

    int getBaseHP() const;
    bool isGameOver() const;

    bool isCustomMap = false;
};

#endif

