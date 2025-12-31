#include "Game.h"
#include "Enemy.h"
#include "Tower.h"     //dodatek bibliotek
#include <algorithm>
#include <cmath>
#include "Map.h"
Game::Game()               //duże zmiany w konstruktorze, pamiętajcie o pobraniu foldera assets!!
    : font()
    , waveText(font)
    , enemiesText(font)
    , livesText(font)
    , GameOverText(font)
    , NextWaveText(font)
    , playerLives(20)
    , baseHP(100)
    , gameOver(false)
{
    if (!font.openFromFile("assets/ArialMT.ttf")) {
        throw std::runtime_error("Nie można wczytać czcionki");
    }

    waveText.setCharacterSize(20);
    enemiesText.setCharacterSize(20);
    livesText.setCharacterSize(20);
    GameOverText.setCharacterSize(60);
    NextWaveText.setCharacterSize(60);

    waveText.setFillColor(sf::Color::Black);
    enemiesText.setFillColor(sf::Color::Black);
    livesText.setFillColor(sf::Color::Black);
    GameOverText.setFillColor(sf::Color::Red);
    NextWaveText.setFillColor(sf::Color::Green);

    waveText.setPosition(sf::Vector2f{ 10.f, 20.f });
    enemiesText.setPosition(sf::Vector2f{ 10.f, 40.f });
    livesText.setPosition(sf::Vector2f{ 10.f, 55.f });
    GameOverText.setPosition(sf::Vector2f{ 280.f, 420.f });
    NextWaveText.setPosition(sf::Vector2f{ 280.f, 420.f });

    GameOverText.setStyle(sf::Text::Bold | sf::Text::Italic);
    NextWaveText.setStyle(sf::Text::Bold | sf::Text::Italic);

    GameOverText.setString("GAME OVER");
}
void Game::addEnemy(Enemy enemy) {
    enemy.setMap(map);   // bardzo ważne by nam się poruszał po mapie
    enemies.push_back(enemy);
}
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
}



void Game::addTower(Tower tower) {
    towers.push_back(tower);
}


void Game::placeTower(sf::Vector2f position) {   //by wieże trzymały się trawy
    if (!map) return;

    int gridX = static_cast<int>(position.x / map->tileSize);
    int gridY = static_cast<int>(position.y / map->tileSize);

    if (gridX < 0 || gridY < 0 ||
        gridX >= map->getWidth() ||
        gridY >= map->getHeight())
        return;

    if (map->getTile(gridX, gridY) != '.')
        return;

    for (const auto& t : towers) {
        sf::Vector2f pos = t.getPosition();
        int tx = static_cast<int>(pos.x / map->tileSize);
        int ty = static_cast<int>(pos.y / map->tileSize);

        if (tx == gridX && ty == gridY)
            return;
    }

    float tsF = static_cast<float>(map->tileSize);
    float centerX = gridX * tsF + tsF / 2.f;
    float centerY = gridY * tsF + tsF / 2.f;

    towers.emplace_back(20, centerX, centerY);

}

void Game::startNextWave() {   //nowe fale
    currentWave++;
    waveInProgress = true;

    // co 5 fala = boss
    isBossWave = (currentWave % 5 == 0);

    currentWaveConfig.count = isBossWave ? 1 : (3 + currentWave);
    currentWaveConfig.enemyHP = isBossWave ? 600 : (30 + currentWave * 30);
    currentWaveConfig.speed = isBossWave ? 60.f : (100.f + currentWave * 2.f);

    enemiesToSpawn = currentWaveConfig.count;

    spawnDelay = isBossWave ? 1.2f : std::max(0.25f, 0.7f - currentWave * 0.03f);
    spawnTimer = 0.f;
}

