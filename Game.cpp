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

Game::Game()
    : font()
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
}

// --- Rozpoczęcie kolejnej fali ---
void Game::startNextWave() {
    currentWave++;
    waveInProgress = true;

    // co 5 fala = boss
    isBossWave = (currentWave % 5 == 0);
    if (isBossWave) autoSave();

    currentWaveConfig.count = isBossWave ? 1 : (3 + currentWave);
    currentWaveConfig.enemyHP = isBossWave ? 600 : (30 + currentWave * 30);
    currentWaveConfig.speed = isBossWave ? 60.f : (100.f + currentWave * 2.f);

    enemiesToSpawn = currentWaveConfig.count;
    spawnDelay = isBossWave ? 1.2f : std::max(0.25f, 0.7f - currentWave * 0.03f);
    spawnTimer = 0.f;
}

// --- Aktualizacja gry ---
void Game::update(float dt) {
    if (gameOver) return;
    

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
            EnemyType type = EnemyType::Normal;
            if (isBossWave) type = EnemyType::Boss;
            else {
                if (currentWave >= 3 && rand() % 3 == 0) type = EnemyType::Fast;
                if (currentWave >= 5 && rand() % 5 == 0) type = EnemyType::Tank;
            }

            auto& spawns = map->spawnPoints;
            if (spawns.empty()) return;
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
    for (auto it = enemies.begin(); it != enemies.end();) {
        it->update(dt);
        if (it->reachedGoal()) {
            playerLives -= it->getLifeDamage();
            it = enemies.erase(it);
        }
        else if (it->isDead()) it = enemies.erase(it);
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
    if (showInfo && infoClock.getElapsedTime().asSeconds() < 2.f)
        window.draw(infoText);

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

// --- ZAPIS GRY ---
void Game::saveGame(int slot) {
    if (!map) return;

    std::ofstream file("save" + std::to_string(slot) + ".txt");
    if (!file.is_open()) return;
    file << "TD_SAVE_V1\n";

    file << (slot == 0) << "\n"; // isAuto
    file << map->mapId << "\n";
    // --- MAPA ---
    file << map->getWidth() << " " << map->getHeight() << "\n";

    for (int y = 0; y < map->getHeight(); ++y) {
        for (int x = 0; x < map->getWidth(); ++x) {
            file << map->getTile(x, y);
        }
        file << "\n";
    }

    

    // --- PODSTAWOWY STAN ---
    file << currentWave << "\n";
    file << playerLives << "\n";
    file << gold << "\n";
    file << baseHP << "\n";
    file << enemiesToSpawn << "\n";
    file << spawnTimer << "\n";
    file << waveInProgress << "\n";
    file << waveBreakTimer << "\n";

    // --- KONFIGURACJA FALI ---
    file << currentWaveConfig.enemyHP << "\n";
    file << currentWaveConfig.speed << "\n";
    file << spawnDelay << "\n";
    file << isBossWave << "\n";

    // --- WIEŻE (tylko pozycje) ---
    file << towers.size() << "\n";
    for (auto& t : towers)
        file << t.getPosition().x << " " << t.getPosition().y << "\n";

    // --- WROGOWIE ---
    file << enemies.size() << "\n";
    for (auto& e : enemies) {
        file << e.getHP() << " "
            << e.getMaxHP() << " "
            << e.getTilePos().x << " "
            << e.getTilePos().y << " "
            << e.getPrevTile().x << " "
            << e.getPrevTile().y << " "
            << static_cast<int>(e.getType()) << "\n";
    }
}



// --- WCZYTYWANIE GRY ---
void Game::loadGame(int slot) {
    if (!map) {
        std::cout << "Nie mozna wczytac gry, brak mapy!\n";
        return;
    }
    map->refreshLogic();
    std::ifstream file("save" + std::to_string(slot) + ".txt");
    if (!file.is_open()) {
        std::cout << "Nie mogę otworzyć pliku: save" << slot << ".txt\n";
        return;
    }
    else {
        std::cout << "Otworzono plik save" << slot << ".txt\n";
    }

    std::string header;
    file >> header;
    if (header != "TD_SAVE_V1") {
        std::cout << "Corrupted save\n";
        return;
    }
    
    bool isAuto;
    file >> isAuto;

    std::string mapId;
    if (!(file >> mapId)) {
        mapId = "default"; // jeśli nie ma mapId w pliku, ustawiamy domyślną
    }
    map->mapId = mapId;

    int w, h;
    file >> w >> h;
    file.ignore(); // usuwa '\n'

    // czyścimy mapę
    map->clearMap();
   

    for (int y = 0; y < h; ++y) {
        std::string line;
        std::getline(file, line);
        for (int x = 0; x < w; ++x) {
            map->setTile(x, y, line[x]);
        }
    }

    

    map->refreshLogic();
    enemies.clear();
    towers.clear();
    bullets.clear();

    gameOver = false;
    showInfo = false;
    waveJustLoaded = true;

    int waveFlag, bossFlag;

    // --- PODSTAWOWY STAN ---
    if (!(file >> currentWave >> playerLives >> gold >> baseHP >> enemiesToSpawn >> spawnTimer >> waveFlag >> waveBreakTimer)) {
        std::cout << "Błąd wczytywania podstawowego stanu\n";
        return;
    }
    waveInProgress = (waveFlag != 0);

    // --- KONFIGURACJA FALI ---
    if (!(file >> currentWaveConfig.enemyHP >> currentWaveConfig.speed >> spawnDelay >> bossFlag)) {
        std::cout << "Błąd wczytywania konfiguracji fali\n";
        return;
    }
    isBossWave = (bossFlag != 0);

    // --- WIEŻE ---
    size_t towerCount;
    if (!(file >> towerCount)) { std::cout << "Błąd wczytywania liczby wież\n"; return; }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // usuwa \n po liczbie

    for (size_t i = 0; i < towerCount; ++i) {
        std::string line;
        if (!std::getline(file, line)) {
            std::cout << "Blad wczytywania wiezy nr " << i << "\n";
            continue;
        }
        std::istringstream iss(line);
        float x, y;
        if (!(iss >> x >> y)) {
            std::cout << "Blad wczytywania wiezy nr " << i << "\n";
            continue;
        }
        towers.emplace_back(20, x, y);
    }



    // --- WROGOWIE ---
    size_t enemyCount;
    if (!(file >> enemyCount)) { std::cout << "Błąd wczytywania liczby wrogów\n"; return; }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // usuwa \n po liczbie

    for (size_t i = 0; i < enemyCount; ++i) {
        std::string line;
        if (!std::getline(file, line)) {
            std::cout << "Blad wczytywania wroga nr " << i << "\n";
            continue;
        }
        std::istringstream iss(line);

        int hp, maxHp, typeInt, tx, ty, ptx, pty;
        if (!(iss >> hp >> maxHp >> tx >> ty >> ptx >> pty >> typeInt)) {
            std::cout << "Blad wczytywania wroga nr " << i << "\n";
            continue;
        }

        Enemy e(maxHp, 0, 0, static_cast<EnemyType>(typeInt));
        e.setMap(map);
        e.setMaxHP(maxHp);
        e.setHP(hp);
        e.restorePath({ tx, ty }, { ptx, pty });
        e.setSpeed(currentWaveConfig.speed);


        enemies.push_back(e);
    }


    std::cout << "Loaded wave: " << currentWave << "\n";
    std::cout << "Enemies loaded: " << enemies.size() << "\n";
    std::cout << "Wave in progress: " << waveInProgress << "\n";
}


// --- Sprawdzenie istniejącego save ---
bool Game::saveExists(int slot) const {
    std::ifstream file("save" + std::to_string(slot) + ".txt");
    return file.good();
}



void Game::HandleHover(sf::Vector2i mousepos) {
    sf::Vector2f mousePosF((float)mousepos.x, (float)mousepos.y);
    if (!gameOver) return;

    RestartButton.setFillColor(RestartButton.getGlobalBounds().contains(mousePosF) ? sf::Color::Cyan : sf::Color::Blue);
    ExitButton.setFillColor(ExitButton.getGlobalBounds().contains(mousePosF) ? sf::Color::Cyan : sf::Color::Blue);
}


std::string Game::getSaveDescription(int slot) const {
    std::ifstream file("save" + std::to_string(slot) + ".txt");
    if (!file.is_open()) return "EMPTY SLOT";
   

    // --- NAGŁÓWEK ---
    std::string header;
    file >> header;
    if (header != "TD_SAVE_V1")
        return "CORRUPTED SAVE";

    bool isAuto;
    std::string mapId;
    int w, h;

    file >> isAuto;

    // jeśli mapId jest w pliku – wczytaj, jeśli nie – default
    if (!(file >> mapId)) {
        mapId = "default";
    }
    file >> w >> h;
    file.ignore();


    // --- POMIJAMY MAPĘ ---
    for (int y = 0; y < h; ++y) {
        std::string dummy;
        std::getline(file, dummy);
    }

    // --- STAN GRY ---
    int wave = 0, lives = 0, gold = 0, baseHP = 0;
    int enemiesToSpawn = 0;
    float spawnTimer = 0.f;
    bool waveInProgress = false;
    float waveBreakTimer = 0.f;

    if (!(file >> wave >> lives >> gold >> baseHP
        >> enemiesToSpawn >> spawnTimer
        >> waveInProgress >> waveBreakTimer)) {
        return "CORRUPTED SAVE";
    }

    if (slot == 0)
        return "[AUTO] Wave " + std::to_string(wave) +
        " | Gold " + std::to_string(gold);

    return "Wave " + std::to_string(wave) +
        " | Gold " + std::to_string(gold) +
        " | Lives " + std::to_string(lives);
}



void Game::autoSave() { saveGame(0); } // slot 0 = autosave
bool Game::autoSaveExists() const { std::ifstream f("save0.txt"); return f.good(); }
std::string Game::getAutoSaveDescription() const {
    return getSaveDescription(0);  
}
void Game::skipMap(std::ifstream& file) {
    int w, h;
    file >> w >> h;
    file.ignore();

    for (int i = 0; i < h; ++i) {
        std::string dummy;
        std::getline(file, dummy);
    }
}

