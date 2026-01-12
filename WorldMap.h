#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AdventureMode.h"
struct WorldMapNode {
    std::string id;
    sf::FloatRect rect;
};

class WorldMap {
public:
    void init();
    void draw(sf::RenderWindow& win, const AdventureMode& adv) const;
    bool handleClick(sf::Vector2f pos, AdventureMode& adv, std::string& outNodeId);
    std::string getNodeAt(sf::Vector2f pos) const;
private:
    std::vector<WorldMapNode> nodes;
};