void Game::update(float dt) {   //bardzo dużo zmian, od fali po przeciwników i wieże
    if (gameOver)
        return;

    if (waveInProgress && enemies.empty() && enemiesToSpawn == 0) {
        waveInProgress = false;
        waveBreakTimer = 0.f;
    }
    // jeżeli fala się skończyła i nie ma przeciwników
    if (!waveInProgress && enemies.empty() && enemiesToSpawn == 0) {

        // ustawiamy slider tylko RAZ na początku przerwy
        if (waveBreakTimer == 0.f) {
            NextWaveText.setPosition(sf::Vector2f{ -600.f, 420.f });
            NextWaveText.setString(
                "Get ready for wave " + std::to_string(currentWave + 1)
            );
        }

        waveBreakTimer += dt;

        // animacja przesuwania tekstu
        NextWaveText.move(sf::Vector2f(600.f * dt, 0.f));

        // po przerwie start nowej fali
        if (waveBreakTimer >= breakDuration) {
            startNextWave();
            waveBreakTimer = 0.f;
        }
    }

    

    // spawn
    if (enemiesToSpawn > 0) {
        spawnTimer += dt;

        if (spawnTimer >= spawnDelay) {
            spawnTimer = 0.f;

            int ts = map->tileSize;

            EnemyType type = EnemyType::Normal;

            if (isBossWave) {
                type = EnemyType::Boss;
            }
            else {
                if (currentWave >= 3 && rand() % 3 == 0)
                    type = EnemyType::Fast;

                if (currentWave >= 5 && rand() % 5 == 0)
                    type = EnemyType::Tank;
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


    if (playerLives <= 0) {
        gameOver = true;
    }

    // 1. RUCH PRZECIWNIKÓW

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        it->update(dt);

        if (it->reachedGoal()) {
            playerLives--;

            it = enemies.erase(it);
        }
        else if (it->isDead()) {

            it = enemies.erase(it);
        }

        else {
            ++it;
        }
    }

    // 2. ATAK WIEŻ (TWORZENIE BULLETÓW)

    for (auto& t : towers) {
        t.updateAttack(enemies, dt, bullets);  // przekazujemy cały wektor
    }

    // 3. UPDATE BULLETÓW

    for (auto& b : bullets) {
        b.update(dt);
    }

    // 4. KOLIZJE BULLET <-> ENEMY 

    for (auto& b : bullets) {
        for (auto& e : enemies) {

            if (e.isDead())
                continue;

            sf::Vector2f bp = b.getPosition();
            sf::Vector2f ep = e.getPosition();

            float dx = bp.x - ep.x;
            float dy = bp.y - ep.y;
            float dist2 = dx * dx + dy * dy;

            float r = e.getRadius();

            if (dist2 <= r * r) {
                e.takeDamage(b.getDamage());
                b.kill();
                break;
            }
        }
    }

    // 5. USUWANIE MARTWYCH BULLETÓW

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.isDead(); }),
        bullets.end()
    );

    updateUI();

}

void Game::draw(sf::RenderWindow& window) const {
    for (const auto& t : towers)
        t.draw(window);
    for (const auto& e : enemies)
        e.draw(window);
    for (const auto& b : bullets)
        b.draw(window);

    if (!waveInProgress && enemies.empty() && enemiesToSpawn == 0) {
        window.draw(NextWaveText);
    }

    if (gameOver) {
        window.draw(GameOverText);
    }
}

int Game::getBaseHP() const {
    return baseHP;
}

bool Game::isGameOver() const {
    return gameOver;
}
void Game::setMap(Map* m) {  //nowa metoda do mapy
    map = m;
}
void Game::drawUI(sf::RenderWindow& window) const {
    window.draw(waveText);
    window.draw(enemiesText);

    for (int i = 0; i < playerLives; ++i) {
        sf::CircleShape life(6.f);
        life.setFillColor(sf::Color::Red);
        life.setPosition({ 10.f + i * 15.f, 10.f });
        window.draw(life);
    }
}

void Game::updateUI() {
    waveText.setString(
        "Wave: " + std::to_string(currentWave)
    );

    enemiesText.setString(
        "Enemies: " + std::to_string(enemies.size() + enemiesToSpawn)
    );
}


bool Game::canPlaceTower(sf::Vector2f pos) const {
    if (!map) return false;

    int tx = pos.x / map->tileSize;
    int ty = pos.y / map->tileSize;

    char tile = map->getTile(tx, ty);
    return tile == '.';
}

