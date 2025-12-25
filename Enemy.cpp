#include "Enemy.h"

Enemy::Enemy(int hp, float x, float y)
    : health(hp) {
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(sf::Vector2f(x, y));
}

void Enemy::move(float dx, float dy) {
    shape.move(sf::Vector2f(dx, dy));
}

void Enemy::takeDamage(int dmg) {
    health -= dmg;
}

bool Enemy::isDead() const {
    return health <= 0;
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}
