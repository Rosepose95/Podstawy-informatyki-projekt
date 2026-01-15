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
		baseColor = shape.getFillColor();
        break;

    case EnemyType::Fast:
        baseSpeed = 140.f;
        shape.setFillColor(sf::Color::Yellow);
        baseColor = shape.getFillColor();
        break;

    case EnemyType::Infestor:
        baseSpeed = 60.f;
        health = h * 2;
        shape.setFillColor(sf::Color(150, 0, 0));
        baseColor = shape.getFillColor();
        break;
    case EnemyType::MeadowBoss:
        isBossEnemy = true; // <<< BRAKOWAŁO
        baseSpeed = 40.f;
        maxHealth = h * 10;
        health = maxHealth;
        shape.setRadius(28.f);
        shape.setOrigin({ 28.f, 28.f });
        shape.setFillColor(sf::Color(80, 0, 120));
        baseColor = shape.getFillColor();
        break;

    case EnemyType::Fireball:
        baseSpeed = 165.f;
        maxHealth = h * 0.6f;
        health = maxHealth;
        shape.setRadius(12.f);
        shape.setOrigin({ 12.f, 12.f });
        shape.setFillColor(sf::Color(255, 140, 40));
        baseColor = shape.getFillColor();
        break;

    case EnemyType::Flame:
        baseSpeed = 55.f;
        maxHealth = h * 0.8f;
        health = maxHealth;
        shape.setRadius(14.f);
        shape.setOrigin({ 14.f, 14.f });
        shape.setFillColor(sf::Color(220, 80, 20));
        baseColor = shape.getFillColor();
        break;
    case EnemyType::FireBoss:
        isBossEnemy = true;
        baseSpeed = 35.f;
        maxHealth = h * 12;
        health = maxHealth;
        shape.setRadius(30.f);
        shape.setOrigin({ 30.f, 30.f });
        shape.setFillColor(sf::Color(200, 50, 0));
        break;
    case EnemyType::Crusher:
        baseSpeed = 45.f;
        maxHealth = h * 3;
        health = maxHealth;
        shape.setRadius(22.f);
        shape.setOrigin({ 22.f, 22.f });
        shape.setFillColor(sf::Color(140, 50, 30));
        break;
    case EnemyType::IceShard:
        baseSpeed = 150.f;
        maxHealth = h * 0.7f;
        health = maxHealth;
        shape.setRadius(12.f);
        shape.setOrigin({ 12.f, 12.f });
        shape.setFillColor(sf::Color(150, 200, 255));
        break;
    case EnemyType::FrostWalker:
        baseSpeed = 45.f;
        maxHealth = h * 3;
        health = maxHealth;
        shape.setRadius(24.f);
        shape.setOrigin({ 24.f, 24.f });
        shape.setFillColor(sf::Color(100, 140, 200));
        baseColor = shape.getFillColor();
        break;

    case EnemyType::IceBoss:
        isBossEnemy = true;
        baseSpeed = 30.f;
        maxHealth = h * 14;
        health = maxHealth;
        shape.setRadius(32.f);
        shape.setOrigin({ 32.f, 32.f });
        shape.setFillColor(sf::Color(180, 220, 255));
        baseColor = shape.getFillColor();
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
    // --- ICE TIMERS ---
    if (slowTimer > 0.f) {
        slowTimer -= dt;
        if (slowTimer <= 0.f)
            slowFactor = 1.f;
    }

    if (freezeTimer > 0.f) {
        freezeTimer -= dt;
        if (freezeTimer <= 0.f) {
            frozen = false;
        }
    }

    if (isBossEnemy) {
        bossSkillTimer += dt;

        // faza rage
        if (!rage && health <= maxHealth * 0.5f) {
            rage = true;
            speed = baseSpeed * 1.8f;
            shape.setFillColor(sf::Color(255, 80, 80));
        }


        // warning + skill
        if (!warningActive) {
            warningActive = true;
            warningClock.restart();
            warningPos = getPosition();
        }

        if (warningClock.getElapsedTime().asSeconds() >= 0.8f) {
            warningActive = false;

            if (bossCallback)
                bossCallback(warningPos);
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
    if (specialTimer > 0.f)
        specialTimer -= dt;

    if (nextTile == tilePos)
        nextTile = findNextTile();

    sf::Vector2f target = tileCenter(nextTile);
    sf::Vector2f pos = shape.getPosition();

    sf::Vector2f toTarget = target - pos;
    float dist = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
    float finalSpeed = speed * slowFactor;
    if (frozen)
        finalSpeed = 0.f;

    float moveDist = finalSpeed * dt;

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

    // --- FIRE DAMAGE OVER TIME ---
    if (burnCallback && type != EnemyType::IceBoss) {

        burnTimer += dt;

        float interval = 0.f;
        float radius = 0.f;
        int stacks = 1;

        switch (type) {
        case EnemyType::Flame:
            interval = 1.5f;
            radius = 45.f;
            break;

        case EnemyType::Fireball:
            interval = 4.5f;
            radius = 90.f;
            break;
        case EnemyType::Crusher:
            interval = 2.5f;
            radius = 60.f;
            break;
        case EnemyType::FireBoss:
            interval = 2.0f;
            radius = 100.f;
            stacks = 2;
            break;

        default:
            break;
        }
        if (type == EnemyType::IceBoss && bossAbilityCallback) {
    float hp = (float)health / maxHealth;

    if (hp <= 0.75f && bossPhase < 1) {
        bossPhase = 1;
        bossAbilityCallback(getPosition(), BossAbility::IceSlow);
    }
    else if (hp <= 0.5f && bossPhase < 2) {
        bossPhase = 2;
        bossAbilityCallback(getPosition(), BossAbility::IceFreeze);
    }
    else if (hp <= 0.25f && bossPhase < 3) {
        bossPhase = 3;
        bossAbilityCallback(getPosition(), BossAbility::IceShatter);
    }
}


        if (interval > 0.f && burnTimer >= interval) {
            burnTimer = 0.f;
            burnCallback(getPosition(), radius, stacks);
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
	if (type == EnemyType::IceBoss) {
    sf::CircleShape aura(45.f);
    aura.setOrigin({45.f, 45.f});
    aura.setPosition(getPosition());
    aura.setFillColor(sf::Color(180,220,255,60));
    window.draw(aura);
}

   

    float ratio = static_cast<float>(health) / maxHealth;
    ratio = std::clamp(ratio, 0.f, 1.f);
    
    float r = shape.getRadius();
    sf::Vector2f posi = shape.getPosition();

    sf::CircleShape iceOverlay(r + 2.f);
    iceOverlay.setOrigin({ r + 2.f, r + 2.f });
    iceOverlay.setPosition(posi);

    iceOverlay.setFillColor(sf::Color(200, 240, 255, 120));
    if (frozen) {
        sf::CircleShape ice(shape.getRadius() + 6);
        ice.setOrigin({ shape.getRadius() + 6, shape.getRadius() + 6 });
        ice.setPosition(shape.getPosition());
        ice.setFillColor(sf::Color(180, 220, 255, 120));
        window.draw(ice);
        window.draw(iceOverlay);
    }
    window.draw(shape);
    sf::Vector2f pos = shape.getPosition();
    if (warningActive) {
        sf::CircleShape warn(60.f);
        warn.setOrigin({ 60.f, 60.f });
        warn.setPosition(warningPos);
        warn.setFillColor(sf::Color(255, 100, 0, 80));
        window.draw(warn);
    }

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
    if (isBossEnemy)
        return 5;
    if (type == EnemyType::Crusher)
        return 2;
    return 1;
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
void Enemy::applySlow(float factor, float duration)
{
    slowFactor = std::min(slowFactor, factor);
    slowTimer = std::max(slowTimer, duration);
}

void Enemy::applyFreeze(float duration)
{
    frozen = true;
    freezeTimer = std::max(freezeTimer, duration);
}
void Enemy::setBossAbilityCallback(BossAbilityCallback cb) {
    bossAbilityCallback = cb;
}


