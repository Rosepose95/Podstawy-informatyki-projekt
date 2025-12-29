#include "Enemy.h"
#include <cmath>
#include <algorithm>

Enemy::Enemy(int h, float startX, float startY, EnemyType t)
	: health(h), maxHealth(h), type(t) { //dodatek typu enemy i życia dla bossa

    shape.setRadius(15.f);
    shape.setOrigin({ 15.f, 15.f });

    switch (type) {
    case EnemyType::Normal:
        speed = 100.f;
        shape.setFillColor(sf::Color::Red);
        break;

    case EnemyType::Fast:
        speed = 160.f;
        shape.setFillColor(sf::Color::Yellow);
        break;

    case EnemyType::Tank:
        speed = 60.f;
        shape.setFillColor(sf::Color(150, 0, 0));
        break;
    case EnemyType::Boss:
        speed = 40.f;
        maxHealth = health = h * 5;
        shape.setRadius(28.f);
        shape.setOrigin({ 28.f, 28.f });
        shape.setFillColor(sf::Color(80, 0, 120));
        break;
    }

    shape.setPosition({ startX, startY });

    tilePos = {
        int(startX) / 40,
        int(startY) / 40
    };
    prevTile = tilePos;
    nextTile = tilePos;
}


void Enemy::setMap(const Map* m) { //nowe fukcja do pathing
    map = m;
    int ts = map->tileSize;

    tilePos.x = shape.getPosition().x / ts;
    tilePos.y = shape.getPosition().y / ts;

    prevTile = tilePos;
    nextTile = findNextTile();

}

sf::Vector2i Enemy::findNextTile() const {
    static const sf::Vector2i dirs[4] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    sf::Vector2i best = tilePos;
    float bestDist = 1e9f;

    for (auto d : dirs) {
        sf::Vector2i candidate = tilePos + d;

        if (candidate == prevTile)
            continue;

        if (candidate.x < 0 || candidate.y < 0 ||
            candidate.x >= map->getWidth() ||
            candidate.y >= map->getHeight())
            continue;

        char tile = map->getTile(candidate.x, candidate.y);
        if (tile != '#' && tile != '*')
            continue;

        float dx = candidate.x - map->baseTile.x;
        float dy = candidate.y - map->baseTile.y;
        float dist = dx * dx + dy * dy;

        if (dist < bestDist) {
            bestDist = dist;
            best = candidate;
        }
    }

    return best;
}

bool Enemy::reachedGoal() const {
    if (!map) return false;
    return map->getTile(tilePos.x, tilePos.y) == '*';
}


sf::Vector2f Enemy::tileCenter(sf::Vector2i tile) const {
    float ts = static_cast<float>(map->tileSize);
    return {
        tile.x * ts + ts / 2.f,
        tile.y * ts + ts / 2.f
    };
}

void Enemy::update(float dt) {
    if (!map) return;
    if (reachedGoal()) return;


    sf::Vector2f target = tileCenter(nextTile);
    sf::Vector2f pos = shape.getPosition();

    sf::Vector2f toTarget = target - pos;
    float dist = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

    float moveDist = speed * dt;

    if (dist <= moveDist) {
        // SNAP do środka kafelka
        shape.setPosition(target);

        prevTile = tilePos;
        tilePos = nextTile;
        nextTile = findNextTile();

        return;
    }

    // NORMALNY RUCH
    sf::Vector2f dir = toTarget / dist;
    shape.move(dir * moveDist);
}



void Enemy::takeDamage(int dmg) { //zmiana
    health -= dmg;
    if (health < 0) health = 0;
}

bool Enemy::isDead() const {
    return health <= 0;
}

sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}

float Enemy::getRadius() const { //metody do paska
    return shape.getRadius();
}


void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);

    float ratio = static_cast<float>(health) / maxHealth;
    ratio = std::clamp(ratio, 0.f, 1.f);

    sf::Vector2f pos = shape.getPosition();

    sf::RectangleShape back({ 36.f, 5.f });
    back.setFillColor(sf::Color::Red);
    back.setPosition({ pos.x - 18.f, pos.y - 30.f });

    sf::RectangleShape hp({ 36.f * ratio, 5.f });
    hp.setFillColor(sf::Color::Green);
    hp.setPosition(back.getPosition());

    window.draw(back);
    window.draw(hp);
}


