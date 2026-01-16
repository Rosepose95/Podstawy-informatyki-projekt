#include "WorldMap.h"
#include <cstdint>
#include"Map.h"
#include <map>
#include "Game.h"
#include "Enemy.h"
void WorldMap::init() {
    nodes.clear();
    background.setSize({ 1240.f, 840.f });
    background.setFillColor(sf::Color(0, 0, 0, 150));
    font.openFromFile("assets/ArialMT.ttf");
   
    std::vector<std::string> ids = { "grasslands", "forest", "ice"};

    float cx = 1240.f / 2.f;
    float cy = 840.f / 2.f;
    float spacing = 180.f;

    for (int i = 0; i < ids.size(); ++i) {
        WorldMapNode n;
        n.id = ids[i];
        n.rect = {
            { cx + (i - 1.5f) * spacing, cy },
            { 140.f, 90.f }
        };
        nodes.push_back(n);
    }
    unlockAnims.assign(nodes.size(), 0.f);

    links.clear();
    links.push_back({ 0, 1 }); // grass -> forest
    links.push_back({ 1, 2 }); // forest -> ice
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].id == "grasslands")
            unlockAnims[i] = 1.f; // startowy node widoczny
    }



}
void WorldMap::update(const AdventureMode& adv, float dt) {
    

    for (size_t i = 0; i < nodes.size(); ++i) {
        if (adv.isUnlocked(nodes[i].id) && unlockAnims[i] == 0.f)
            unlockAnims[i] = 0.01f; // start animacji

        if (adv.isUnlocked(nodes[i].id) && unlockAnims[i] < 1.f) {
            unlockAnims[i] = std::min(1.f, unlockAnims[i] + dt * 3.f);
        }
    }
}

void WorldMap::draw(sf::RenderWindow& win, const AdventureMode& adv) const {
    win.draw(background);
    for (const auto& l : links) {
        sf::Vector2f a = nodes[l.from].rect.position + nodes[l.from].rect.size / 2.f;
        sf::Vector2f b = nodes[l.to].rect.position + nodes[l.to].rect.size / 2.f;

        sf::Vertex line[] = {
            sf::Vertex(a, sf::Color(100,100,100)),
            sf::Vertex(b, sf::Color(100,100,100))
        };
        win.draw(line, 2, sf::PrimitiveType::Lines);

    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        const WorldMapNode& node = nodes[i];
        float anim = adv.isUnlocked(node.id) ? std::max(0.1f, unlockAnims[i]) : 1.f;
        ;

        sf::RectangleShape r;

        r.setPosition(node.rect.position);
        r.setSize(node.rect.size);

        // Kolory w zależności od statusu
        if (!adv.isUnlocked(node.id)) {
            r.setFillColor(sf::Color(80, 80, 80));
            r.setOutlineThickness(3.f);
            r.setOutlineColor(sf::Color::Black);
        }
        else if (adv.isCompleted(node.id)) {
            r.setFillColor(sf::Color(60, 160, 60));
            r.setOutlineThickness(3.f);
            r.setOutlineColor(sf::Color::Black);
        }
        else {
            r.setFillColor(sf::Color(200, 200, 60));
            r.setOutlineThickness(3.f);
            r.setOutlineColor(sf::Color::Black);
        }

        sf::Vector2f mouse = win.mapPixelToCoords(sf::Mouse::getPosition(win));
        if (node.rect.contains(mouse) && adv.isUnlocked(node.id)) {
            r.setOutlineThickness(5.f);
            r.setOutlineColor(sf::Color::White);
        }
        r.setOrigin(node.rect.size / 2.f);
        r.setPosition(node.rect.position + node.rect.size / 2.f);
        r.setScale({ anim, anim });

        sf::Color c = r.getFillColor();
        c.a = static_cast<uint8_t>(255 * anim);
        r.setFillColor(c);

       
        win.draw(r);
        sf::Text label(font);
        label.setString(node.id);
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::Black);

        auto b = label.getLocalBounds();
        label.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        label.setPosition({
            node.rect.position.x + node.rect.size.x / 2.f,
            node.rect.position.y + node.rect.size.y / 2.f
            });

        win.draw(label);
    }
}

bool WorldMap::handleClick(
    sf::Vector2f pos,
    AdventureMode& adv,
    std::string& outNodeId
) {
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].rect.contains(pos) &&
            adv.isUnlocked(nodes[i].id) &&
            !adv.isCompleted(nodes[i].id)) {
            outNodeId = nodes[i].id;
            return true;
        }
        

    }
    return false;
}

std::string WorldMap::getNodeAt(sf::Vector2f pos) const {
    for (const auto& n : nodes) {
        if (n.rect.contains(pos))
            return n.id;
    }
    return "";
}
