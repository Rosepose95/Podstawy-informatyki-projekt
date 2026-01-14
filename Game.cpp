#include "Game.h"
#include "Enemy.h"
#include "Tower.h"
#include "Map.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include "SaveV3.h"
#include "SaveSystem.h"
#include "AdventureMode.h"
#include "WorldMap.h"

Game::Game()
    :autoSaveIcon(autoSaveTexture)
    , font()
    , waveText(font)
    , enemiesText(font)
    , livesText(font)
    , GameOverText(font)
    , NextWaveText(font)
    , playerLives(20)
    , baseHP(100)
    , gameOver(false)
    , Restart(font)
    , Exit(font)
    , infoText(font) // dodatkowy tekst informacyjny
    , autoSaveText(font)
    , nodeCompletedText(font)

{
    // --- Wczytanie czcionki ---
    if (!font.openFromFile("assets/ArialMT.ttf")) {
        throw std::runtime_error("Nie można wczytać czcionki");
    }

    // --- Ustawienia czcionek i kolorów ---
    waveText.setCharacterSize(20);
    enemiesText.setCharacterSize(20);
    livesText.setCharacterSize(20);
    GameOverText.setCharacterSize(80); // duży tekst Game Over
    NextWaveText.setCharacterSize(60);
    Restart.setCharacterSize(30);
    infoText.setCharacterSize(24); // infoText z drugiego kodu

    waveText.setFillColor(sf::Color::Black);
    enemiesText.setFillColor(sf::Color::Black);
    livesText.setFillColor(sf::Color::Black);
    GameOverText.setFillColor(sf::Color::Red);
    NextWaveText.setFillColor(sf::Color::Green);
    Restart.setFillColor(sf::Color::White);
    Exit.setFillColor(sf::Color::White);
    infoText.setFillColor(sf::Color::Green);

    // --- Pozycje tekstów na ekranie ---
    waveText.setPosition({ 10.f, 20.f });
    enemiesText.setPosition({ 10.f, 40.f });
    livesText.setPosition({ 10.f, 55.f });
    NextWaveText.setPosition({ 280.f, 420.f });
    infoText.setPosition({ 450.f, 50.f }); // infoText z drugiego kodu

    // --- Style czcionek ---
    GameOverText.setStyle(sf::Text::Bold | sf::Text::Italic);
    NextWaveText.setStyle(sf::Text::Bold | sf::Text::Italic);
    Restart.setStyle(sf::Text::Bold);
    Exit.setStyle(sf::Text::Bold);

    // --- Napisy przycisków ---
    GameOverText.setString("GAME OVER");
    Restart.setString("Restart");
    Exit.setString("Exit");

    // --- Ekran Game Over ---
    GameOverScreen.setSize({ 1240.f, 840.f });
    GameOverScreen.setFillColor(sf::Color(0, 0, 0, 200));

    // --- Guzik Restart ---
    RestartButton.setSize({ 200.f, 60.f });
    RestartButton.setFillColor(sf::Color::Blue);
    RestartButton.setOutlineThickness(3.f);
    RestartButton.setOutlineColor(sf::Color::Black);
    RestartButton.setOrigin({ RestartButton.getSize().x / 2.f, RestartButton.getSize().y / 2.f });
    RestartButton.setPosition({ 1240.f / 2.f, 840.f / 2.f });

    // --- Guzik Exit ---
    ExitButton.setSize({ 200.f, 60.f });
    ExitButton.setFillColor(sf::Color::Blue);
    ExitButton.setOutlineThickness(3.f);
    ExitButton.setOutlineColor(sf::Color::Black);
    ExitButton.setOrigin({ ExitButton.getSize().x / 2.f, ExitButton.getSize().y / 2.f });
    ExitButton.setPosition({ 1240.f / 2.f, 840.f * 0.65f });

    // --- Teksty ---
    auto goBounds = GameOverText.getLocalBounds();
    GameOverText.setOrigin({ goBounds.position.x + goBounds.size.x / 2.f, goBounds.position.y + goBounds.size.y / 2.f });
    GameOverText.setPosition({ 1240.f / 2.f, 840.f * 0.25f });

    auto rBounds = Restart.getLocalBounds();
    Restart.setOrigin({ rBounds.position.x + rBounds.size.x / 2.f, rBounds.position.y + rBounds.size.y / 2.f });
    Restart.setPosition(RestartButton.getPosition());

    auto eBounds = Exit.getLocalBounds();
    Exit.setOrigin({ eBounds.position.x + eBounds.size.x / 2.f, eBounds.position.y + eBounds.size.y / 2.f });
    Exit.setPosition(ExitButton.getPosition());

    autoSaveText.setFont(font);
    autoSaveText.setString("AUTOSAVED");
    autoSaveText.setCharacterSize(28);
    autoSaveText.setFillColor(sf::Color(0, 200, 0, 0));
    autoSaveIcon.setPosition({ 500.f, 792.f });
    autoSaveText.setPosition({ 530.f, 780.f });

    // --- Wczytanie tekstury ikonki zapisu ---
    if (!autoSaveTexture.loadFromFile("assets/save1.png"))
        throw std::runtime_error("Nie można wczytać save1.png");

    autoSaveIcon.setTexture(autoSaveTexture);
    auto bounds = autoSaveIcon.getLocalBounds();
    autoSaveIcon.setOrigin({
        bounds.size.x / 2.f,
        bounds.size.y / 2.f
        });

    // trochę wyżej niż tekst
    autoSaveIcon.setScale({ 1.f, 1.f });       // dopasuj wielkość
    // to samo co autoSaveText
    /*std::vector<Biome> biomes = {
    { "Meadow", { Bee, Ladybug, Beetle }, BeetleBoss },
    { "Fire", { Fireball, Flame, Tank }, FireBoss },
    { "Forest", { Witch, Teleporter, Wizard }, IllusionBoss },
    { "Water", { Neon, Anchor }, WaterBoss },
    { "Ice", { Skater, Snowball, Snowman }, IceBoss }
    };
    */
    adventure.init();

}

