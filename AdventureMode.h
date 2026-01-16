#pragma once
#include <string>
#include <unordered_map>
#include "Biomes.h"
struct AdventureNode {
    std::string id;
    Biomes biome;
    bool unlocked;
    bool completed;
    bool boss;
};


class AdventureMode {
public:
    void completeNode(const std::string& id);
    void setNodeState(const std::string& id, bool unlocked, bool completed);

    void init();
    bool isUnlocked(const std::string& id) const;
    bool isCompleted(const std::string& id) const;
    std::string getNextNode(const std::string& id) const;
    int getRewardGold(const std::string& nodeId) const;
    bool isBossNode(const std::string& id) const;
    Biomes getBiomeForNode(const std::string& id) const;


private:
    std::unordered_map<std::string, AdventureNode> nodes;
	int baseGoldReward = 25;
};

