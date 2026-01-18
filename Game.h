#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include "Map.h" // dodanie nagłówka mapy
#include <array>

// --- Struktura konfiguracji fali ---
struct WaveConfig {
    int count;       // liczba przeciwników
    int enemyHP;     // zdrowie przeciwników
    float speed;     // prędkość
    float spawnDelay; // odstęp między spawnami
};

// --- Struktura zapisu gry ---
struct SaveInfo {
    int wave;   // numer fali
    int health; // liczba żyć gracza
    int gold;   // złoto (jeśli planowane)
};

class Game {
private:
    //tekstury
    std::array<sf::Texture, 3> towerTextures;
    bool towerTexturesLoaded = false;
    void loadTowerTextures();
    int towerCost(int type) const; // koszt zależny od typu
    sf::RectangleShape TowerTypeIcon; //ikonka upgrade wiezy
    void updateTowerTypeIcon();

    // --- Obiekty gry ---
    std::vector<Enemy> enemies; // lista przeciwników
    std::vector<Tower> towers;  // lista wież
    std::vector<Bullet> bullets; // lista pocisków
    Map* map = nullptr;          // wskaźnik na mapę

    // --- Statystyki gracza ---
    int playerLives = 20; // życie gracza
    int baseHP;           // zdrowie bazy
    int gold = 30;         // złoto (z drugiego kodu)
    bool gameOver;        // flaga Game Over

    // --- Fale ---
    int currentWave = 0;     // aktualna fala
    int enemiesToSpawn = 0;  // ile jeszcze przeciwników do spawnu
    WaveConfig currentWaveConfig;
    float spawnTimer = 0.f;
    float spawnDelay = 0.6f; // co ile sekund spawn

    bool isBossWave = false;     // flaga boss wave
    bool waveInProgress = false; // czy fala w toku

    float waveBreakTimer = 0.f;
    const float breakDuration = 3.0f; // przerwa między falami w sekundach

    // --- UI ---
    sf::Font font;          // czcionka
    sf::Text waveText;      // tekst aktualnej fali
	sf::Text goldText;     // tekst złota
    sf::Text enemiesText;   // liczba przeciwników
    sf::Text livesText;     // życie gracza
    sf::Text GameOverText;  // Game Over
    sf::Text NextWaveText;  // komunikat kolejnej fali

    // --- UI dodatkowe z drugiego kodu ---
    sf::Text infoText;      // tekst informacyjny (save/load)
    sf::Clock infoClock;    // zegar infoText
    bool showInfo = false;  // flaga wyświetlania infoText

    // --- Game Over ekran i przyciski ---
    sf::RectangleShape TowerTypeButton;
    sf::RectangleShape GameOverScreen;
    sf::RectangleShape RestartButton;
    sf::RectangleShape ExitButton;
    sf::Text Restart;
    sf::Text Exit;
    sf::Text TowerTypeText;

    sf::Vector2f startTPos;
public:
    Game();

    int towerUpgradeStep;// Zmienna poziomu wier

    // --- Map ---
    void setMap(Map* m);  // ustawienie mapy i blokady stawiania wież

    // --- Dodawanie jednostek ---
    void addEnemy(Enemy enemy);
    void addTower(Tower tower);
    void placeTower(sf::Vector2f position);
    bool canPlaceTower(sf::Vector2f pos) const;

    // --- Fale ---
    void startNextWave();
    void startGame();  // reset gry

    // --- UI ---
    void updateUI();
    void drawUI(sf::RenderWindow& window) const;
    void draw(sf::RenderWindow& window) const;

    // --- Obsługa przycisków ---
    void tryRestart(sf::Vector2f mousePos); // restart gry
    void HandleHover(sf::Vector2i mousePos); // hover przycisków
    bool tryExit(sf::Vector2f mousePos);     // wyjście z gry
    void tryTowerType(sf::Vector2f mousePos); // przycisk rozoju wierzy 


    // --- Aktualizacja gry ---
    void update(float dt);
    // --- Gettery ---
    int getBaseHP() const;
    bool isGameOver() const;

    // --- Zapis i wczytywanie ---
    void saveGame(int slot);
    void loadGame(int slot);
    bool saveExists(int slot) const;
    SaveInfo getSaveInfo(int slot);

    // --- Autosave ---
    void autoSave();
    bool loadAutoSave();

    // --- Flagi dodatkowe ---
    bool isCustomMap = false; // czy mapa niestandardowa


    bool isClickOnUI(sf::Vector2f mousePos) const; 
    const sf::Texture& getTowerTexture(int type) const;

    void setStartTPos(sf::Vector2f pos);
};

#endif


