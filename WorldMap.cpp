#include "WorldMap.h"

void WorldMap::init() {
    nodes.clear();

    WorldMapNode n;

    n.id = "grasslands";
    n.rect = sf::FloatRect(
        sf::Vector2f(100.f, 300.f),
        sf::Vector2f(120.f, 80.f)
    );
    nodes.push_back(n);

    n.id = "desert";
    n.rect = sf::FloatRect(
        sf::Vector2f(300.f, 300.f),
        sf::Vector2f(120.f, 80.f)
    );
    nodes.push_back(n);

    n.id = "ice";
    n.rect = sf::FloatRect(
        sf::Vector2f(500.f, 300.f),
        sf::Vector2f(120.f, 80.f)
    );
    nodes.push_back(n);

    n.id = "volcano";
    n.rect = sf::FloatRect(
        sf::Vector2f(700.f, 300.f),
        sf::Vector2f(120.f, 80.f)
    );
    nodes.push_back(n);
}

void WorldMap::draw(sf::RenderWindow& win, const AdventureMode& adv) const {
    for (const auto& node : nodes) {
        sf::RectangleShape r;

        r.setPosition(node.rect.position);
        r.setSize(node.rect.size);

        if (!adv.isUnlocked(node.id))
            r.setFillColor(sf::Color(80, 80, 80));
        else if (adv.isCompleted(node.id))
            r.setFillColor(sf::Color(60, 160, 60));
        else
            r.setFillColor(sf::Color(200, 200, 60));

        win.draw(r);
    }
}





bool WorldMap::handleClick(
    sf::Vector2f pos,
    AdventureMode& adv,
    std::string& outNodeId
) {
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].rect.contains(pos) && adv.isUnlocked(nodes[i].id)) {
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
