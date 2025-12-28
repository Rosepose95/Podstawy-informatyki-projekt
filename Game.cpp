#include "Game.h"
#include "Enemy.h"
#include <algorithm> //dodatek bibliotek
#include <cmath> 

Game::Game()  //duże zmiany w konstruktorze, pamiętajcie o pobraniu foldera assets!!
    : waveText(font),
    enemiesText(font),
    livesText(font)
{
    font.openFromFile("assets/ArialMT.ttf");

    waveText.setCharacterSize(20);
    enemiesText.setCharacterSize(20);
    livesText.setCharacterSize(20);

    waveText.setFillColor(sf::Color::Black);
    enemiesText.setFillColor(sf::Color::Black);
    livesText.setFillColor(sf::Color::Black);

    waveText.setPosition({ 10.f, 5.f });
    enemiesText.setPosition({ 10.f, 30.f });
    livesText.setPosition({ 10.f, 55.f });
}

void Game::addEnemy(Enemy enemy) {
	enemy.setMap(map);   // bardzo ważne by nam się poruszał po mapie
    enemies.push_back(enemy);
}

void Game::addTower(const Tower& t) {
    towers.push_back(t);
}
void Game::placeTower(sf::Vector2f position) {  //by wieże trzymały się trawy
    if (!map) return;

    int gridX = position.x / map->tileSize;
    int gridY = position.y / map->tileSize;


    if (gridX < 0 || gridY < 0 ||
        gridX >= map->getWidth() ||
        gridY >= map->getHeight())
        return;

    if (map->getTile(gridX, gridY) != '.')
        return;

    for (const auto& t : towers) {
        sf::Vector2f pos = t.getPosition();
        int tx = pos.x / map->tileSize;
        int ty = pos.y / map->tileSize;

        if (tx == gridX && ty == gridY)
            return;
    }

    float ts = map->tileSize;
    float centerX = gridX * ts + ts / 2.f;
    float centerY = gridY * ts + ts / 2.f;

    towers.emplace_back(20, centerX, centerY);
}
void Game::startNextWave() {  //nowe fale
    currentWave++;

    currentWaveConfig.count = 5 + currentWave * 3;
    currentWaveConfig.enemyHP = 100 + currentWave * 30;
    currentWaveConfig.speed = 80.f + currentWave * 5.f;
    currentWaveConfig.spawnDelay = std::max(0.25f, 0.7f - currentWave * 0.03f);

    enemiesToSpawn = currentWaveConfig.count;
    enemiesAlive = enemiesToSpawn;

    spawnTimer = 0.f;
}

void Game::update(float dt) {  //bardzo dużo zmian, od fali po przeciwników i wieże
    if (gameOver)
        return;
    // jeśli nie ma wrogów – nowa fala
    if (enemiesAlive == 0 && enemiesToSpawn == 0) {
        startNextWave();
    }

    // spawn
    if (enemiesToSpawn > 0) {
        spawnTimer += dt;

        if (spawnTimer >= spawnDelay) {
            spawnTimer = 0.f;

            int ts = map->tileSize;

            enemies.emplace_back(
                currentWaveConfig.enemyHP,
                0 * ts + ts / 2.f,
                2 * ts + ts / 2.f
            );

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
            enemiesAlive--;
            it = enemies.erase(it);
        }
        else if (it->isDead()) {
            enemiesAlive--;
            it = enemies.erase(it);
        }

        else {
            ++it;
        }
    }




   
    // 2. ATAK WIEŻ (TWORZENIE BULLETÓW)
 
    for (auto& t : towers) {
        for (auto& e : enemies) {
            if (!e.isDead()) {
                t.updateAttack(e, dt, bullets);
                break;
            }
        }
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
   
}
    
void Game::draw(sf::RenderWindow& window) const {
    for (const auto& e : enemies)
        e.draw(window);

    for (const auto& t : towers)
        t.draw(window);

    for (const auto& b : bullets)
        b.draw(window);
}

int Game::getBaseHP() const {
    return baseHP;
}

bool Game::isGameOver() const {
    return gameOver;
}
void Game::setMap(Map* m) { //nowa metoda do mapy
    map = m;
}
void Game::drawUI(sf::RenderWindow& window) const {  /*nowa funkcja do ui :)*/

    //LIVES
    for (int i = 0; i < playerLives; ++i) {
        sf::CircleShape life(6.f);
        life.setFillColor(sf::Color::Red);
        life.setPosition({ 10.f + i * 15.f, 10.f });
        window.draw(life);
    }


