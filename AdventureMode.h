#pragma once
#include <string>
#include <unordered_map>
struct AdventureNode {
    std::string id;
    bool unlocked = false;
    bool completed = false;
    bool boss = false;   
enum class Biomes {
    Meadow,
    Fire,
    Forest,
    Water,
    Ice
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
    Biomes getBiomeForNode(const std::string& nodeId);


private:
    std::unordered_map<std::string, AdventureNode> nodes;
};
