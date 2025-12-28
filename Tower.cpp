#include "Tower.h"
#include <cmath>
Tower::Tower(int dmg, float x, float y) //zmiana inicjatora
    : damage(dmg), cooldown(0.5f), timeSinceLastShot(0.f) {
    shape.setSize(sf::Vector2f(40.f, 40.f));
    shape.setOrigin({ 20.f, 20.f }); 
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition({ x, y });
}
sf::Vector2f Tower::getPosition() const { //nowa funkcja
    return shape.getPosition();
}
void Tower::updateAttack(Enemy& enemy, float dt, std::vector<Bullet>& bullets) { //zmiana, dodanie lepszej fizyki
    timeSinceLastShot += dt;

    if (timeSinceLastShot < cooldown)
        return;

     sf::Vector2f towerPos = shape.getPosition() + shape.getSize() / 2.f;
 sf::Vector2f enemyPos = enemy.getPosition();

 float dx = enemyPos.x - towerPos.x;
 float dy = enemyPos.y - towerPos.y;
 float dist2 = dx * dx + dy * dy;

 const float range = 200.f;

 if (dist2 <= range * range) {
     bullets.emplace_back(towerPos, enemyPos);
     timeSinceLastShot = 0.f;
 }
}

void Tower::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}


