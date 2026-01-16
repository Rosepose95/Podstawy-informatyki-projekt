#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AdventureMode.h"
struct WorldMapNode {
    std::string id;
    sf::FloatRect rect;
    float unlockAnim = 0.f; 
};
struct WorldMapLink {
    int from;
    int to;
};



class WorldMap {
public:
    void init();
    void update(const AdventureMode& adv, float dt);
    void draw(sf::RenderWindow& win, const AdventureMode& adv) const;
    bool handleClick(sf::Vector2f pos, AdventureMode& adv, std::string& outNodeId);
    std::string getNodeAt(sf::Vector2f pos) const;
private:
    std::vector<WorldMapNode> nodes;
    sf::RectangleShape background;
    sf::Font font;
    std::vector<float> unlockAnims;
    std::vector<WorldMapLink> links;
   

};
