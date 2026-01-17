#include "Game.h"
#include "Enemy.h"
#include "Tower.h"
#include "Map.h"
#include "Bullet.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

Game::Game()
    : font()
    , waveText(font)
    , goldText(font)
    , enemiesText(font)
    , livesText(font)
    , GameOverText(font)
    , NextWaveText(font)
    , playerLives(20)
    , baseHP(100)
    , gameOver(false)
    , Restart(font)
    , Exit(font)
    , TowerTypeText(font)
    , infoText(font) // dodatkowy tekst informacyjny
{   
    // --- Wczytanie czcionki ---
    if (!font.openFromFile("assets/ArialMT.ttf")) {
        throw std::runtime_error("Nie można wczytać czcionki");
    }
    
    // --- Ustawienia czcionek i kolorów ---
    waveText.setCharacterSize(20);
    goldText.setCharacterSize(30);
    enemiesText.setCharacterSize(20);
    livesText.setCharacterSize(20);
    GameOverText.setCharacterSize(80); // duży tekst Game Over
    NextWaveText.setCharacterSize(60);
    Restart.setCharacterSize(30); //
    Exit.setCharacterSize(30);
    TowerTypeText.setCharacterSize(20);
    infoText.setCharacterSize(24); // infoText z drugiego kodu

    waveText.setFillColor(sf::Color::Black);
    goldText.setFillColor(sf::Color::Black);
    enemiesText.setFillColor(sf::Color::Black);
    livesText.setFillColor(sf::Color::Black);
    GameOverText.setFillColor(sf::Color::Red);
    NextWaveText.setFillColor(sf::Color::Green);
    Restart.setFillColor(sf::Color::White);
    Exit.setFillColor(sf::Color::White);
    TowerTypeText.setFillColor(sf::Color::Black);
    infoText.setFillColor(sf::Color::Green); // infoText kolor

    // --- Pozycje tekstów na ekranie ---
    waveText.setPosition({ 10.f, 20.f });
    goldText.setPosition({ 1040.f, 10.f });
    enemiesText.setPosition({ 10.f, 40.f });
    livesText.setPosition({ 10.f, 55.f });
    NextWaveText.setPosition({ 280.f, 420.f });
    infoText.setPosition({ 450.f, 50.f }); // infoText z drugiego kodu
    TowerTypeText.setPosition({ 1180.f, 100.f }); 

    // --- Style czcionek ---
    GameOverText.setStyle(sf::Text::Bold | sf::Text::Italic);
    NextWaveText.setStyle(sf::Text::Bold | sf::Text::Italic);
    Restart.setStyle(sf::Text::Bold);
    Exit.setStyle(sf::Text::Bold);
    TowerTypeText.setStyle(sf::Text::Bold);

    // --- Napisy przycisków ---
    GameOverText.setString("GAME OVER");
    Restart.setString("Restart");
    Exit.setString("Exit");
    TowerTypeText.setString("UP");

    

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

	// --- Guzik Tower Type ---
	TowerTypeButton.setSize({ 75.f, 150.f });
	TowerTypeButton.setFillColor(sf::Color::Blue);
	TowerTypeButton.setOutlineThickness(3.f);
	TowerTypeButton.setOutlineColor(sf::Color::Black);
	TowerTypeButton.setOrigin({ TowerTypeButton.getSize().x / 2.f, TowerTypeButton.getSize().y / 2.f });
	TowerTypeButton.setPosition({ 1230.f - TowerTypeButton.getSize().x / 2.f, 100.f + TowerTypeButton.getSize().y / 2.f });
    TowerTypeIcon.setSize({ TowerTypeButton.getSize().x * 0.9f, TowerTypeButton.getSize().y * 0.9f });
    TowerTypeIcon.setOrigin({ TowerTypeIcon.getSize().x / 2.f, TowerTypeIcon.getSize().y / 2.f });
    TowerTypeIcon.setPosition(TowerTypeButton.getPosition());

    loadTowerTextures();
    updateTowerTypeIcon();

    // --- Guzik Exit ---
    ExitButton.setSize({ 200.f, 60.f });
    ExitButton.setFillColor(sf::Color::Blue);
    ExitButton.setOutlineThickness(3.f);
    ExitButton.setOutlineColor(sf::Color::Black);
    ExitButton.setOrigin({ ExitButton.getSize().x / 2.f, ExitButton.getSize().y / 2.f });
    ExitButton.setPosition({ 1240.f / 2.f, 840.f * 0.65f });

    // --- Teksty ---
    auto goBounds = GameOverText.getLocalBounds();
    GameOverText.setOrigin({ goBounds.position.x + goBounds.size.x / 2.f,
                             goBounds.position.y + goBounds.size.y / 2.f });
    GameOverText.setPosition({ 1240.f / 2.f, 840.f * 0.25f });

    auto rBounds = Restart.getLocalBounds();
    Restart.setOrigin({ rBounds.position.x + rBounds.size.x / 2.f,
                        rBounds.position.y + rBounds.size.y / 2.f });
    Restart.setPosition(RestartButton.getPosition());

    auto eBounds = Exit.getLocalBounds();
    Exit.setOrigin({ eBounds.position.x + eBounds.size.x / 2.f,
                     eBounds.position.y + eBounds.size.y / 2.f });
    Exit.setPosition(ExitButton.getPosition());
}
    
    //wczytanie tekstur wież
    void Game::loadTowerTextures() {
        if (towerTexturesLoaded) return;

        if (!towerTextures[0].loadFromFile("assets/Tower_1.png"))  throw std::runtime_error("No Tower_1.png");
        if (!towerTextures[1].loadFromFile("assets/Tower_2.png"))  throw std::runtime_error("No Tower_2.png");
        if (!towerTextures[2].loadFromFile("assets/Tower_3.png"))  throw std::runtime_error("No Tower_3.png");

        towerTexturesLoaded = true;
    }
    //zmiana ikony buttonu po kliknieciu
    void Game::updateTowerTypeIcon()
    {
        loadTowerTextures(); // upewnia się że tekstury są wczytane

        int t = towerUpgradeStep;
        if (t < 0) t = 0;
        if (t > 2) t = 2;

        TowerTypeIcon.setTexture(&towerTextures[t], true);

        TowerTypeIcon.setSize({ TowerTypeButton.getSize().x * 0.9f,
                                TowerTypeButton.getSize().y * 0.9f });

        TowerTypeIcon.setOrigin({ TowerTypeIcon.getSize().x / 2.f,
                                  TowerTypeIcon.getSize().y / 2.f });

        TowerTypeIcon.setPosition(TowerTypeButton.getPosition());
    }

    //koszt postawienia wiezy
    int Game::towerCost(int type) const {

        static const int cost[3] = { 5, 20, 50 };
        if (type < 0) type = 0;
        if (type > 2) type = 2;
        return cost[type];
    }

