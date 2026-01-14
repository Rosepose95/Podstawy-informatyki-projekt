#include "Bullet.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f start, sf::Vector2f target, int dmg) //dodatek do konstruktora
    : speed(300.f), lifetime(3.f), damage(dmg), dead(false) {

    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::Black);
    shape.setPosition(start);

    sf::Vector2f dir = target - start;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len != 0.f)
        direction = dir / len;
    else
        direction = { 0.f, 0.f };
}

void Bullet::update(float dt) {
    if (dead) return;
    shape.move(direction * speed * dt);
    lifetime -= dt;
}


void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

bool Bullet::isDead() const { //dodałam parę metod, by pociski działały poprawnie
    return dead || lifetime <= 0.f;
}

void Bullet::kill() {
    dead = true;
}

sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}

int Bullet::getDamage() const {
    return damage;
}