// --- Dodawanie przeciwników ---
void Game::addEnemy(Enemy enemy) {
    enemy.setMap(map); // bardzo ważne by się poruszał po mapie
    enemies.push_back(enemy);
}

// --- Dodawanie wież ---
void Game::addTower(Tower tower) {
    towers.push_back(tower);
}

// --- Umieszczanie wieży na mapie ---
void Game::placeTower(sf::Vector2f position) {
    if (!map) return;

    int gridX = static_cast<int>(position.x / map->tileSize);
    int gridY = static_cast<int>(position.y / map->tileSize);
    if (gridX < 0 || gridY < 0 || gridX >= map->getWidth() || gridY >= map->getHeight()) return;
    if (map->getTile(gridX, gridY) != '.') return;

    for (const auto& t : towers) {
        sf::Vector2f pos = t.getPosition();
        int tx = static_cast<int>(pos.x / map->tileSize);
        int ty = static_cast<int>(pos.y / map->tileSize);
        if (tx == gridX && ty == gridY) return;
    }

    float tsF = static_cast<float>(map->tileSize);
    towers.emplace_back(20, gridX * tsF + tsF / 2.f, gridY * tsF + tsF / 2.f);
}
//dla stawiania wiezy
void Game::setStartTPos(sf::Vector2f pos) {
    startTPos = pos;
}
// --- Start gry ---
void Game::startGame() {
    enemies.clear();
    bullets.clear();
    towers.clear();
    currentWave = 0;
    enemiesToSpawn = 0;
    waveInProgress = false;
    isBossWave = false;
    playerLives = 20;
    gameOver = false;
    spawnTimer = 0.f;
    waveBreakTimer = 0.f;

    waveJustLoaded = false;
    inWorldMap = false;
    bossDefeatedThisFrame = false;

    // ustawienie slidera fali poza ekranem
    NextWaveText.setString("Get ready for wave 1");
    NextWaveText.setPosition({ -600.f, 420.f });

    // dodanie wieży startowej jeśli nie niestandardowa mapa
    if (!isCustomMap && map) {
        int ts = map->tileSize;
        addTower(Tower(20, 14 * ts + ts / 2.f, 11 * ts + ts / 2.f));
    }
    else {
        addTower(Tower(20, startTPos.x, startTPos.y));
    }
    startNextWave();

}


