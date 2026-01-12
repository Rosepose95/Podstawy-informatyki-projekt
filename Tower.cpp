#include "Tower.h"
#include <cmath>

Tower::Tower(int dmg, float x, float y) //zmiana inicjatora
    : damage(dmg),
    cooldown(0.5f),
    timeSinceLastShot(0.f),
    level(1),
    range(150.f)
{
    baseDamage = damage;
    shape.setSize({ 40.f, 40.f });
    shape.setOrigin({ 20.f, 20.f });
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition({ x, y });
}


sf::Vector2f Tower::getPosition() const { //nowa funkcja
    return shape.getPosition();
}

void Tower::updateAttack(      //zmiana, dodanie lepszej fizyki
    std::vector<Enemy>& enemies,
    float dt,
    std::vector<Bullet>& bullets
) {
    if (destroyed || damage <= 0.f)
        return;

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

    int finalDamage = static_cast<int>(damage * getDamageMultiplier());

    if (finalDamage <= 0)
        return;

    bullets.emplace_back(
        shape.getPosition(),
        target->getPosition(),
        finalDamage
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
void Tower::addInfestation(int stacks) {
    infestationStacks += stacks;

    float reduction = 0.1f * infestationStacks; // 10% za stack
    reduction = std::min(reduction, 1.0f);      // max 100%

    damage = baseDamage * (1.f - reduction);

    if (damage <= 0.f) {
        damage = 0.f;
        destroyed = true;
    }
}


float Tower::getDamageMultiplier() const {
    return std::max(0.f, 1.f - infestationStacks * 0.1f);
}

bool Tower::isDestroyedByInfestation() const {
    return infestationStacks >= 10;
}
