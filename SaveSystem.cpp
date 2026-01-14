#include "SaveSystem.h"
#include "Game.h"
#include "Map.h"
#include "Enemy.h"
#include "Tower.h"
#include "SaveV3.h"

#include <fstream>
#include <sstream>

namespace SaveSystem {

    // SAVE 
    void save(Game& g, int slot)
    {
        Map* map = g.getMap();
        if (!map) return;

        std::ofstream file("save" + std::to_string(slot) + ".txt");
        if (!file) return;

        file << SAVE_MAGIC << "\n";
        file << "AUTO " << (slot == 0) << "\n";
        file << "MAP " << map->mapId << "\n";

        // --- MAP ---
        file << "SIZE " << map->getWidth() << " " << map->getHeight() << "\n";
        file << "MAPDATA\n";
        for (int y = 0; y < map->getHeight(); ++y) {
            for (int x = 0; x < map->getWidth(); ++x)
                file << map->getTile(x, y);
            file << "\n";
        }
        file << "ENDMAP\n";

        // --- TOWERS ---
        const auto& towers = g.getTowers();
        file << "TOWERS " << towers.size() << "\n";
        for (auto& t : towers) {
            file << t.getPosition().x << " "
                << t.getPosition().y << " "
                << t.getInfestationStacks() << " "
                << t.getBurnStacks() << " "
                << t.isDestroyed() << "\n";
        }


        // --- ENEMIES ---
        const auto& enemies = g.getEnemies();
        file << "ENEMIES " << enemies.size() << "\n";
        for (auto& e : enemies) {
            auto p = e.getPosition();
            file << p.x << " " << p.y << " "
                << e.getHP() << " " << e.getMaxHP() << " "
                << e.getTilePos().x << " " << e.getTilePos().y << " "
                << e.getPrevTile().x << " " << e.getPrevTile().y << " "
                << (int)e.getType() << "\n";
        }

        // --- GAME STATE ---
        int wave, lives, gold, baseHP, enemiesToSpawn;
        float spawnTimer, waveBreakTimer;
        bool waveFlag;

        g.getWaveState(
            wave, lives, gold, baseHP,
            enemiesToSpawn, spawnTimer,
            waveFlag, waveBreakTimer
        );

        file << "GAMESTATE\n";
        file << wave << " " << lives << " " << gold << " " << baseHP << " "
            << enemiesToSpawn << " " << spawnTimer << " "
            << waveFlag << " " << waveBreakTimer << "\n";

        // --- WAVE CONFIG ---
        int enemyHP;
        float speed, spawnDelay;
        bool boss;

        g.getWaveConfig(enemyHP, speed, spawnDelay, boss);

        file << "WAVECONFIG\n";
        file << enemyHP << " " << speed << " "
            << spawnDelay << " " << boss << "\n";

        // --- STATS ---
        float playTime;
        int killed;
        g.getStats(playTime, killed);

        file << "STATS\n";
        file << playTime << " " << killed << "\n";


    }

