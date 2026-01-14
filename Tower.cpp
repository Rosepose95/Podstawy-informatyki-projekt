#include "Tower.h"
#include <cmath>

Tower::Tower(int dmg, float x, float y, int Towertype)
    : damage(dmg),
    cooldown(0.5f),
    timeSinceLastShot(0.f),
    level(1),
    range(150.f),
    type(Towertype)
{
    shape.setSize({ 40.f, 40.f });
    shape.setOrigin({ 20.f, 20.f });
    shape.setPosition({ x, y });

    if (type == 0) { // Wie¿a podstawowa
        shape.setFillColor(sf::Color::Blue);
        damage = 15;
        range = 150.f;
        cooldown = 0.5f;
    }
    else if (type == 1) { // Wie¿a ulepszona
        shape.setFillColor(sf::Color::Yellow);
        damage = 30;
        range = 180.f;
        cooldown = 0.4f;
    }
    else if (type >= 2) { // Wie¿a MAX
        shape.setFillColor(sf::Color::Red);
        damage = 60;
        range = 220.f;
        cooldown = 0.3f;
    }
}


sf::Vector2f Tower::getPosition() const { //nowa funkcja
    return shape.getPosition();
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

        sf::Vector2f diff = e.getPosition() - shape.getPosition();
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
        shape.getPosition(),
        target->getPosition(),
        damage
    );
}

void Tower::upgrade() {
    level++;
    damage += 10;
    range += 20.f;
    cooldown *= 0.85f;
}

void Tower::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}