// --- Rozpoczęcie kolejnej fali ---
void Game::startNextWave()
{
    currentWave++;
    waveJustLoaded = false;

    currentBiome = adventure.getBiomeForNode(currentNodeId);

    currentWaveConfig.enemyHP = 50 + currentWave * 10;
    currentWaveConfig.speed = 1.0f + currentWave * 0.03f;

    spawnDelay = 0.8f;
    spawnTimer = 0.f;

    if (mode == GameMode::Adventure)
        isBossWave = adventure.isBossNode(currentNodeId);
    else
        isBossWave = (currentWave % 5 == 0);

    enemiesToSpawn = isBossWave ? 1 : 5 + currentWave;
    waveInProgress = true;
}

// --- Aktualizacja gry ---
void Game::update(float dt) {
    if (gameOver) return;
    totalPlayTime += dt;


    if (inWorldMap) {
        return;
    }

    if (waveJustLoaded) {
        waveJustLoaded = false;
        return;
    }

    // zakończenie fali
    if (waveInProgress && enemies.empty() && enemiesToSpawn == 0) {

        waveInProgress = false;
        waveBreakTimer = 0.f;

        autoSave();
    }

    // przerwa między falami
    if (!waveInProgress && enemies.empty() && enemiesToSpawn == 0) {
        if (waveBreakTimer == 0.f) {
            NextWaveText.setPosition({ -600.f, 420.f });
            NextWaveText.setString("Get ready for wave " + std::to_string(currentWave + 1));
        }
        waveBreakTimer += dt;
        NextWaveText.move({ 600.f * dt, 0.f });
        if (waveBreakTimer >= breakDuration) {
            startNextWave();
            waveBreakTimer = 0.f;
        }
    }


    // --- Spawn przeciwników ---
    if (enemiesToSpawn > 0 && !waveJustLoaded) {
        spawnTimer += dt;
        if (spawnTimer >= spawnDelay) {
            spawnTimer = 0.f;
            int ts = map->tileSize;

            Biomes biome = currentBiome;

            EnemyType type = pickEnemyForBiome(biome, isBossWave);

// fire biome extra spice
            if (biome == Biomes::Fire && !isBossWave) {
                int r = rand() % 6;
                if (r == 0) type = EnemyType::Fireball;
                    if (currentWave >= 6 && rand() % 8 == 0)
                        type = EnemyType::Crusher;
}



            

            auto& spawns = map->spawnPoints;
            if (spawns.empty()) {
                std::cout << "NO SPAWN POINTS\n";
                return;
            }

            if (spawns.empty()) return;
            sf::Vector2i spawnTile = spawns[rand() % spawns.size()];
            float spawnX = static_cast<float>(spawnTile.x * ts + ts / 2);
            float spawnY = static_cast<float>(spawnTile.y * ts + ts / 2);

            enemies.emplace_back(currentWaveConfig.enemyHP, spawnX, spawnY, type);

            Enemy& e = enemies.back();
            e.setMap(map);
            e.applyWaveSpeed(currentWaveConfig.speed);

            // --- INFESTATION (tylko Infestor / MeadowBoss) ---
            if (e.getType() == EnemyType::Infestor ||
                e.getType() == EnemyType::MeadowBoss)
            {
                e.setInfestCallback(
                [this](sf::Vector2f pos, float radius, int stacks) {
                    infestTowers(pos, radius, stacks);
        }
    );
}

            

            // --- FIRE BIOME ---
            if (e.getType() == EnemyType::FireBoss ||
                e.getType() == EnemyType::Flame ||
                e.getType() == EnemyType::Fireball ||
                e.getType() == EnemyType::Crusher)
    {
                e.setBurnCallback(
                [this](sf::Vector2f pos, float radius, int stacks) {
                    burnTowers(pos, radius, stacks);
        }
    );
}
if (e.getType() == EnemyType::FireBoss) {
    e.setBossCallback(
        [this](sf::Vector2f pos) {
            pushTowersNear(pos);
        }
    );
}
else if (e.getType() == EnemyType::IceBoss) {
    e.setBossCallback(
        [this](sf::Vector2f pos) {
            slowTowers(pos, 120.f, 2);
            freezeTowers(pos, 100.f, 1.5f);
            pushTowersNear(pos);
        }
    );
}
else if (e.getType() == EnemyType::Crusher) {
    e.setBossCallback(
        [this](sf::Vector2f pos) {
            pushTowersNear(pos);
        }
    );
}


}
sf::Color biomeColor;

switch (currentBiome) {
case Biomes::Meadow:
    biomeColor = sf::Color(80, 160, 80);
    break;
case Biomes::Fire:
    biomeColor = sf::Color(200, 80, 40);
    break;
case Biomes::Ice:
    biomeColor = sf::Color(150, 200, 255);
    break;
default:
    biomeColor = sf::Color::White;
}

e.setColor(biomeColor);

            
            


if (!e.isBoss() && e.getType() == EnemyType::Normal)
    e.setColor(biomeColor);

            enemiesToSpawn--;
        }
    }

    // --- Sprawdzenie Game Over ---
    if (playerLives <= 0) gameOver = true;

    // --- Ruch przeciwników ---
    for (auto it = enemies.begin(); it != enemies.end();) {
        it->update(dt);
       
        if (it->reachedGoal()) {
            playerLives -= it->getLifeDamage();
            it = enemies.erase(it);
        }
        else if (it->isDead()) {

            if (it->getType() == EnemyType::FireBoss) {
                sf::Vector2f pos = it->getPosition();

                //  eksplozja burn
                burnTowers(pos, 120.f, 3);

                //  spawn Flame
                for (int i = 0; i < 4; ++i) {
                    Enemy e(40, pos.x, pos.y, EnemyType::Flame);
                    e.setMap(map);
                    e.setBurnCallback(
                        [this](sf::Vector2f p, float r, int s) {
                            burnTowers(p, r, s);
                        }
                    );
                    enemies.push_back(e);
                }
            }

            if (isBossWave)
                bossDefeatedThisFrame = true;

            it = enemies.erase(it);
        }

        else ++it;
    }

    // --- Atak wież ---
    for (auto& t : towers) t.updateAttack(enemies, dt, bullets);

    // --- Aktualizacja pocisków ---
    for (auto& b : bullets) b.update(dt);

    // --- Kolizje bullet <-> enemy ---
    for (auto& b : bullets) {
        for (auto& e : enemies) {
            if (e.isDead()) continue;
            sf::Vector2f bp = b.getPosition();
            sf::Vector2f ep = e.getPosition();
            float dx = bp.x - ep.x;
            float dy = bp.y - ep.y;
            if (dx * dx + dy * dy <= e.getRadius() * e.getRadius()) {
                e.takeDamage(b.getDamage());
                b.kill();
                break;
            }
        }
    }

    // --- Usuwanie martwych pocisków ---
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return b.isDead(); }), bullets.end());
    towers.erase(
        std::remove_if(
            towers.begin(),
            towers.end(),
            [](const Tower& t) { return t.isDestroyed(); }
        ),
        towers.end()
    );


    updateUI();
    if (bossDefeatedThisFrame) {
        bossDefeatedThisFrame = false;
        // tylko info, bez world map
        showInfo = true;
        infoText.setString("BOSS DEFEATED!");
        infoClock.restart();
    }


}

