#include "Map.h"

Map::Map() {
    grid = {
        {'.','.','.','.','.','.'},
        {'#','#','#','#','.','.'},
        {'.','.','.','#','.','.'},
        {'.','.','.','#','#','#'}
    };
}

char Map::getTile(int x, int y) const {
    return grid[y][x];
}

int Map::getWidth() const {
    return grid[0].size();
}

int Map::getHeight() const {
    return grid.size();
}
