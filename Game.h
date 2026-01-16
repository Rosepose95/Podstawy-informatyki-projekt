#ifndef GAME_H
#define GAME_H
#include "WorldMap.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include "AdventureMode.h"
#include "Map.h" // dodanie nagłówka mapy
#include "GameTypes.h"

enum class AdventureLevel {
    Grass = 0,
    Ice = 1,
    Fire = 2
};
struct WarningTile {
    sf::Vector2f pos;
    float timer;
};


/*struct Biome {
    std::string name;
    std::vector<EnemyType> enemies;
    EnemyType boss;
};
*/
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
    bool isAuto;
};



class Game {
private:
    // --- Obiekty gry ---
    std::vector<Enemy> enemies; // lista przeciwników
    std::vector<Tower> towers;  // lista wież
    std::vector<Bullet> bullets; // lista pocisków
    Map* map = nullptr;
    // wskaźnik na mapę
    AdventureMode adventure;
    std::string currentNodeId;
    WorldMap worldMap;
    bool inWorldMap = true;
    int adventureWave = 0;
    int maxAdventureWaves = 10;
    bool bossAlive = false;
    
    static constexpr int MAX_ADVENTURE_WAVES = 3;
    std::vector<WarningTile> warningTiles;

    // --- Statystyki gracza ---
    int playerLives = 20; // życie gracza
    int baseHP;           // zdrowie bazy
    int gold = 0;         // złoto 
    bool gameOver;        // flaga Game Over
    // Adventure state
    void applyIceWave(sf::Vector2f bossPos);
   
    int adventureStage = 0;     // 0–2 (3 levele)
    bool bossSpawned = false;
    bool levelFinished = false;

    std::vector<std::pair<Tower*, sf::Vector2f>> pendingTowerMoves;

   
    // --- Fale ---
    int currentWave = 0;     // aktualna fala
    int enemiesToSpawn = 0;  // ile jeszcze przeciwników do spawnu
    WaveConfig currentWaveConfig;
    float spawnTimer = 0.f;
    float spawnDelay = 0.6f; // co ile sekund spawn
    bool waveJustLoaded = false;
    float enemyHpMultiplier = 1.f;
    float goldMultiplier = 1.f;

    bool isBossWave = false;     // flaga boss wave
    bool waveInProgress = false; // czy fala w toku

    float waveBreakTimer = 0.f;
    const float breakDuration = 3.0f; // przerwa między falami w sekundach
    float totalPlayTime = 0.f;
    int enemiesKilled = 0;
    int adventureBiome = 0; // 0–4

    // --- UI ---
    sf::Font font;          // czcionka
    sf::Text waveText;      // tekst aktualnej fali
    sf::Text enemiesText;   // liczba przeciwników
    sf::Text livesText;     // życie gracza
    sf::Text GameOverText;  // Game Over
    sf::Text NextWaveText;  // komunikat kolejnej fali
    sf::Text autoSaveText;
    sf::Clock autoSaveClock;
    bool showAutoSave = false;
    sf::Sprite autoSaveIcon;
    sf::Texture autoSaveTexture;

    // --- UI dodatkowe z drugiego kodu ---
    sf::Text infoText;      // tekst informacyjny (save/load)
    sf::Clock infoClock;    // zegar infoText
    bool showInfo = false;  // flaga wyświetlania infoText

    // --- Game Over ekran i przyciski ---
    sf::RectangleShape GameOverScreen;
    sf::RectangleShape RestartButton;
    sf::RectangleShape ExitButton;
    sf::Text Restart;
    sf::Text Exit;
    sf::Vector2f startTPos;

    bool showNodeCompleted = false;
    sf::Clock nodeCompletedClock;
    sf::Text nodeCompletedText;
    bool bossDefeatedThisFrame = false;
    Biomes currentBiome = Biomes::Meadow;

    GameMode mode = GameMode::Adventure;
    Difficulty difficulty = Difficulty::Normal;

public:
    Game();
    void startAdventureNode(int nodeId);
    // --- Map ---
    void setMap(Map* m);  // ustawienie mapy i blokady stawiania wież
    void spawnBossForCurrentLevel();
    // --- Dodawanie jednostek ---
    void addEnemy(Enemy enemy);
    void addTower(Tower tower);
    void placeTower(sf::Vector2f position);
    bool canPlaceTower(sf::Vector2f pos) const;
    void spawnIceExplosion(sf::Vector2f pos);

    // --- Fale ---
    void startNextWave();
    void startGame();  // reset gry

    // --- UI ---
    void updateUI();
    void drawUI(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    // --- Obsługa przycisków ---
    void tryRestart(sf::Vector2f mousePos); // restart gry
    void HandleHover(sf::Vector2i mousePos); // hover przycisków
    bool tryExit(sf::Vector2f mousePos);     // wyjście z gry

    // --- Aktualizacja gry ---
    void update(float dt);

    // --- Gettery ---
    int getBaseHP() const;
    bool isGameOver() const;

    void loadWaveFromFile(const std::string& filename);

    // --- Autosave ---
    void autoSave();
    bool autoSaveExists() const;
    std::string getAutoSaveDescription() const;


    // --- Flagi dodatkowe ---
    bool isCustomMap = false; // czy mapa niestandardowa
    void setStartTPos(sf::Vector2f pos);
    void loadMapById(const std::string& id);
    // --- SAVE / LOAD API ---
    void clearForLoad();

    void addTowerFromSave(float x, float y);
    void addEnemyFromSave(const Enemy& e);

    void setWaveState(
        int currentWave,
        int lives,
        int gold,
        int baseHP,
        int enemiesToSpawn,
        float spawnTimer,
        bool waveInProgress,
        float waveBreakTimer
    );

    void setWaveConfig(
        int enemyHP,
        float speed,
        float spawnDelay,
        bool bossWave
    );
    void onMapCompleted();
    void setStats(float playTime, int killed);
    void markLoaded();
    //Gettery
    Map* getMap() const;

    const std::vector<Tower>& getTowers() const;
    const std::vector<Enemy>& getEnemies() const;

    void getWaveState(
        int& wave, int& lives, int& gold, int& baseHP,
        int& enemiesToSpawn, float& spawnTimer,
        bool& waveInProgress, float& waveBreakTimer
    ) const;

    void getWaveConfig(
        int& enemyHP, float& speed,
        float& spawnDelay, bool& bossWave
    ) const;

    void getStats(float& playTime, int& enemiesKilled) const;

    void onLoadedFromSave();

    AdventureMode& getAdventure();
    void enterWorldMap();
    GameMode getMode() const;

    void setCurrentNode(const std::string& id);
    const std::string& getCurrentNode() const;
    void handleWorldMapClick(sf::Vector2f mouseWorldPos);
    void infestTowers(sf::Vector2f pos, float radius, int stacks);
    std::vector<Tower*> getTowersNear(sf::Vector2f pos, float radius);
    void pushTowersNear(sf::Vector2f bossPos);
    void burnTowers(sf::Vector2f pos, float radius, int stacks);
    EnemyType pickEnemyForBiome(Biomes biome, bool isBossWave);
    void slowTowers(sf::Vector2f pos, float radius, int stacks);
    void freezeTowers(sf::Vector2f pos, float radius, float time);

    void setMode(GameMode m);
    void setDifficulty(Difficulty d);
    void advanceBiome();
    bool isInWorldMap() const { return inWorldMap; }
    void applyDifficulty(Difficulty diff);
    void applyMeadowBossAbility();
    bool canMoveTower(int tx, int ty);
    bool canPushTowerWithoutBlockingPath(int tx, int ty, Tower* movingTower);
};

#endif
