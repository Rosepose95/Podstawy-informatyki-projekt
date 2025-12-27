#include "Enemy.h"

Enemy::Enemy(int h, float startX, float startY)
    : health(h), maxHealth(h) {    //dodatek życia
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition({ startX, startY }); //zmiana zapisu
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
#include <algorithm> // Do rysowania paska zdrowia

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);

    float ratio = static_cast<float>(health) / maxHealth;
    ratio = std::clamp(ratio, 0.f, 1.f);

    sf::Vector2f pos = shape.getPosition();

    sf::RectangleShape backBar({ 40.f, 5.f });
    backBar.setFillColor(sf::Color::Red);
    backBar.setPosition({ pos.x, pos.y - 10.f });

    sf::RectangleShape hpBar({ 40.f * ratio, 5.f });
    hpBar.setFillColor(sf::Color::Green);
    hpBar.setPosition({ pos.x, pos.y - 10.f });

    window.draw(backBar);
    window.draw(hpBar);
}

sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}

float Enemy::getRadius() const { //metody do paska
    return shape.getRadius();
}
int Enemy::getMaxHealth() const {
    return maxHealth;
}
