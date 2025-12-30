#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <SFML/Graphics.hpp>

class PauseMenu {
private:
    sf::RectangleShape background;

    sf::Text title;
    sf::Text resumeText;
    sf::Text restartText;
    sf::Text menuText;

public:
    PauseMenu(float width, float height, sf::Font& font);

    void draw(sf::RenderWindow& window);
    void handleHover(sf::Vector2i mousePos);

    bool resumeClicked(sf::Vector2i mousePos);
    bool restartClicked(sf::Vector2i mousePos);
    bool menuClicked(sf::Vector2i mousePos);
};

#endif