// --- Dodawanie przeciwników ---
void Game::addEnemy(Enemy enemy) {
    enemy.setMap(map);   // bardzo ważne by się poruszał po mapie
    enemies.push_back(enemy);
}

// --- Dodawanie wież ---
void Game::addTower(Tower tower) {
    towers.push_back(tower);
}

// --- Umieszczanie wieży na mapie ---
void Game::placeTower(sf::Vector2f position) {
    if (!map) return;
    loadTowerTextures();
    const int selectedType = towerUpgradeStep;
    const int ts = map->tileSize;
    float tsF = static_cast<float>(ts);

    int gridX = static_cast<int>(position.x / map->tileSize);
    int gridY = static_cast<int>(position.y / map->tileSize);

    if (gridX < 0 || gridY < 0 || gridX >= map->getWidth() || gridY >= map->getHeight())
        return;

    if (!map->isBuildable(gridX, gridY))
        return;
    

   

    auto towerBaseTile = [&](const Tower& t) {
        int tx = static_cast<int>(t.getPosition().x / ts);
        int tyBase = static_cast<int>(t.getPosition().y / ts) - 1; 
        return sf::Vector2i(tx, tyBase);
        };


    // jeśli na tym kafelku jest już wieża, nadpisuje ją i zwraca różnice
    for (auto& t : towers) {
        auto bt = towerBaseTile(t);
        if (bt.x == gridX && std::abs(bt.y - gridY) <= 1) {
            int currentType = t.getType();

            int cost = towerCost(selectedType) - towerCost(currentType);
            if (cost < 0) cost = 0; // brak zwrotów przy downgrade

            if (gold < cost) {
                infoText.setString("Not enough gold!");
                infoClock.restart();
                showInfo = true;
                return;
            }

            gold -= cost;
            t.setType(selectedType);
            return;
        }
    }

   

    // jeśli kafelek pusty, stawia nową wieżę, jeśli za mało golda daje komunikat 
    int cost = towerCost(selectedType);
    if (gold < cost) {
        infoText.setString("Not enough gold!");
        infoClock.restart();
        showInfo = true;
        return;
    }


   
    float posX = gridX * tsF + tsF / 2.f;
    float posY = (gridY + 1) * tsF;  

    towers.emplace_back(20, posX, posY, selectedType, towerTextures, map->tileSize);
    gold-=cost; // odejmowanie złota za postawienie wieży
}

