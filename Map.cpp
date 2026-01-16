#include "Map.h"
#include <string>
#include <vector>


char Map::getTile(int x, int y) const { return grid[y][x]; }
int Map::getWidth() const { return grid[0].size(); }
int Map::getHeight() const { return grid.size(); }

Map::Map() {
    loadMap();
}
void Map::loadMap() {
    std::vector<std::string> layout = {
        "..........#...............#....", // 0  
        "..........#...............#....", // 1
        "#######...###########.....#....", // 2
        "......#.............#.....#....", // 3
        "...####.............#.....#....", // 4
        "...#................#######....", // 5
        "...#...########.....#..........", // 6
        "...#...#......#.....#..........", // 7
        "...#####......#######..........", // 8
        "..............#................", // 9  
        ".............***...............", // 10 
        "..###########***......#####....", // 11     
        "..#..........***......#...#....", // 12
        "..#...........#.......#...#....", // 13
        "..#...........#.......#...#....", // 14
        "..#####.......#.......#...#....", // 15 
        "......#.......#.......#...#....", // 16
        "......#.......#.......#...#....", // 17
        "#######.......#########...#....", // 18
        "..........................#....", // 19
        "..........................#...."  // 20 
    };
    int rows = layout.size();           //pion liczy znaki
    int cols = layout[0].size();        //poziom liczy znaki

    grid.assign(rows, std::vector<char>(cols)); //ustawiamy rozmiar siatki

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = layout[i][j];
            if (layout[i][j] == '*') { // meta
                baseTile = { j, i };
            }
            if (layout[i][j] == '#') {//można to w sumie w jedneym ifie zrobić 
                if (j == 0 || i == 0 || i == rows - 1) {
                    spawnPoints.push_back({ j, i });
                }
            }

        }
    }
}

void Map::draw(sf::RenderWindow& window) {
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            sf::RectangleShape tile(sf::Vector2f(static_cast<float>(tileSize), static_cast<float>(tileSize)));
            tile.setPosition({ static_cast<float>(j * tileSize), static_cast<float>(i * tileSize) });

            // Dobór koloru
            if (grid[i][j] == '#') {
                tile.setFillColor(sf::Color(100, 100, 100)); // Droga (szary)
            }
            else if (grid[i][j] == '.') {
                tile.setFillColor(sf::Color(34, 139, 34));  // Trawa (zielony)
            }
            else if (grid[i][j] == '*') {
                tile.setFillColor(sf::Color(255, 100, 0));  // meta (pomaranczowy)
            }
            else if (grid[i][j] == 'T') {
                tile.setFillColor(sf::Color::Blue); // dla editora wieza
            }


            // Siatka pomocnicza
            tile.setOutlineThickness(1.f);
            tile.setOutlineColor(sf::Color(0, 0, 0, 50));

            window.draw(tile);
        }
    }
}
//rysowanie mnapy
void Map::setTile(int x, int y, char type) {
    if (y >= 0 && y < grid.size() && x >= 0 && x < grid[0].size()) {
        grid[y][x] = type;
    }
}
//wypelnienie nowej mapy trawa
void Map::clearMap() {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++)
            grid[i][j] = '.';
    }
}
//sprawdzanie mapy gdzie i co gracz postawil 
void Map::refreshLogic() {
    spawnPoints.clear();

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {

            if (grid[i][j] == '#' && (j == 0 || i == 0 || i == grid.size() - 1 || j == grid[i].size() - 1)) {
                spawnPoints.push_back({ j, i });
            }
            if (grid[i][j] == '*') {
                baseTile = { j, i };
            }
        }
    }
}
