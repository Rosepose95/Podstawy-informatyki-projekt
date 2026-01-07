#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class PauseScreen {
    MAIN,
    SAVE_SLOTS,
    LOAD_SLOTS
};

class PauseMenu {
private:
    sf::RectangleShape background;

    sf::Text title;
    sf::Text resumeText;
    sf::Text restartText;
    sf::Text menuText;
    sf::Text saveText;
    sf::Text loadText;   // ? TO JEST OK
    std::vector<sf::RectangleShape> slotButtons;
    std::vector<sf::Text> slotTexts;

    sf::RectangleShape backButton;
    sf::Text backText;



public:
    PauseScreen currentScreen = PauseScreen::MAIN;

    PauseMenu(float width, float height, sf::Font& font);

    void draw(sf::RenderWindow& window);
    void handleHover(sf::Vector2i mousePos);

    bool resumeClicked(sf::Vector2i mousePos);
    bool restartClicked(sf::Vector2i mousePos);
    bool menuClicked(sf::Vector2i mousePos);
    bool saveClicked(sf::Vector2i mousePos);
    bool loadClicked(sf::Vector2i mousePos);

    int slotClicked(sf::Vector2i mousePos);
    void setSlotText(int index, const std::string& text);
    bool backClicked(sf::Vector2i mousePos);
    
};
#endif
