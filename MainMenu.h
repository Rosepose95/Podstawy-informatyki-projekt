#pragma once
#include <SFML/Graphics.hpp>
#include "GameTypes.h"

class MainMenu {
public:
    MainMenu(float width, float height, sf::Font& font);

    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);

    void handleClick(sf::Vector2f mousePos);
    void reset();

    bool confirmClicked() const { return confirmed; }
    bool backClicked() const { return backPressed; }

    GameMode getSelectedMode() const { return selectedMode; }
    Difficulty getSelectedDifficulty() const { return selectedDifficulty; }

private:
    struct Button {
        sf::RectangleShape box;
        sf::Text text;
        bool hovered = false;
		bool selected = false;
        Button(sf::Font& font) : text(font) {}
    };

    // tryb wyboru gry
    Button adventureBtn;
    Button endlessBtn;

    // trudnoœæ
    Button easyBtn;
    Button normalBtn;
    Button hardBtn;

    // start / back
    Button startBtn;
    Button backBtn;

    GameMode selectedMode = GameMode::Adventure;
    Difficulty selectedDifficulty = Difficulty::Normal;

    bool confirmed = false;
    bool backPressed = false;
};
