#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

std::array<sf::Texture, 4> Enemy::s_textures;
bool Enemy::s_Loaded = false;

void Enemy::loadTextures()
{
    if (s_Loaded) return;

    if (!s_textures[0].loadFromFile("assets/enemy_normal.png")) throw std::runtime_error("No assets/enemy_normal.png");
    if (!s_textures[1].loadFromFile("assets/enemy_fast.png"))   throw std::runtime_error("No assets/enemy_fast.png");
    if (!s_textures[2].loadFromFile("assets/enemy_tank.png"))   throw std::runtime_error("No assets/enemy_tank.png");
    if (!s_textures[3].loadFromFile("assets/enemy_boss.png"))   throw std::runtime_error("No assets/enemy_boss.png");

    for (auto& t : s_textures) t.setSmooth(false);

    s_Loaded = true;
}

Enemy::Enemy(int h, float startX, float startY, EnemyType t) {
    loadTextures();

    
    body.setPosition({ startX, startY });
    applyType(t, h);

   
    tilePos = { int(startX) / 40, int(startY) / 40 };
    prevTile = tilePos;
    nextTile = tilePos;
}

    void Enemy::applyType(EnemyType t, int baseHP)
    {
        type = t;

        int texIndex = 0;
        switch (type) {
        case EnemyType::Normal:
            speed = 100.f;
            radius = 25.f;
            maxHealth = health = baseHP;
            texIndex = 0;
            break;

        case EnemyType::Fast:
            speed = 160.f;
            radius = 22.f;
            maxHealth = health = baseHP;
            texIndex = 1;
            break;

        case EnemyType::Tank:
            speed = 60.f;
            radius = 30.f;
            maxHealth = health = baseHP;
            texIndex = 2;
            break;

        case EnemyType::Boss:
            speed = 40.f;
            radius = 35.f;
            maxHealth = health = baseHP * 5;
            texIndex = 3;
            break;
        }

        body.setSize({ 2.f * radius, 2.f * radius });
        body.setOrigin({ radius, radius });
        body.setTexture(&s_textures[texIndex], true);
    }



void Enemy::setMap(const Map* m) {
    map = m;
    if (!map) return;

    int ts = map->tileSize;

    sf::Vector2f pos = body.getPosition();
    tilePos.x = (int)(pos.x / (float)ts);
    tilePos.y = (int)(pos.y / (float)ts);

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

        float dx = (float)candidate.x - map->baseTile.x;
        float dy = (float)candidate.y - map->baseTile.y;
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


sf::Vector2f Enemy::getPosition() const {
    return body.getPosition();
}

sf::Vector2f Enemy::tileCenter(sf::Vector2i tile) const {
    float ts = static_cast<float>(map->tileSize);
    return { tile.x * ts + ts / 2.f, tile.y * ts + ts / 2.f };
}

void Enemy::update(float dt) {
    if (!map) return;
    if (reachedGoal()) return;

    sf::Vector2f target = tileCenter(nextTile);
    sf::Vector2f pos = body.getPosition();

    sf::Vector2f toTarget = target - pos;
    float dist = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
    float moveDist = speed * dt;

    if (dist <= 0.0001f) return;

    sf::Vector2f dir = { toTarget.x / dist, toTarget.y / dist };

    // obrót zgodnie z ruchem
    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    body.setRotation(sf::degrees(angleDeg + ROT_OFFSET_DEG));


    if (dist <= moveDist) {
        body.setPosition(target);

        prevTile = tilePos;
        tilePos = nextTile;
        nextTile = findNextTile();
        return;
    }

    body.move(dir * moveDist);
}

void Enemy::takeDamage(int dmg) { //zmiana
    health -= dmg;
    if (health < 0) health = 0;
}

bool Enemy::isDead() const {
    return health <= 0;
}

float Enemy::getRadius() const
{
    return radius;
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(body);

    float ratio = (maxHealth > 0) ? (float)health / (float)maxHealth : 0.f;
    ratio = std::clamp(ratio, 0.f, 1.f);

    sf::Vector2f pos = body.getPosition();

    sf::RectangleShape back({ 36.f, 5.f });
    back.setFillColor(sf::Color::Red);
    back.setPosition({ pos.x - 18.f, pos.y - radius - 12.f });

    sf::RectangleShape hp({ 36.f * ratio, 5.f });
    hp.setFillColor(sf::Color::Green);
    hp.setPosition(back.getPosition());

    window.draw(back);
    window.draw(hp);
}
int Enemy::getLifeDamage() const {
    {
        return (type == EnemyType::Boss) ? 5 : 1;       //boss zabiera 5 żyć
    }

}

