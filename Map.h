#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SFML/Graphics.hpp>

class Map {
public:
    Map();
    void draw(sf::RenderWindow& window);

    char getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    int tileSize = 40; // Rozmiar kafelka (800/20 = 40, 640/16 = 40)

private:

    std::vector<std::vector<char>> grid;
};

#endif
