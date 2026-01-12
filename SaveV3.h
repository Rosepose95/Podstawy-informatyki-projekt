#pragma once
#include <vector>
#include <string>
#include <SFML/System.hpp>

static constexpr const char* SAVE_MAGIC = "TD_SAVE_V3";

// ---------------- ENEMY ----------------
struct SaveEnemy {
    float worldX;
    float worldY;

    int hp;
    int maxHp;

    int tileX;
    int tileY;
    int prevTileX;
    int prevTileY;

    int type;
};

// ---------------- TOWER ----------------
struct SaveTower {
    float x;
    float y;
};

// ---------------- GAME STATE ----------------
struct SaveGameV3 {
    // meta
    bool isAuto;
    std::string mapId;

    // map
    int mapW;
    int mapH;
    std::vector<std::string> mapTiles;

    // entities
    std::vector<SaveTower> towers;
    std::vector<SaveEnemy> enemies;

    // wave state
    int currentWave;
    int playerLives;
    int gold;
    int baseHP;

    int enemiesToSpawn;
    float spawnTimer;
    bool waveInProgress;
    float waveBreakTimer;

    // wave config
    int enemyHP;
    float waveSpeed;
    float spawnDelay;
    bool isBossWave;

    // stats (V3+)
    float totalPlayTime;
    int enemiesKilled;
};

