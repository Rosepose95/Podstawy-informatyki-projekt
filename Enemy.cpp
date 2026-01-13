#include "Enemy.h"
#include "Game.h"
#include "Tower.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Enemy::Enemy(int h, float startX, float startY, EnemyType t)
    : health(h), maxHealth(h), type(t) { //dodatek typu enemy i życia dla bossa

    shape.setRadius(15.f);
    shape.setOrigin({ 15.f, 15.f });

    switch (type) {
    case EnemyType::Normal:
        baseSpeed = 100.f;
        shape.setFillColor(sf::Color::Red);
        break;

    case EnemyType::Fast:
        baseSpeed = 140.f;
        shape.setFillColor(sf::Color::Yellow);
        break;

    case EnemyType::Infestor:
        baseSpeed = 60.f;
        health = h * 2;
        shape.setFillColor(sf::Color(150, 0, 0));
        break;
    case EnemyType::Boss:
        isBossEnemy = true; // <<< BRAKOWAŁO
        baseSpeed = 40.f;
        maxHealth = h * 10;
        health = maxHealth;
        shape.setRadius(28.f);
        shape.setOrigin({ 28.f, 28.f });
        shape.setFillColor(sf::Color(80, 0, 120));
        break;

    case EnemyType::Fireball:
        baseSpeed = 160.f;
        shape.setFillColor(sf::Color(255, 120, 0));
        break;

    case EnemyType::Flame:
        baseSpeed = 60.f;
        shape.setFillColor(sf::Color(200, 60, 20));
        break;
    }
    speed = baseSpeed;
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
    if (type == EnemyType::Boss) {
        bossAbilityTimer += dt;
        if (bossAbilityTimer >= 5.f) {
            bossAbilityTimer = 0.f;

            if (bossCallback) {
                bossCallback(getPosition());
            }

        }
    }
    if (!map) return;
    if (reachedGoal()) return;
    if (isBossEnemy && !rage && health <= maxHealth * 0.5f) {
        rage = true;
        speed *= 1.8f;          // rage speed
    }
    if (isBossEnemy && rage) {
        shape.setFillColor(sf::Color(160, 0, 200)); // rage color
    }

    
    if (nextTile == tilePos)
        nextTile = findNextTile();


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
    if (type == EnemyType::Infestor) {
        infestationTimer += dt;
        if (infestationTimer >= 1.0f) {
            infestationTimer = 0.f;

            if (infestCallback) {
                infestCallback(getPosition(), map->tileSize * 2.f, 1);

            }
        }
    }
    if (isBossEnemy) {
        infestationTimer += dt;
        if (infestationTimer >= 2.0f) {
            infestationTimer = 0.f;

            if (infestCallback) {
                infestCallback(getPosition(), map->tileSize * 2.f, 1);

            }
        }
    }

    if (type == EnemyType::Flame) {
        burnTimer += dt;

        if (burnTimer >= 1.5f) {
            burnTimer = 0.f;

            if (burnCallback) {
                burnCallback(getPosition(), 50.f, 1);
            }
        }
    }

    if (type == EnemyType::Fireball) {
        burnTimer += dt;
        if (burnTimer >= 5.f) {
            burnTimer = 0.f;
            if (burnCallback)
                burnCallback(getPosition(), 80.f, 1);
        }
    }
    
    if (type == EnemyType::Fireball || type == EnemyType::Flame) {
        burnTimer += dt;
        if (burnTimer >= 3.f) {
            burnTimer = 0.f;
            if (burnCallback)
                burnCallback(getPosition(), 120.f, 1);
        }
    }

    if (type == EnemyType::Crusher) {
        burnTimer += dt;
        if (burnTimer >= 2.f) {
            burnTimer = 0.f;
            if (burnCallback)
                burnCallback(getPosition(), 60.f, 1);
        }
    }
    if (type == EnemyType::FireBoss) {
        bossSkillTimer += dt;

        if (bossSkillTimer >= 2.5f) {
            bossSkillTimer = 0.f;
            if (bossCallback)
                bossCallback(getPosition());
        }
    }

    
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

    float barWidth = isBossEnemy ? 50.f : 36.f;

    sf::RectangleShape back({ barWidth , 5.f });
    back.setFillColor(sf::Color::Red);
    back.setPosition({ pos.x - 18.f, pos.y - 30.f });

    sf::RectangleShape hp({ barWidth * ratio, 5.f });
    hp.setFillColor(sf::Color::Green);
    hp.setPosition(back.getPosition());

    window.draw(back);
    window.draw(hp);
}
int Enemy::getLifeDamage() const {
    if (type == EnemyType::Boss)
        return 5;   // boss zabiera 5 żyć
    return 1;       // normalny wróg
}
void Enemy::forceWorldPosition(sf::Vector2f pos) {
    shape.setPosition(pos);
}
void Enemy::recalculatePath() {
    if (!map) return;

    int ts = map->tileSize;

    // przelicz kafelek na podstawie aktualnej pozycji
    tilePos.x = static_cast<int>(shape.getPosition().x) / ts;
    tilePos.y = static_cast<int>(shape.getPosition().y) / ts;

    prevTile = tilePos;
    nextTile = findNextTile();
}
void Enemy::applyWaveSpeed(float multiplier) {
    speed = baseSpeed * multiplier;
}
void Enemy::setInfestCallback(InfestCallback cb) {
    infestCallback = cb;
}

void Enemy::setBossCallback(BossAbilityCallback cb) {
    bossCallback = cb;
}
void Enemy::setBurnCallback(BurnCallback cb) {
    burnCallback = cb;
}
EnemyStatus Enemy::getStatus() const {
    EnemyStatus s;
    s.hp = health;
    s.type = type;
    s.rage = rage;
    return s;
}

void Enemy::setStatus(const EnemyStatus& s) {
    health = s.hp;
    type = s.type;
    rage = s.rage;

    if (rage)
        speed = baseSpeed * 1.8f;
}



