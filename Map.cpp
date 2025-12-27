#include "Map.h"
#include <string>
#include <vector>


char Map::getTile(int x, int y) const { return grid[y][x]; }
int Map::getWidth() const { return grid[0].size(); }
int Map::getHeight() const { return grid.size(); }

Map::Map() {
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
            else {
                tile.setFillColor(sf::Color(255, 100, 0));  // meta (pomaranczowy)
            }

            // Siatka pomocnicza
            tile.setOutlineThickness(1.f);
            tile.setOutlineColor(sf::Color(0, 0, 0, 50));

            window.draw(tile);
        }
    }
}
