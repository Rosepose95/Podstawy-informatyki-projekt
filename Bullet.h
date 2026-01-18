#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <array>

class Bullet {
private:
    sf::RectangleShape body;
    sf::Vector2f direction{ 0.f, 0.f };

    float speed = 300.f;
    float lifetime = 3.f;
    int damage = 1;
    bool dead = false;

    static std::array<sf::Texture, 3> s_textures;
    static bool s_loaded;
    static void loadTextures();
    //rotacja w zaleznosci od poczatkowego polozenia grafiki
    static constexpr float ROT_OFFSET_DEG = +90.f; 

public:
    // bulletType: 0..2 (dla 3 wież)
    Bullet(sf::Vector2f start, sf::Vector2f target, int dmg, int bulletType = 0);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    bool isDead() const;
    void kill();

    sf::Vector2f getPosition() const;
    int getDamage() const;
};
#endif







