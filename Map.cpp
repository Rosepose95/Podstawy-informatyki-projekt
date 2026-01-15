#include "Map.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

char Map::getTile(int x, int y) const { return grid[y][x]; }
int Map::getWidth() const { return grid[0].size(); }
int Map::getHeight() const { return grid.size(); }

Map::Map() {
    std::srand((unsigned)std::time(nullptr));
    loadMap();
    loadTextures();
    texGrass.setSmooth(false);
    texPath.setSmooth(false);
    texStone.setSmooth(false);
    texWater.setSmooth(false);

    for (auto& t : texTrees) t.setSmooth(false);
}

void Map::loadTextures()
{
    if (texturesLoaded) return;

    if (!texGrass.loadFromFile("assets/grass_2.png")) throw std::runtime_error("No assets/grass_2.png");
    if (!texPath.loadFromFile("assets/path.png"))   throw std::runtime_error("No assets/path.png");
    if (!texStone.loadFromFile("assets/stone.png")) throw std::runtime_error("No assets/stone.png");
    if (!texWater.loadFromFile("assets/water_2.png")) throw std::runtime_error("No assets/water_2.png");
    
    if (!texTrees[0].loadFromFile("assets/tree_1.png")) throw std::runtime_error("No assets/tree_1.png");
    if (!texTrees[1].loadFromFile("assets/tree_2.png")) throw std::runtime_error("No assets/tree_2.png");
    if (!texTrees[2].loadFromFile("assets/tree_3.png")) throw std::runtime_error("No assets/tree_3.png");
    if (!texTrees[3].loadFromFile("assets/tree_4.png")) throw std::runtime_error("No assets/tree_4.png");

    if (!texSmallStones[0].loadFromFile("assets/smallStone_1.png")) throw std::runtime_error("No assets/smallStone_1.png");
    if (!texSmallStones[1].loadFromFile("assets/smallStone_2.png")) throw std::runtime_error("No assets/smallStone_2.png");
    if (!texSmallStones[2].loadFromFile("assets/smallStone_3.png")) throw std::runtime_error("No assets/smallStone_3.png");

    if (!texWeeds[0].loadFromFile("assets/weeds_1.png")) throw std::runtime_error("No assets/weeds_1.png");
    if (!texWeeds[1].loadFromFile("assets/weeds_2.png")) throw std::runtime_error("No assets/weeds_2.png");
  


    texturesLoaded = true;
}



//rysowanie pojedynczego drzewa
void Map::drawTile(sf::RenderWindow& window, const sf::Texture& tex, float x, float y)
{
    sf::Sprite spr(tex);
    // skalowanie do tileSize
    auto s = tex.getSize();
    if (s.x > 0 && s.y > 0) {
        float sx = (float)tileSize / (float)s.x;
        float sy = (float)tileSize / (float)s.y;
        spr.setScale({ sx, sy });
    }
    spr.setPosition({ x, y });
    window.draw(spr);
}

//rysowanie drzew na dowolną ilość kafelków
void Map::drawTree2Tiles(sf::RenderWindow& window, const sf::Texture& tex, int gridX, int gridY)
{
    sf::Sprite spr(tex);
    auto s = tex.getSize();
    if (s.x == 0 || s.y == 0) return;

    //skalowanie drzew 
    float targetW = 1.2f * tileSize;
    float targetH = 1.4f * tileSize;

    spr.setScale({ targetW / (float)s.x, targetH / (float)s.y });

    
    spr.setOrigin({ s.x / 2.f, (float)s.y });

   
    float x = gridX * tileSize + tileSize / 2.f;
    float y = (gridY + 1) * tileSize;

    spr.setPosition({ x, y });
    window.draw(spr);
}

//rysowanie flag
void Map::drawFlag2Tiles(sf::RenderWindow& window, const sf::Texture& tex, int gridX, int gridY)
{
    sf::Sprite spr(tex);
    auto s = tex.getSize();
    if (s.x == 0 || s.y == 0) return;

    // 1×2 kafelki (w górę)
    float targetW = 1.f * tileSize;
    float targetH = 2.f * tileSize;

    spr.setScale({ targetW / (float)s.x, targetH / (float)s.y });

    // kotwica: dół-środek (żeby stało na kafelku z '*')
    spr.setOrigin({ s.x / 2.f, (float)s.y });

    // pozycja: środek kafelka w X, dół kafelka w Y
    float x = gridX * tileSize + tileSize / 2.f;
    float y = (gridY + 1) * tileSize;

    spr.setPosition({ x, y });
    window.draw(spr);
}