// --- Rysowanie gry ---
void Game::draw(sf::RenderWindow& window) const {
    for (const auto& t : towers) t.draw(window);
    for (const auto& e : enemies) e.draw(window);
    for (const auto& b : bullets) b.draw(window);
    if (inWorldMap) {
        worldMap.draw(window, adventure);
        return;
    }

    if (!waveInProgress && enemies.empty() && enemiesToSpawn == 0)
        window.draw(NextWaveText);

    if (gameOver) {
        window.draw(GameOverScreen);
        window.draw(GameOverText);
        window.draw(RestartButton);
        window.draw(Restart);
        window.draw(ExitButton);
        window.draw(Exit);
    }
}

// --- Rysowanie UI ---
void Game::drawUI(sf::RenderWindow& window) {
    window.draw(waveText);
    window.draw(enemiesText);
    if (showNodeCompleted) {
        if (nodeCompletedClock.getElapsedTime().asSeconds() < 2.f)
            window.draw(nodeCompletedText);
        else
            showNodeCompleted = false;
    }

    if (showInfo && infoClock.getElapsedTime().asSeconds() < 2.f)
        window.draw(infoText);

    for (int i = 0; i < playerLives; ++i) {
        sf::CircleShape life(6.f);
        life.setFillColor(sf::Color::Red);
        life.setPosition({ 10.f + i * 15.f, 10.f });
        window.draw(life);
    }

    // --- AUTO-SAVE ---
    if (showAutoSave) {
        float t = autoSaveClock.getElapsedTime().asSeconds();
        if (t < 2.f) {
            int alpha = 255;
            if (t < 0.5f) alpha = static_cast<int>(255 * (t / 0.5f));
            else if (t > 1.5f) alpha = static_cast<int>(255 * (1.f - (t - 1.5f) / 0.5f));

            autoSaveText.setFillColor(sf::Color(0, 200, 0, alpha));
            autoSaveIcon.setColor(sf::Color(255, 255, 255, alpha));

            autoSaveIcon.setPosition({
                autoSaveText.getPosition().x - 40.f,
                autoSaveText.getPosition().y - 4.f
                });

            window.draw(autoSaveText);
            window.draw(autoSaveIcon);

            autoSaveIcon.setColor(sf::Color(255, 255, 255, 255));

        }
        else {
            showAutoSave = false;
        }
    }
}


