#include "AdventureMode.h"
#include "Game.h"
void AdventureMode::completeNode(const std::string& id) {
    auto& node = nodes[id];
    node.unlocked = true;
    node.completed = true;
}

void AdventureMode::setNodeState(
    const std::string& id,
    bool unlocked,
    bool completed
) {
    auto& node = nodes[id];
    node.unlocked = unlocked;
    node.completed = completed;
}

bool AdventureMode::isUnlocked(const std::string& id) const {
    auto it = nodes.find(id);
    return it != nodes.end() && it->second.unlocked;
}

bool AdventureMode::isCompleted(const std::string& id) const {
    auto it = nodes.find(id);
    return it != nodes.end() && it->second.completed;
}
std::string AdventureMode::getNextNode(const std::string& id) const {
    if (id == "grasslands") return "desert";
    if (id == "desert")     return "ice";
    if (id == "ice")        return "volcano";
    return "";

}
bool AdventureMode::isBossNode(const std::string& id) const
{
    auto it = nodes.find(id);
    if (it == nodes.end()) return false;
    return it->second.boss;
}

void AdventureMode::init() {
    nodes["grasslands"] = { "grasslands", Biomes::Meadow, true,  false, false };
    nodes["forest"]     = { "forest",     Biomes::Forest, false, false, false };
    nodes["ice"]        = { "ice",        Biomes::Ice,    false, false, true  };
    nodes["water"]      = { "water",      Biomes::Water,  false, false, false };
    
}

int AdventureMode::getRewardGold(const std::string& nodeId) const
{
    // prosta, bezpieczna logika na start
    if (nodeId == "grasslands") return 50;
    if (nodeId == "desert")     return 75;
    if (nodeId == "ice")        return 100;
    if (nodeId == "volcano")    return 150;

    return 25; // fallback
}
Biomes AdventureMode::getBiome(const std::string& id) {
    return nodes.at(id).biome;
}



