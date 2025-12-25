#include "Bullet.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f start, sf::Vector2f target)
    : lifetime(2.f) {

    shape.setRadius(4.f);
    shape.setFillColor(sf::Color::Black);
    shape.setPosition(start);

    sf::Vector2f dir = target - start;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len != 0.f)
        dir /= len;

    velocity = dir * 300.f;
}

void Bullet::update(float dt) {
    shape.move(velocity * dt);
    lifetime -= dt;
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

bool Bullet::isDead() const {
    return lifetime <= 0.f;
}