// --- Aktualizacja UI ---
void Game::updateUI() {
    waveText.setString("Wave: " + std::to_string(currentWave));
    enemiesText.setString("Enemies: " + std::to_string(enemies.size() + enemiesToSpawn));
}

// --- Map ---
void Game::setMap(Map* m) { map = m; }

bool Game::canPlaceTower(sf::Vector2f pos) const {
    if (!map) return false;
    int tx = pos.x / map->tileSize;
    int ty = pos.y / map->tileSize;
    return map->getTile(tx, ty) == '.';
}

// --- Obsługa przycisków ---
void Game::tryRestart(sf::Vector2f mousePos) {
    if (gameOver && RestartButton.getGlobalBounds().contains(mousePos))
        startGame();
}

bool Game::tryExit(sf::Vector2f mousePos) {
    return gameOver && ExitButton.getGlobalBounds().contains(mousePos);
}

// --- Gettery ---
int Game::getBaseHP() const { return baseHP; }
bool Game::isGameOver() const { return gameOver; }

void Game::HandleHover(sf::Vector2i mousepos) {
    sf::Vector2f mousePosF((float)mousepos.x, (float)mousepos.y);
    if (!gameOver) return;

    RestartButton.setFillColor(RestartButton.getGlobalBounds().contains(mousePosF) ? sf::Color::Cyan : sf::Color::Blue);
    ExitButton.setFillColor(ExitButton.getGlobalBounds().contains(mousePosF) ? sf::Color::Cyan : sf::Color::Blue);
}

void Game::autoSave() {
    SaveSystem::save(*this, 0);
    showAutoSave = true;
    autoSaveClock.restart();
}

bool Game::autoSaveExists() const {
    return SaveSystem::exists(0);
}

std::string Game::getAutoSaveDescription() const {
    return SaveSystem::getDescription(0);
}