//dla stawiania wiezy
void Game::setStartTPos(sf::Vector2f pos) {
    startTPos = pos;
}

// --- Start gry ---
void Game::startGame() {
    loadTowerTextures();
    // RESETOWANIE ZASOBÓW I SKLEPU 
    gold = 30;               
    towerUpgradeStep = 0;    
    updateTowerTypeIcon();
    // RESETOWANIE WYGLĄDU PRZYCISKU ULEPSZENIA
    TowerTypeButton.setFillColor(sf::Color::Blue);
    TowerTypeText.setString("UP");

    // CZYSZCZENIE OBIEKTÓW NA MAPIE
    enemies.clear();
    bullets.clear();
    towers.clear();

    // RESETOWANIE STATUSU FALI I ŻYCIA
    currentWave = 0;
    enemiesToSpawn = 0;
    waveInProgress = false;
    isBossWave = false;
    playerLives = 20;
    gameOver = false;
    spawnTimer = 0.f;
    waveBreakTimer = 0.f;

    // Ustawienie slidera fali poza ekranem
    NextWaveText.setString("Get ready for wave 1");
    NextWaveText.setPosition({ -600.f, 420.f });

    // DODANIE WIEŻY STARTOWEJ 
    if (!isCustomMap && map) {
        int ts = map->tileSize;
        addTower(Tower(20,14 * ts + ts / 2.f,(11 + 1) * ts,        // dół kafelka
            towerUpgradeStep, towerTextures, map->tileSize));
    }
    else {
        addTower(Tower(20, startTPos.x, startTPos.y,
            towerUpgradeStep, towerTextures, map->tileSize));
    }

    updateUI();
}

// --- Rozpoczęcie kolejnej fali ---
void Game::startNextWave() {
    currentWave++;
    waveInProgress = true;

    // co 5 fala = boss
    isBossWave = (currentWave % 5 == 0);
    if (isBossWave) autoSave(); // autosave co 5 falę

    currentWaveConfig.count = isBossWave ? 1 : (3 + currentWave);
    currentWaveConfig.enemyHP = isBossWave ? 600 : (30 + currentWave * 30);
    currentWaveConfig.speed = isBossWave ? 60.f : (100.f + currentWave * 2.f);

    enemiesToSpawn = currentWaveConfig.count;
    spawnDelay = isBossWave ? 1.2f : std::max(0.25f, 0.7f - currentWave * 0.03f);
    spawnTimer = 0.f;

    autoSave(); // autosave na początku każdej fali
}

// --- Aktualizacja gry ---
void Game::update(float dt) {
    if (gameOver) return;

    // zakończenie fali
    if (waveInProgress && enemies.empty() && enemiesToSpawn == 0) {
        waveInProgress = false;
        waveBreakTimer = 0.f;
    }

    // przerwa między falami
    if (!waveInProgress && enemies.empty() && enemiesToSpawn == 0) {
        if (waveBreakTimer == 0.f) {
            NextWaveText.setPosition({ -600.f, 420.f });
            NextWaveText.setString("Get ready for wave " + std::to_string(currentWave + 1));
        }
        waveBreakTimer += dt;

        // animacja przesuwania tekstu
        NextWaveText.move({ 600.f * dt, 0.f });

        // start nowej fali po przerwie
        if (waveBreakTimer >= breakDuration) {
            startNextWave();
            waveBreakTimer = 0.f;
        }
    }

    // --- Spawn przeciwników ---
    if (enemiesToSpawn > 0) {
        spawnTimer += dt;
        if (spawnTimer >= spawnDelay) {
            spawnTimer = 0.f;

            int ts = map->tileSize;
            EnemyType type = EnemyType::Normal;

            if (isBossWave) type = EnemyType::Boss;
            else {
                if (currentWave >= 3 && rand() % 3 == 0) type = EnemyType::Fast;
                if (currentWave >= 5 && rand() % 5 == 0) type = EnemyType::Tank;
            }

            auto& spawns = map->spawnPoints;
            sf::Vector2i spawnTile = spawns[rand() % spawns.size()];

            float spawnX = static_cast<float>(spawnTile.x * ts + ts / 2);
            float spawnY = static_cast<float>(spawnTile.y * ts + ts / 2);

            enemies.emplace_back(currentWaveConfig.enemyHP, spawnX, spawnY, type);
            enemies.back().setMap(map);
            enemies.back().setSpeed(currentWaveConfig.speed);

            enemiesToSpawn--;
        }
    }

    // --- Sprawdzenie Game Over ---
    if (playerLives <= 0) gameOver = true;

    // --- Ruch przeciwników ---
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        it->update(dt);
        if (it->reachedGoal()) {
            playerLives -= it->getLifeDamage();
            it = enemies.erase(it);
        }
        else if (it->isDead()) {
            it = enemies.erase(it);
            gold+= 9; // nagroda za zabicie przeciwnika
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
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.isDead(); }), bullets.end());

    updateUI();
}

