#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <array>
#include <cstdint>

class Map {
public:
    Map();
    void draw(sf::RenderWindow& window);

    char getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    sf::Vector2i baseTile;  //do pathingu
    std::vector<sf::Vector2i> spawnPoints; //do spawnów
    
    int tileSize = 40; // Rozmiar kafelka (800/20 = 40, 640/16 = 40)
    void drawTree2Tiles(sf::RenderWindow& window, const sf::Texture& tex, int gridX, int gridY);
    void setTile(int x, int y, char type);  //malowanie mapy
    void clearMap();
    void refreshLogic();
    void loadMap();
    void drawFlag2Tiles(sf::RenderWindow& window, const sf::Texture& tex, int gridX, int gridY);
    void drawFenceX(sf::RenderWindow& window, int gridX, int gridY);
    void drawFenceY(sf::RenderWindow& window, int gridX, int gridY);
    bool isBuildable(int x, int y) const;
    void drawTower2Tiles(sf::RenderWindow& window, int gridX, int gridY);
    const sf::Texture& getBrushTexture(char brush) const; //getter na tekstury do edytora
private:

    std::array<sf::Texture, 4> texTrees;                 // tree_1..tree_4
    std::vector<std::vector<std::uint8_t>> treeVariant;  
    std::array<sf::Texture, 3> texSmallStones;                 // smallStone_1..3
    std::vector<std::vector<std::uint8_t>> stoneVariant;   
    std::array<sf::Texture, 2> texWeeds;                       // weeds_1..2
    std::vector<std::vector<std::uint8_t>> weedsVariant;       


    std::vector<std::vector<char>> grid;
  
    bool texturesLoaded = false;
    sf::Texture texGrass;  //trawa (stawianie wiez)
    sf::Texture texPath;   //sciezka enemies
    sf::Texture texStone;  //meta 
    sf::Texture texWater;  //woda
    sf::Texture texTree;   //drzewo
    sf::Texture texFenceX;  //kamienny płot poziomy
    sf::Texture texFenceY;  //kamienny płot pionowy
    sf::Texture texTowerEditor; //textura wieży do edytora
  
    void loadTextures();
    void drawTile(sf::RenderWindow& window, const sf::Texture& tex, float x, float y);
   
};


#endif