void Game::loadMapById(const std::string& id)
{
    if (!map) return;

    if (id == "default")
        map->loadMap();
    else if (id == "custom")
        ; // mapa z save
    else
        map->loadMap();

    map->refreshLogic();
}

void Game::clearForLoad() {
    enemies.clear();
    towers.clear();
    bullets.clear();
}

void Game::addTowerFromSave(float x, float y) {
    towers.emplace_back(20, x, y);
}

void Game::addEnemyFromSave(const Enemy& e) {
    enemies.push_back(e);
}

void Game::setWaveState(
    int cw, int lives, int g, int bhp,
    int ets, float st, bool wip, float wbt
) {
    currentWave = cw;
    playerLives = lives;
    gold = g;
    baseHP = bhp;
    enemiesToSpawn = ets;
    spawnTimer = st;
    waveInProgress = wip;
    waveBreakTimer = wbt;
}

void Game::setWaveConfig(
    int hp, float spd, float delay, bool boss
) {
    currentWaveConfig.enemyHP = hp;
    currentWaveConfig.speed = spd;
    spawnDelay = delay;
    isBossWave = boss;
}

void Game::setStats(float time, int killed) {
    totalPlayTime = time;
    enemiesKilled = killed;
}

void Game::markLoaded() {
    waveJustLoaded = true;
    gameOver = false;
}
// Getter
Map* Game::getMap() const { return map; }
const std::vector<Tower>& Game::getTowers() const { return towers; }
const std::vector<Enemy>& Game::getEnemies() const { return enemies; }
void Game::onLoadedFromSave()
{
    waveJustLoaded = true;
    gameOver = false;
    inWorldMap = false;


}
// Game.cpp

void Game::getWaveState(
    int& wave, int& lives, int& gold, int& baseHP,
    int& enemiesToSpawn, float& spawnTimer,
    bool& waveInProgress, float& waveBreakTimer
) const {
    wave = currentWave;
    lives = playerLives;
    gold = this->gold;
    baseHP = this->baseHP;
    enemiesToSpawn = this->enemiesToSpawn;
    spawnTimer = this->spawnTimer;
    waveInProgress = this->waveInProgress;
    waveBreakTimer = this->waveBreakTimer;
}

void Game::getWaveConfig(
    int& enemyHP, float& speed,
    float& spawnDelay, bool& bossWave
) const {
    enemyHP = currentWaveConfig.enemyHP;
    speed = currentWaveConfig.speed;
    spawnDelay = this->spawnDelay;
    bossWave = this->isBossWave;
}

void Game::getStats(float& playTime, int& enemiesKilled) const {
    playTime = this->totalPlayTime;
    enemiesKilled = this->enemiesKilled;
}
AdventureMode& Game::getAdventure() {
    return adventure;
}

void Game::setCurrentNode(const std::string& id) {
    currentNodeId = id;
}

const std::string& Game::getCurrentNode() const {
    return currentNodeId;
}
void Game::onMapCompleted()
{
    adventure.completeNode(currentNodeId);
    int reward = adventure.getRewardGold(currentNodeId);
    gold += reward;

    showNodeCompleted = true;
    nodeCompletedClock.restart();
    nodeCompletedText.setString("NODE COMPLETED!");
    waveInProgress = false;
    enemies.clear();
    bullets.clear();

    std::string next = adventure.getNextNode(currentNodeId);
    if (!next.empty())
        adventure.setNodeState(next, true, false);

    autoSave();
    inWorldMap = true;
}

