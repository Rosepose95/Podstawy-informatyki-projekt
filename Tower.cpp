#include "Tower.h"
#include <cmath>
#include <iostream>

Tower::Tower(int dmg, float x, float y) //zmiana inicjatora
    : damage(dmg),
    cooldown(0.5f),
    timeSinceLastShot(0.f),
    level(1),
    range(150.f)
{
    baseDamage = dmg;
    damage = dmg;
    baseCooldown = cooldown;

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
    if (isDestroyed())
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

    float fireRateMult = getFireRateMultiplier();
    if (fireRateMult <= 0.f)
        return;

    if (timeSinceLastShot < baseCooldown / fireRateMult)
        return;


    timeSinceLastShot = 0.f;
    int finalDamage = static_cast<int>(baseDamage * getDamageMultiplier());

    if (finalDamage <= 0)
        return; // wieża martwa funkcjonalnie

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

    if (infestationStacks > 0) {
        sf::RectangleShape barBack({ 30.f, 4.f });
        barBack.setFillColor(sf::Color(50, 50, 50));
        barBack.setPosition({ shape.getPosition().x - 15.f,
            shape.getPosition().y - 28.f });



        float ratio = 1.f - infestationStacks / float(MAX_INFESTATION);
        ratio = std::clamp(ratio, 0.f, 1.f);


        sf::RectangleShape bar({ 30.f * ratio, 4.f });


        sf::CircleShape infestIcon(5.f);
        infestIcon.setFillColor(sf::Color(60, 200, 60));
        infestIcon.setPosition({
            shape.getPosition().x + 4.f,
          std::max(0.f, shape.getPosition().y - 42.f)
            });
        window.draw(infestIcon);
        // kolor zależny od infestation
        if (ratio > 0.6f)
            bar.setFillColor(sf::Color::Green);
        else if (ratio > 0.3f)
            bar.setFillColor(sf::Color::Yellow);
        else
            bar.setFillColor(sf::Color::Red);

        bar.setPosition(barBack.getPosition());

        window.draw(barBack);
        window.draw(bar);
    }
    if (burnStacks > 0) {
        sf::CircleShape burnIcon(5.f);
        burnIcon.setFillColor(
            sf::Color(255, 80 + burnStacks * 20, 0)
        );
        burnIcon.setPosition({
            shape.getPosition().x - 14.f,
            shape.getPosition().y - 42.f
            });
        window.draw(burnIcon);


    }




}
void Tower::addInfestation(int stacks) {
    infestationStacks += stacks;
    infestationStacks = std::min(infestationStacks, MAX_INFESTATION);
}

bool Tower::isDestroyed() const {
    return infestationStacks >= MAX_INFESTATION;
}

int Tower::getInfestationStacks() const {
    return infestationStacks;
}

float Tower::getDamageMultiplier() const {
    // 10% mniej dmg za stack
    float mult = 1.f - 0.1f * infestationStacks;
    return std::max(0.f, mult);
}
float Tower::getFireRateMultiplier() const {
    return std::max(0.f, 1.f - 0.25f * burnStacks);
}
void Tower::addBurn(int stacks) {
    burnStacks += stacks;
    burnStacks = std::min(burnStacks, MAX_BURN);
}

int Tower::getBurnStacks() const {
    return burnStacks;
}

void Tower::forceDestroy() {
    destroyed = true;
}

void Tower::setPosition(sf::Vector2f pos) {
    shape.setPosition(pos);
}

TowerStatus Tower::getStatus() const {
    TowerStatus s;
    s.infestation = infestationStacks;
    s.burn = burnStacks;
    s.destroyed = destroyed;
    return s;
}

void Tower::setStatus(const TowerStatus& s) {
    infestationStacks = s.infestation;
    burnStacks = s.burn;
    destroyed = s.destroyed;
}

void Tower::setInfestationStacks(int v) { infestationStacks = v; }
void Tower::setBurnStacks(int v) { burnStacks = v; }
void Tower::setDestroyed(bool v) { destroyed = v; }
