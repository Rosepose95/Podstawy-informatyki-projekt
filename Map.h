#ifndef MAP_H
#define MAP_H

#include <vector>

class Map {
public:
    Map();
    char getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

private:
    std::vector<std::vector<char>> grid;
};

#endif