void Game::handleWorldMapClick(sf::Vector2f mouseWorldPos)
{
    if (!inWorldMap)
        return;

    std::string clicked = worldMap.getNodeAt(mouseWorldPos);

    if (clicked.empty())
        return;

    if (!adventure.isUnlocked(clicked))
        return;

    setCurrentNode(clicked);
    inWorldMap = false;

    loadMapById(clicked);
    startGame();
}
void Game::startAdventureNode(int nodeId)
{
    currentNodeId = nodeId;
    inWorldMap = false;
    waveJustLoaded = false;
    startNextWave();
}
std::vector<Tower*> Game::getTowersNear(sf::Vector2f pos, float radius) {
    std::vector<Tower*> result;
    float r2 = radius * radius;

    for (auto& t : towers) {
        sf::Vector2f diff = t.getPosition() - pos;
        float d2 = diff.x * diff.x + diff.y * diff.y;
        if (d2 <= r2)
            result.push_back(&t);
    }

    return result;
}
void Game::infestTowers(sf::Vector2f pos, float radius, int stacks) {
    float r2 = radius * radius;

    for (auto& t : towers) {
        sf::Vector2f diff = t.getPosition() - pos;
        float dist2 = diff.x * diff.x + diff.y * diff.y;

        if (dist2 <= r2) {
            t.addInfestation(stacks);
        }
    }
}


void Game::pushTowersNear(sf::Vector2f bossPos)
{
    int ts = map->tileSize;

    for (auto& t : towers) {
        sf::Vector2f tp = t.getPosition();
        sf::Vector2f diff = tp - bossPos;
        float dist2 = diff.x * diff.x + diff.y * diff.y;

        if (dist2 <= (ts * 2.5f) * (ts * 2.5f)) {

            int tx = static_cast<int>(tp.x) / ts;
            int ty = static_cast<int>(tp.y) / ts;

            // prawo
            if (tx + 1 < map->getWidth() &&
                map->getTile(tx + 1, ty) == '.') {

                t.setPosition({
                    (tx + 1) * ts + ts / 2.f,
                    ty * ts + ts / 2.f
                    });
                return;
            }

            // lewo
            if (tx - 1 >= 0 &&
                map->getTile(tx - 1, ty) == '.') {

                t.setPosition({
                    (tx - 1) * ts + ts / 2.f,
                    ty * ts + ts / 2.f
                    });
                return;
            }
        }
    }
}


void Game::burnTowers(sf::Vector2f pos, float radius, int stacks) {
    float r2 = radius * radius;

    for (auto& t : towers) {
        sf::Vector2f diff = t.getPosition() - pos;
        if (diff.x * diff.x + diff.y * diff.y <= r2)
            t.addBurn(stacks);
    }
}
void Game::slowTowers(sf::Vector2f pos, float radius, int stacks) {
    float r2 = radius * radius;

    for (auto& t : towers) {
        sf::Vector2f d = t.getPosition() - pos;
        if (d.x*d.x + d.y*d.y <= r2)
            t.addSlow(stacks);
    }
}
void Game::freezeTowers(sf::Vector2f pos, float radius, float time) {
    float r2 = radius * radius;

    for (auto& t : towers) {
        sf::Vector2f d = t.getPosition() - pos;
        if (d.x*d.x + d.y*d.y <= r2)
            t.freeze(time);
    }
}

EnemyType Game::pickEnemyForBiome(Biomes biome, bool isBossWave) {
    if (isBossWave) {
        switch (biome) {
        case Biomes::Fire:
            return EnemyType::FireBoss;
        case Biomes::Meadow:
            return EnemyType::MeadowBoss;
        case Biomes::Ice:
            return EnemyType::IceBoss;

        default:
            return EnemyType::MeadowBoss;
        }
    }

    switch (biome) {
    case Biomes::Fire: {
        int r = rand() % 5;
        if (r == 0) return EnemyType::Fast;
        if (r == 1) return EnemyType::Flame;
        if (r == 2) return EnemyType::Crusher;
        return EnemyType::Normal;
    }
    case Biomes::Meadow: {
        int r = rand() % 3;
        if (r == 0) return EnemyType::Fast;
        if (r == 1) return EnemyType::Infestor;
        return EnemyType::Normal;
    case Biomes::Ice: {
    int r = rand() % 4;
    if (r == 0) return EnemyType::IceShard;
    if (r == 1) return EnemyType::FrostWalker;
    return EnemyType::Normal;
}

    }
    default:
        return EnemyType::Normal;
    }
}







