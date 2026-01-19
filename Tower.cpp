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

void Tower::updateAttack(
    std::vector<Enemy>& enemies,
    float dt,
    std::vector<Bullet>& bullets
) {
    if (isDestroyed())
        return;

    timeSinceLastShot += dt;

    if (freezeTimer > 0.f) {
        freezeTimer -= dt;
        if (freezeTimer <= 0.f) {
            frozen = false;
        }
    }


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
        return;

    // Zwiększamy licznik strzałów
    shotsFired++;

    // Co 100 strzałów ustaw efekt Ice
    BulletEffect effect = (shotsFired % 50 == 0) ? BulletEffect::Ice : BulletEffect::Normal;

    bullets.emplace_back(
        shape.getPosition(),
        target->getPosition(),
        finalDamage,
        effect
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

    float barWidth = 40.f;
    float barHeight = 5.f;
    float spacing = 2.f;

    float barX = shape.getPosition().x - barWidth / 2.f;
    float barY = shape.getPosition().y - 30.f;

    auto drawBar = [&](int current, int max, sf::Color color) {
        if (max <= 0 || current <= 0)
            return;

        // tło
        sf::RectangleShape bg({ barWidth, barHeight });
        bg.setFillColor(sf::Color(40, 40, 40, 200));
        bg.setPosition({ barX, barY });
        window.draw(bg);

        // wypełnienie
        float ratio = static_cast<float>(current) / max;
        sf::RectangleShape fg({ barWidth * ratio, barHeight });
        fg.setFillColor(color);
        fg.setPosition({barX, barY});
        window.draw(fg);

        barY -= (barHeight + spacing); // 👈 przesuwamy w górę
        };

    // 🔥 Burn
    drawBar(burnStacks, MAX_BURN, sf::Color::Red);

    // ☣ Infestation
    drawBar(infestationStacks, MAX_INFESTATION, sf::Color(120, 0, 120));

    // 🐌 Slow
    drawBar(slowStacks, 5, sf::Color(150, 200, 255));

    // ❄ Freeze (czas)
    if (freezeTimer > 0.f) {
        int freezeValue = static_cast<int>(freezeTimer * 10);
        drawBar(freezeValue, 50, sf::Color(120, 180, 255));
    }
}

void Tower::addInfestation(int stacks) {
    infestationStacks += stacks;
    infestationStacks = std::min(infestationStacks, MAX_INFESTATION);
}

bool Tower::isDestroyed() const {
    return destroyed || infestationStacks >= MAX_INFESTATION;
}


int Tower::getInfestationStacks() const {
    return infestationStacks;
}

float Tower::getDamageMultiplier() const {
    // 10% mniej dmg za stack
    float mult = 1.f - 0.1f * infestationStacks;
    return std::max(0.f, mult);
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
    s.slow = slowStacks;
    s.freeze = freezeTimer;
    s.destroyed = destroyed;
    return s;
}

void Tower::setStatus(const TowerStatus& s) {
    infestationStacks = s.infestation;
    burnStacks = s.burn;
    slowStacks = s.slow;
    freezeTimer = s.freeze;
    destroyed = s.destroyed;
}
void Tower::setInfestationStacks(int v) { infestationStacks = v; }
void Tower::setBurnStacks(int v) { burnStacks = v; }
void Tower::setDestroyed(bool v) { destroyed = v; }
float Tower::getFireRateMultiplier() const {
    float slowMul = 1.f - 0.1f * slowStacks;
    return std::max(0.3f, slowMul);
}
void Tower::addSlow(int stacks) {
    slowStacks += stacks;
    slowStacks = std::min(slowStacks, 5);
}

void Tower::freeze(float time) {
    frozen = true;
    freezeTimer = std::max(freezeTimer, time);
}


bool Tower::isFrozen() const {
    return frozen;
}
