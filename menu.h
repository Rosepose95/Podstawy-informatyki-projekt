#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Ekrany menu
enum class MenuScreen {
    MAIN,
    LOAD_SLOTS
};

class Menu {
private:
    // --- Teksty ---
    sf::Text title;
    sf::Text startText;
    sf::Text loadText;
    sf::Text exitText;
    sf::Text editText;
    sf::Text backText;
    sf::Text autoSaveText;

    // --- Przycisk / prostokąty ---
    sf::RectangleShape startButton;
    sf::RectangleShape loadButton;
    sf::RectangleShape exitButton;
    sf::RectangleShape editButton;
    sf::RectangleShape backButton;

    // --- Grafika tła ---
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSpirite;

    // --- Sloty ---
    std::vector<sf::RectangleShape> slotButtons; // prostokąty slotów
    std::vector<sf::Text> slotTexts;             // teksty slotów
    static constexpr float SLOT_WIDTH = 360.f;
    static constexpr float SLOT_HEIGHT = 70.f;
    static constexpr float SLOT_SPACING = 20.f;

    // --- Stan menu ---
    bool inLoadScreen = false;
    MenuScreen currentScreen = MenuScreen::MAIN;

    // --- Rozmiary okna ---
    float width;
    float height;

public:
    // Konstruktor
    Menu(float width, float height, sf::Font& arial);

    // --- Rysowanie ---
    void draw(sf::RenderWindow& window);

    // --- Hover / podświetlanie ---
    void handleHover(sf::Vector2i mousepos);

    // --- Kliknięcia ---
    bool isStartClicked(sf::Vector2i mousepos);
    bool isLoadClicked(sf::Vector2i mousepos);
    bool isExitClicked(sf::Vector2i mousepos);
    bool isEditClicked(sf::Vector2i mousepos);
    bool isBackClicked(sf::Vector2i mousePos);
    bool backClicked(sf::Vector2i mousePos);

    // --- Obsługa slotów ---
    int slotClicked(sf::Vector2i mousePos);
    void setSlotText(int slot, const std::string& text);
    void initSlots(sf::Font& font);

    // --- Load Screen ---
    void enterLoadScreen();
    void exitLoadScreen();

    // --- Sprawdzenie stanu ---
    bool isInLoadScreen() const { return inLoadScreen; }
};

#endif