void Map::loadMap() {
    std::vector<std::string> layout = {
        "..........#W.........S....#....", // 0  
        ".S......A.#...W...........#.WA.", // 1
        "#######...###########.A...#....", // 2
        "......#.S...........#.....#.S..", // 3
        "...####.....A...W...#..W..#..S.", // 4
        "...#............~~..#######....", // 5
        ".A.#...########.~~W.#.......A..", // 6
        "...#..S#......#.....#.......S..", // 7
        "...#####...W..#######.A..SW....", // 8
        ".....W....A...#..~~~~~~~~~~~~~~", // 9  
        ".S......W....*#*.~........###W.", // 10 
        "..############*#########..#.###", // 11     
        "..#..........*#*.~.....#..#....", // 12
        "..#....W....W.#..~..W..####....", // 13
        "~~#~~~~~~~~~~~#~~~....W.....W..", // 14
        "..#....S....W.#.......#######S.", // 15 
        "..#W.A........#....A..#.....#..", // 16
        "..#........S..#.....S.#.A.###..", // 17
        "..#.W..S......#########...#...A", // 18
        "###........A............S.#....", // 19
        ".W..A..W...S..........W...#...."  // 20 
    };
    int rows = (int)layout.size();           //pion liczy znaki
    int cols = (int)layout[0].size();        //poziom liczy znaki

    grid.assign(rows, std::vector<char>(cols)); //ustawiamy rozmiar siatki
    treeVariant.assign(rows, std::vector<std::uint8_t>(cols, 0));
    stoneVariant.assign(rows, std::vector<std::uint8_t>(cols, 0));
    weedsVariant.assign(rows, std::vector<std::uint8_t>(cols, 0));
    spawnPoints.clear();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
           
            grid[i][j] = layout[i][j];

            if (layout[i][j] == '*') {
                baseTile = { j, i };
            }
            if (layout[i][j] == '#') {//można to w sumie w jedneym ifie zrobić 
                if (j == 0 || i == 0 || i == rows - 1) {
                    spawnPoints.push_back({ j, i });
                }
            }
            if (layout[i][j] == 'A') {            // drzewo losowane z 1-4
                treeVariant[i][j] = std::rand() % 4;
                
                if (i - 1 >= 0 && grid[i - 1][j] == '.') {
                    grid[i - 1][j] = 'x'; // zajęte przez koronę drzewa
                }
            }
            else if (grid[i][j] == 'S') {                 // mały kamień losowany 1-3
                stoneVariant[i][j] = std::rand() % 3;
            }
            else if (grid[i][j] == 'W') {                 // chwasty losowane 1-2
                weedsVariant[i][j] = std::rand() % 2;   
            }
         }
     }
 }

    void Map::draw(sf::RenderWindow& window) 
    {
        loadTextures();

        for (int i = 0; i < getHeight(); i++) {
            for (int j = 0; j < getWidth(); j++) {
                float px = (float)(j * tileSize);
                float py = (float)(i * tileSize);

                char c = grid[i][j];

               // woda
                if (c == '~') {
                    drawTile(window, texWater, px, py);
                }
                // ścieżka
                else if (c == '#') {
                    // path 
                    drawTile(window, texPath, px, py);
                }
                else {
                    // grass
                    drawTile(window, texGrass, px, py);
                }

                //  OBIEKTY/DEKORACJE NA WIERZCHU
                if (c == 'A') {
                    drawTree2Tiles(window, texTrees[treeVariant[i][j]], j, i);
                } // małe kamienie
                else if (c == 'S') {
                    drawTile(window, texSmallStones[stoneVariant[i][j]], px, py);
                } //chwasty
                else if (c == 'W') {
                    drawTile(window, texWeeds[weedsVariant[i][j]], px, py);
                }
                else if (c == '*') {
                    // stone jako flagi (baza
                    drawFlag2Tiles(window, texStone, j, i);
                }
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
