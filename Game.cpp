#include "Game.h"

Game::Game()
    : baseHP(10), gameOver(false) {
}

void Game::addEnemy(const Enemy& e) {
    enemies.push_back(e);
}

void Game::addTower(const Tower& t) {
    towers.push_back(t);
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

    // usuwanie martwych
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return e.isDead(); }),
        enemies.end()
    );
    for (auto& b : bullets)
        b.update(dt);

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.isDead(); }),
        bullets.end()
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
void Game::placeTower(sf::Vector2f position) {
    towers.emplace_back(20, position.x, position.y);
}