    // ================= LOAD =================
    bool load(Game& g, int slot)
    {
        std::ifstream file("save" + std::to_string(slot) + ".txt");
        if (!file) return false;

        std::string token;
        file >> token;
        if (token != SAVE_MAGIC) return false;

        // AUTO
        file >> token; // AUTO
        bool isAuto;
        file >> isAuto;

        // MAP
        file >> token; // MAP
        std::string mapId;
        file >> mapId;
        g.loadMapById(mapId);

        // SIZE
        file >> token; // SIZE
        int w, h;
        file >> w >> h;

        // MAPDATA
        file >> token; // MAPDATA
        file.ignore();

        g.getMap()->clearMap();
        for (int y = 0; y < h; ++y) {
            std::string line;
            std::getline(file, line);
            for (int x = 0; x < w; ++x)
                g.getMap()->setTile(x, y, line[x]);
        }

        file >> token; // ENDMAP
        g.getMap()->refreshLogic();
        g.clearForLoad();

        // TOWERS
        file >> token; // TOWERS
        size_t towerCount;
        file >> towerCount;

        for (size_t i = 0; i < towerCount; ++i) {
            float x, y;
            int infest, burn;
            bool destroyed;

            file >> x >> y >> infest >> burn >> destroyed;

            g.addTowerFromSave(x, y);

            // ustaw statusy
            auto& t = const_cast<Tower&>(g.getTowers().back());
            t.setInfestationStacks(infest);
            t.setBurnStacks(burn);
            t.setDestroyed(destroyed);
        }


        // ENEMIES
        file >> token; // ENEMIES
        size_t enemyCount;
        file >> enemyCount;

        for (size_t i = 0; i < enemyCount; ++i) {
            float wx, wy;
            int hp, maxHp, tx, ty, ptx, pty, type;

            file >> wx >> wy >> hp >> maxHp
                >> tx >> ty >> ptx >> pty >> type;

            Enemy e(maxHp, wx, wy, (EnemyType)type);
            e.setMap(g.getMap());
            e.setHP(hp);
            e.restorePath({ tx, ty }, { ptx, pty });
            g.addEnemyFromSave(e);
        }

        // GAMESTATE
        file >> token; // GAMESTATE
        int wave, lives, gold, baseHP, enemiesToSpawn;
        float spawnTimer, waveBreakTimer;
        bool waveFlag;

        file >> wave >> lives >> gold >> baseHP
            >> enemiesToSpawn >> spawnTimer
            >> waveFlag >> waveBreakTimer;

        g.setWaveState(
            wave, lives, gold, baseHP,
            enemiesToSpawn, spawnTimer,
            waveFlag, waveBreakTimer
        );

        // WAVECONFIG
        file >> token; // WAVECONFIG
        int enemyHP;
        float speed, spawnDelay;
        bool boss;

        file >> enemyHP >> speed >> spawnDelay >> boss;
        g.setWaveConfig(enemyHP, speed, spawnDelay, boss);

        // STATS
        file >> token; // STATS
        float playTime;
        int killed;
        file >> playTime >> killed;
        g.setStats(playTime, killed);
        g.onLoadedFromSave();

        std::string token2;
        file >> token2;

        if (token2 == "ADVENTURE") {
            int count;
            file >> count;

            for (int i = 0; i < count; ++i) {
                std::string nodeId;
                int unlocked, completed;
                file >> nodeId >> unlocked >> completed;

                g.getAdventure().setNodeState(
                    nodeId,
                    unlocked != 0,
                    completed != 0
                );
            }
        }

        return true;
    }

    // ================= UTILS =================
    bool exists(int slot)
    {
        std::ifstream f("save" + std::to_string(slot) + ".txt");
        return f.good();
    }
    std::string getDescription(int slot) {
        std::ifstream file("save" + std::to_string(slot) + ".txt");
        if (!file.is_open()) return "EMPTY SLOT";

        std::string header;
        std::getline(file, header);
        if (header != SAVE_MAGIC) return "CORRUPTED SAVE";

        // AUTO
        std::string autoLine;
        std::getline(file, autoLine);
        bool isAuto = (autoLine.back() == '1');

        // MAP
        std::string mapLine;
        std::getline(file, mapLine);
        std::istringstream mapStream(mapLine);
        std::string mapToken, mapId;
        mapStream >> mapToken >> mapId;

        // SIZE
        std::string sizeLine;
        std::getline(file, sizeLine);
        std::istringstream sizeStream(sizeLine);
        std::string sizeToken;
        int width, height;
        sizeStream >> sizeToken >> width >> height;

        // MAPDATA
        std::string token;
        std::getline(file, token); // MAPDATA
        for (int i = 0; i < height; ++i) std::getline(file, token);
        std::getline(file, token); // ENDMAP

        // TOWERS
        std::getline(file, token); // TOWERS X
        int towerCount;
        std::istringstream(token) >> token >> towerCount;
        for (int i = 0; i < towerCount; ++i) std::getline(file, token);

        // ENEMIES
        std::getline(file, token); // ENEMIES X
        int enemyCount;
        std::istringstream(token) >> token >> enemyCount;
        for (int i = 0; i < enemyCount; ++i) std::getline(file, token);

        // GAMESTATE
        std::getline(file, token); // "GAMESTATE"
        std::getline(file, token); // linia z wartościami
        std::istringstream gs(token);
        int wave, lives, gold, baseHP, enemiesToSpawn;
        float spawnTimer, waveBreakTimer;
        int waveFlagInt;
        if (!(gs >> wave >> lives >> gold >> baseHP
            >> enemiesToSpawn >> spawnTimer
            >> waveFlagInt >> waveBreakTimer))
            return "CORRUPTED SAVE";

        if (slot == 0)
            return "[AUTO] Wave " + std::to_string(wave) + " | Gold " + std::to_string(gold);

        return "Wave " + std::to_string(wave) + " | Gold " + std::to_string(gold) + " | Lives " + std::to_string(lives);
    }
}
