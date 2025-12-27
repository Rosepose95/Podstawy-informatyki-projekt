#include "Game.h"
#include <algorithm> //dodatek bibliotek
#include <cmath> 

Game::Game()
    : baseHP(10), gameOver(false) {
}

void Game::addEnemy(const Enemy& e) {
    enemies.push_back(e);
}

void Game::addTower(const Tower& t) {
    towers.push_back(t);
}
void Game::placeTower(sf::Vector2f position) {   //by wieże trzymały się trawy
    if (!map) return;

    int gridX = position.x / map->tileSize;
    int gridY = position.y / map->tileSize;

    if (gridX < 0 || gridY < 0 ||
        gridX >= map->getWidth() ||
        gridY >= map->getHeight())
        return;

    if (map->getTile(gridX, gridY) != '.')
        return;

    towers.emplace_back(20, position.x, position.y);
}

void Game::update(float dt) {
    if (gameOver)
        return;

    // ruch przeciwników
    for (auto& e : enemies) {
        e.move(100.f * dt, 0.f);
    }

    // atak wie¿
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
// 6. USUWANIE MARTWYCH ENEMY
enemies.erase(
    std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& e) { return e.isDead(); }),
    enemies.end()
);
    

}

void Game::draw(sf::RenderWindow& window) const {
    for (const auto& e : enemies)
        e.draw(window);
    for (const auto& b : bullets)
        b.draw(window);
    for (const auto& t : towers)
        t.draw(window);
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

