#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

std::array<sf::Texture, 4> Enemy::s_textures;
bool Enemy::s_texturesLoaded = false;

void Enemy::loadTextures()
{
    if (s_texturesLoaded) return;

    if (!s_textures[0].loadFromFile("assets/enemy_normal.png")) throw std::runtime_error("No assets/enemy_normal.png");
    if (!s_textures[1].loadFromFile("assets/enemy_fast.png"))   throw std::runtime_error("No assets/enemy_fast.png");
    if (!s_textures[2].loadFromFile("assets/enemy_tank.png"))   throw std::runtime_error("No assets/enemy_tank.png");
    if (!s_textures[3].loadFromFile("assets/enemy_boss.png"))   throw std::runtime_error("No assets/enemy_boss.png");

    for (auto& t : s_textures) t.setSmooth(false);

    s_texturesLoaded = true;
}

Enemy::Enemy(int h, float startX, float startY, EnemyType t)
    : health(h), maxHealth(h), type(t) { //dodatek typu enemy i życia dla bossa

    loadTextures();

    shape.setRadius(15.f);
    shape.setOrigin({ 15.f, 15.f });
    shape.setPosition({ startX, startY });

    applyType(type, h);

    tilePos = { int(startX) / 40, int(startY) / 40 };
    prevTile = tilePos;
    nextTile = tilePos;

}

    void Enemy::applyType(EnemyType t, int h)
    {
        type = t;

        int texIndex = 0;
        switch (type) {
        case EnemyType::Normal:
            speed = 100.f;
            texIndex = 0;
            shape.setRadius(15.f);
            shape.setOrigin({ 15.f, 15.f });
            maxHealth = health = h;
            break;

        case EnemyType::Fast:
            speed = 160.f;
            texIndex = 1;
            shape.setRadius(15.f);
            shape.setOrigin({ 15.f, 15.f });
            maxHealth = health = h;
            break;

        case EnemyType::Tank:
            speed = 60.f;
            texIndex = 2;
            shape.setRadius(17.f);
            shape.setOrigin({ 17.f, 17.f });
            maxHealth = health = h;
            break;

        case EnemyType::Boss:
            speed = 40.f;
            texIndex = 3;
            maxHealth = health = h * 5;
            shape.setRadius(28.f);
            shape.setOrigin({ 28.f, 28.f });
            break;
        }
    

    sprite.setTexture(s_textures[texIndex], true);

    auto s = sprite.getTexture().getSize();
    sprite.setOrigin({ s.x / 2.f, s.y / 2.f });

    float target = 2.f * shape.getRadius();
    sprite.setScale({ target / (float)s.x, target / (float)s.y });

    sprite.setPosition(shape.getPosition());
}


void Enemy::setMap(const Map* m) { //nowe fukcja do pathing
    map = m;
    int ts = map->tileSize;

    tilePos.x = (int)shape.getPosition().x / ts;
    tilePos.y = (int)shape.getPosition().y / ts;

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

    if (dist > 0.0001f) {
        sf::Vector2f dir = toTarget / dist;

        // obrót sprite zgodnie z kierunkiem ruchu
        float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
        sprite.setRotation(sf::degrees(angle + ROT_OFFSET_DEG));

        if (dist <= moveDist) {
            shape.setPosition(target);
            sprite.setPosition(target);

            prevTile = tilePos;
            tilePos = nextTile;
            nextTile = findNextTile();
            return;
        }
        shape.move(dir * moveDist);
        sprite.setPosition(shape.getPosition());
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
    window.draw(sprite);

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
int Enemy::getLifeDamage() const {
    if (type == EnemyType::Boss)
        return 5;   // boss zabiera 5 żyć
    return 1;       // normalny wróg
}



