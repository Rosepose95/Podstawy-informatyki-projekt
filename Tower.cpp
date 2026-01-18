#include "Tower.h"
#include <cmath>

static int clampType(int t) {
    return t;
}

Tower::Tower(int dmg, float x, float y, int Towertype,
    const std::array<sf::Texture, 3>& tex, int tileSz)
    : damage(dmg),
    cooldown(0.5f),
    timeSinceLastShot(0.f),
    level(1),
    range(150.f),
    type(clampType(Towertype)),
    pos(x, y),
    sprite(tex[clampType(Towertype)]),   
    textures(&tex),
    tileSize(tileSz)
{
    sprite.setPosition(pos);
    applyType(type);
}


void Tower::applyType(int newType)
{
    type = clampType(newType);

     //STATY per typ
    if (type == 0) { // 0 (poczatkowa)
        damage = 15;
        range = 150.f;
        cooldown = 0.5f;
        level = 1;
    }
    else if (type == 1) { // 1
        damage = 30;
        range = 180.f;
        cooldown = 0.4f;
        level = 2;
    }
    else { // 2
        damage = 60;
        range = 220.f;
        cooldown = 0.3f;
        level = 3;
    }

    // GRAFIKA 
    if (textures && tileSize > 0) {
        sprite.setTexture((*textures)[type], true);

        auto s = (*textures)[type].getSize();
        sprite.setOrigin({ s.x / 2.f, (float)s.y });

        float targetW = 1.1f * tileSize;
        float targetH = 3.0f * tileSize;
        sprite.setScale({ targetW / s.x, targetH / s.y });
    }
}

    void Tower::setType(int newType)
    {
        applyType(newType);
    }





void Tower::updateAttack(      //zmiana, dodanie lepszej fizyki
    std::vector<Enemy>& enemies,
    float dt,
    std::vector<Bullet>& bullets
) {
    timeSinceLastShot += dt;

    Enemy* target = nullptr;
    float bestDist = range * range;

    for (auto& e : enemies) {
        if (e.isDead())
            continue;

        sf::Vector2f diff = e.getPosition() - sprite.getPosition();
        float dist2 = diff.x * diff.x + diff.y * diff.y;

        if (dist2 <= bestDist) {
            bestDist = dist2;
            target = &e;
        }
    }

    if (!target)
        return;

    if (timeSinceLastShot < cooldown)
        return;

    timeSinceLastShot = 0.f;

    bullets.emplace_back(
        sprite.getPosition(),
        target->getPosition(),
        damage,
        type
    );
}


void Tower::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}




