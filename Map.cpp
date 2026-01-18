#include "Map.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

char Map::getTile(int x, int y) const { return grid[y][x]; }
int Map::getWidth() const { return grid[0].size(); }
int Map::getHeight() const { return grid.size(); }
bool Map::isBuildable(int x, int y) const
{
    if (y < 0 || y >= getHeight() || x < 0 || x >= getWidth()) return false;

    char c = grid[y][x];
    // budować wolno tylko na czystej trawie '.'
    // wszystko inne blokuje: ścieżka, woda, meta, drzewa, chwasty, kamienie, most, płoty itd.
    return (c == '.');
}
Map::Map() {
    std::srand((unsigned)std::time(nullptr));
    loadMap();
    loadTextures();
    texGrass.setSmooth(false);
    texPath.setSmooth(false);
    texStone.setSmooth(false);
    texWater.setSmooth(false);
    texFenceX.setSmooth(false);
    texFenceY.setSmooth(false);
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
  
    if (!texFenceX.loadFromFile("assets/fence_x.png")) throw std::runtime_error("No assets/fence_x.png");
    if (!texFenceY.loadFromFile("assets/fence_y.png")) throw std::runtime_error("No assets/fence_y.png");

    texturesLoaded = true;
}

//rysowanie kamiennego płotu, x/y
void Map::drawFenceX(sf::RenderWindow& window, int gridX, int gridY)
{
    sf::Sprite spr(texFenceX);
    auto s = texFenceX.getSize();
    if (s.x == 0 || s.y == 0) return;

    float targetW = 3.15f * tileSize;
    float targetH = 1.f * tileSize;

    spr.setScale({ targetW / (float)s.x, targetH / (float)s.y });
    spr.setOrigin({ s.x / 2.f, (float)s.y });

    float x = gridX * tileSize + tileSize / 2.f;
    float y = (gridY + 1) * (float)tileSize;

    spr.setPosition({ x, y });
    window.draw(spr);
}

void Map::drawFenceY(sf::RenderWindow& window, int gridX, int gridY)
{
    sf::Sprite spr(texFenceY);
    auto s = texFenceY.getSize();
    if (s.x == 0 || s.y == 0) return;

    float targetW = 1.f * tileSize;
    float targetH = 2.2f * tileSize;

    spr.setScale({ targetW / (float)s.x, targetH / (float)s.y });
    spr.setOrigin({ s.x / 2.f, (float)s.y });

    float x = gridX * tileSize + tileSize / 2.f;
    float y = (gridY + 1) * (float)tileSize;

    spr.setPosition({ x, y });
    window.draw(spr);
}

//rysowanie na pojedynczym kafelku
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

//rysowanie na dowolną ilość kafelków
void Map::drawTree2Tiles(sf::RenderWindow& window, const sf::Texture& tex, int gridX, int gridY)
{
    sf::Sprite spr(tex);
    auto s = tex.getSize();
    if (s.x == 0 || s.y == 0) return;

    //skalowanie 
    float targetW = 1.2f * tileSize;
    float targetH = 1.7f * tileSize;

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

    float targetW = 1.f * tileSize;
    float targetH = 2.f * tileSize;

    spr.setScale({ targetW / (float)s.x, targetH / (float)s.y });
    spr.setOrigin({ s.x / 2.f, (float)s.y });

    float x = gridX * tileSize + tileSize / 2.f;
    float y = (gridY + 1) * tileSize;

    spr.setPosition({ x, y });
    window.draw(spr);
}



void Map::loadMap() {
    std::vector<std::string> layout = {
        "W~XXXXXXXX#.XXXXXXXXXXXXX~#.XXW", // 0  
        "Y~S..W...A#A..W......S.~~~#AWAY", // 1
        "#######.W.###########.A~..#...W", // 2
        "W~.S..#..A.......S..#..~..#.S.Y", // 3
        "Y~.####.....A.W.~~~~#~~~W.#..SW", // 4
        "W~.#....S.......~~~W#######...Y", // 5
        "Y~.#.W.########.~~~.#......AW.W", // 6
        "W~.#####......#...W.#.......S.Y", // 7
        "Y~.........W..#######.A..SW~~~~", // 8
        "W~~~.W....A...#..~~~~~~~~~~~..W", // 9  
        "YS.~....W....S#S.~........###WY", // 10 
        "W#############*#########..#.###", // 11     
        "Y#.~.......W.S#S.~.....####...W", // 12
        "W#.~~~~~~~~~~~#~~~...S.......SY", // 13
        "Y#.......S....#.W.....W....W..W", // 14
        "W#.A...S....W.#.......#######SY", // 15 
        "Y#...A........#....A..#.....#.W", // 16
        "W#.........S..#.....S.#.A.###.Y", // 17
        "Y#..W..S......#########...#...W", // 18
        "W#.........A.....W...W..S.#..AY", // 19
        "Y#WXXXXXXXXXXXXXXXXXXXXXXX#.XXX"  // 20 
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
                    grid[i - 1][j] = 'c'; // brak mozliwosci budowania wiez na "drugim" kafelku ktory zajmuje drzewo
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

                //  OBIEKTY/DEKORACJE 
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
                    // stone jako flagi (baza)
                    drawFlag2Tiles(window, texStone, j, i);
                }
                else if (c == 'X') {
                    drawFenceX(window, j, i);
                }
                else if (c == 'Y') {
                    drawFenceY(window, j, i);
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

