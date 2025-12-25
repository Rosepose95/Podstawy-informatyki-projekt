#include "Tower.h"
#include <cmath>

Tower::Tower(int dmg, float x, float y)
    : damage(dmg),
    range(150.f),
    cooldown(0.5f),
    timeSinceLastShot(0.f)
{
    shape.setSize(sf::Vector2f(40.f, 40.f));
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(sf::Vector2f(x, y));
}


void Tower::updateAttack(Enemy& enemy, float dt,
    std::vector<Bullet>& bullets) {

    timeSinceLastShot += dt;
    if (timeSinceLastShot < cooldown)
        return;

    sf::Vector2f start =
        shape.getPosition() + shape.getSize() / 2.f;

    bullets.emplace_back(start, enemy.getPosition());

    enemy.takeDamage(damage);
    timeSinceLastShot = 0.f;
}



void Tower::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}