// --- Rysowanie gry ---
void Game::draw(sf::RenderWindow& window) const {
    for (const auto& t : towers) t.draw(window);
    for (const auto& e : enemies) e.draw(window);
    for (const auto& b : bullets) b.draw(window);

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
void Game::drawUI(sf::RenderWindow& window) const {
    window.draw(waveText);
    window.draw(enemiesText);
    window.draw(goldText); // złoto
	window.draw(TowerTypeButton); // przycisk wyboru typu wieży
    window.draw(TowerTypeIcon);
    window.draw(TowerTypeText);


    if (showInfo && infoClock.getElapsedTime().asSeconds() < 2.f)
        window.draw(infoText); // wyświetlanie komunikatu z drugiego kodu

    for (int i = 0; i < playerLives; ++i) {
        sf::CircleShape life(6.f);
        life.setFillColor(sf::Color::Red);
        life.setPosition({ 10.f + i * 15.f, 10.f });
        window.draw(life);
    }
}

// --- Aktualizacja UI ---
void Game::updateUI() {
    waveText.setString("Wave: " + std::to_string(currentWave));
    enemiesText.setString("Enemies: " + std::to_string(enemies.size() + enemiesToSpawn));
    goldText.setString("gold: " + std::to_string(gold));
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
    if (gameOver && RestartButton.getGlobalBounds().contains(mousePos)) {
        startGame();
    }
}
bool Game::tryExit(sf::Vector2f mousePos) {
    return gameOver && ExitButton.getGlobalBounds().contains(mousePos);
}

// --- obsluga przyciksu UP tower ---

void Game::tryTowerType(sf::Vector2f mousePos) {
    if (gameOver) return;
    if (!TowerTypeButton.getGlobalBounds().contains(mousePos)) return;

    towerUpgradeStep = (towerUpgradeStep + 1) % 3; // 0->1->2->0

    if (towerUpgradeStep == 0) {
        TowerTypeButton.setFillColor(sf::Color::Blue);
        TowerTypeText.setString("BASIC");
    }
    else if (towerUpgradeStep == 1) {
        TowerTypeButton.setFillColor(sf::Color::Yellow);
        TowerTypeText.setString("UP");
    }
    else {
        TowerTypeButton.setFillColor(sf::Color::Red);
        TowerTypeText.setString("MAX");
    }
    updateTowerTypeIcon();
}

// --- Gettery ---
int Game::getBaseHP() const { return baseHP; }
bool Game::isGameOver() const { return gameOver; }

// --- Zapis / ładowanie gry ---
void Game::saveGame(int slot) {
    std::ofstream file("save" + std::to_string(slot) + ".txt");
    if (!file.is_open()) return;

    // --- Podstawowe dane ---
    file << "wave " << currentWave << "\n";
    file << "lives " << playerLives << "\n";
    file << "gold " << gold << "\n";
    file << "shopLevel " << towerUpgradeStep << "\n"; 



    // --- Wieże ---
    file << "towers " << towers.size() << "\n";
    for (auto& t : towers) {
        sf::Vector2f pos = t.getPosition();
        file << pos.x << " " << pos.y << " " << t.getType() << "\n";
    }

    // --- Przeciwnicy ---
    file << "enemies " << enemies.size() << "\n";
    for (auto& e : enemies) {
        sf::Vector2f pos = e.getPosition();
        file << static_cast<int>(e.getType()) << " " << e.getHP() << " " << pos.x << " " << pos.y << "\n";
    }

    // --- Spawn i stan fali ---
    file << "toSpawn " << enemiesToSpawn << "\n";
    file << "waveInProgress " << waveInProgress << "\n";
    file << "spawnTimer " << spawnTimer << "\n";
    file << "waveBreakTimer " << waveBreakTimer << "\n";

    // --- InfoText ---
    infoText.setString("Game Saved");
    infoClock.restart();
    showInfo = true;
}

void Game::loadGame(int slot) {
    enemies.clear();
    bullets.clear();
    towers.clear();

    std::ifstream file("save" + std::to_string(slot) + ".txt");
    if (!file.is_open()) return;

    std::string label;
    int towerCount = 0, enemyCount = 0;

    // --- Podstawowe dane ---
    file >> label >> currentWave;
    file >> label >> playerLives;
	file >> label >> gold;
	file >> label >> towerUpgradeStep;

    // --- WYGLĄD PRZYCISKU ---
    if (towerUpgradeStep == 1) {
        TowerTypeButton.setFillColor(sf::Color::Yellow);
        TowerTypeText.setString("UP: Strong");
    }
    else if (towerUpgradeStep == 2) {
        TowerTypeButton.setFillColor(sf::Color::Red);
        TowerTypeText.setString("MAX");
    }
    else {
        TowerTypeButton.setFillColor(sf::Color::Blue);
        TowerTypeText.setString("UP");
    }

    // --- Wieże ---
    loadTowerTextures();
    file >> label >> towerCount;
    for (int i = 0; i < towerCount; ++i) {
        float x, y;
        int ttype;
        file >> x >> y;
        towers.emplace_back(20, x, y, towerUpgradeStep, towerTextures, map->tileSize);
    }
    

    isBossWave = (currentWave % 5 == 0);
    currentWaveConfig.count = isBossWave ? 1 : (3 + currentWave);
    currentWaveConfig.enemyHP = isBossWave ? 600 : (30 + currentWave * 30);
    currentWaveConfig.speed = isBossWave ? 60.f : (100.f + currentWave * 2.f);
    spawnDelay = isBossWave ? 1.2f : std::max(0.25f, 0.7f - currentWave * 0.03f);

    // --- Przeciwnicy ---
    file >> label >> enemyCount;
    for (int i = 0; i < enemyCount; ++i) {
        int typeInt, hp; float x, y;
        file >> typeInt >> hp >> x >> y;
        EnemyType type = static_cast<EnemyType>(typeInt);
        enemies.emplace_back(hp, x, y, type);
        enemies.back().setMap(map);
        enemies.back().setSpeed(currentWaveConfig.speed);
    }

    // --- Spawn i stan fali ---
    file >> label >> enemiesToSpawn;
    file >> label >> waveInProgress;
    file >> label >> spawnTimer;
    file >> label >> waveBreakTimer;

    // --- InfoText ---
    infoText.setString("Game Loaded");
    infoClock.restart();
    showInfo = true;
    waveInProgress = true;
    spawnTimer = 0.f;
    waveBreakTimer = 0.f;

    file.close();
}

// --- Autosave ---
void Game::autoSave() {
    std::ofstream file("autosave.txt");
    if (!file.is_open()) return;

    file << currentWave << "\n" << playerLives << "\n";
    file << towers.size() << "\n";
    for (auto& t : towers)
        file << t.getPosition().x << " " << t.getPosition().y << "\n";

    file.close();
}
bool Game::loadAutoSave() {
    std::ifstream file("autosave.txt");
    if (!file.is_open()) return false;

    enemies.clear();
    bullets.clear();
    towers.clear();

    file >> currentWave >> playerLives;
    int towerCount; file >> towerCount;
    for (int i = 0; i < towerCount; ++i) {
        float x, y; file >> x >> y;
        towers.emplace_back(20, x, y, towerUpgradeStep, towerTextures, map->tileSize);
    }

    updateUI();
    return true;
}

// --- Sprawdzenie istniejącego save ---
bool Game::saveExists(int slot) const {
    std::ifstream file("save" + std::to_string(slot) + ".txt");
    return file.good();
}
SaveInfo Game::getSaveInfo(int slot) {
    SaveInfo info{ 0,0,0 };
    std::ifstream file("save" + std::to_string(slot) + ".txt");
    if (!file.is_open()) return info;
    std::string label;
    file >> label >> info.wave;
    file >> label >> info.health;
    file >> label >> info.gold;

    return info;
}
void Game::HandleHover(sf::Vector2i mousepos) {
    sf::Vector2f mousePosF((float)mousepos.x, (float)mousepos.y);
    if (!gameOver) return;

    RestartButton.setFillColor(
        RestartButton.getGlobalBounds().contains(mousePosF)
        ? sf::Color::Cyan
        : sf::Color::Blue
    );

    ExitButton.setFillColor(
        ExitButton.getGlobalBounds().contains(mousePosF)
        ? sf::Color::Cyan
        : sf::Color::Blue
    );
}

bool Game::isClickOnUI(sf::Vector2f mousePos) const {
    return TowerTypeButton.getGlobalBounds().contains(mousePos);
}