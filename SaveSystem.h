#pragma once
#include <string>

class Game;

namespace SaveSystem {

    void save(Game& game, int slot);
    bool load(Game& game, int slot);
    std::string getDescription(int slot);
    bool exists(int slot);

}
