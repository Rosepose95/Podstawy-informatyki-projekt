#include "Bullet.h"
#include <cmath>
#include <stdexcept>
#include <algorithm> 

std::array<sf::Texture, 3> Bullet::s_textures;
bool Bullet::s_loaded = false;

void Bullet::loadTextures() {
    if (s_loaded) return;

    if (!s_textures[0].loadFromFile("assets/arrow_1.png")) throw std::runtime_error("No assets/arrow_1.png");
    if (!s_textures[1].loadFromFile("assets/arrow_2.png")) throw std::runtime_error("No assets/arrow_2.png");
    if (!s_textures[2].loadFromFile("assets/arrow_3.png")) throw std::runtime_error("No assets/arrow_3.png");

    for (auto& t : s_textures) t.setSmooth(false);

    s_loaded = true;
}

Bullet::Bullet(sf::Vector2f start, sf::Vector2f target, int dmg, int bulletType)
    : damage(dmg) {

    loadTextures();

    sf::Vector2f dir = target - start;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.0001f) direction = dir / len;

    body.setTexture(&s_textures[bulletType], true);

    auto sz = s_textures[bulletType].getSize();


    float targetMax = 26.f;
    float maxDim = (float)std::max(sz.x, sz.y);
    float scale = (maxDim > 0.f) ? (targetMax / maxDim) : 1.f;

    body.setSize({ (float)sz.x, (float)sz.y });
    body.setOrigin({ (float)sz.x / 2.f, (float)sz.y / 2.f });
    body.setScale({ scale, scale });
    body.setPosition(start);
  
  
    if (len > 0.0001f) {
        float angleDeg = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
        body.setRotation(sf::degrees(angleDeg + ROT_OFFSET_DEG));
    }




}


void Bullet::update(float dt) {
    if (dead) return;
    body.move(direction * speed * dt);
    lifetime -= dt;
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(body);
}

bool Bullet::isDead() const {
    return dead || lifetime <= 0.f;
}

void Bullet::kill() {
    dead = true;
}

sf::Vector2f Bullet::getPosition() const {
    return body.getPosition();
}

int Bullet::getDamage() const {
    return damage;
}








