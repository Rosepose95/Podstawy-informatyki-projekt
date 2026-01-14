#include "Menu.h"
#include <vector>
#include <iostream>
#include "PauseMenu.h"

// Konstruktor Menu
Menu::Menu(float width, float height, sf::Font& arial)
    : title(arial)
    , startText(arial)
    , loadText(arial)
    , exitText(arial)
    , editText(arial)
    , backText(arial)
    , backgroundSpirite(backgroundTexture)
    , slotButtons()
    , slotTexts()
    , autoSaveText(arial)
    , width(width)
    , height(height)
{
    // --- ŁADOWANIE TŁA ---
    if (!backgroundTexture.loadFromFile("assets/menugraf.png")) {
        std::cout << "Nie zaladowalo tla" << std::endl;
    }
    backgroundSpirite.setTexture(backgroundTexture, true);
    sf::Vector2u texturesize = backgroundTexture.getSize();
    backgroundSpirite.setScale({ width / (float)texturesize.x, height / (float)texturesize.y });

    // --- TYTUŁ ---
    title.setString("TOWER DEFFENCE");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color(0, 0, 128));
    title.setStyle(sf::Text::Bold | sf::Text::Italic);
    auto b = title.getLocalBounds();
    title.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
    title.setPosition({ width / 2.f, height * 0.25f });

    // --- PRZYCISKI MENU ---
    float buttonSpacing = 100.f; // odstęp między przyciskami
    float startY = height / 2.f;  // Y pierwszego przycisku (START)

    // --- PRZYCISK START ---
    startButton.setSize({ 250.f, 70.f });
    startButton.setOutlineThickness(3.f);
    startButton.setOutlineColor(sf::Color::Black);
    auto sb = startButton.getLocalBounds();
    startButton.setOrigin({ sb.position.x + sb.size.x / 2.f, sb.position.y + sb.size.y / 2.f });
    startButton.setPosition({ width / 2.f, startY });

    startText.setString("START");
    startText.setCharacterSize(40);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);
    auto st = startText.getLocalBounds();
    startText.setOrigin({ st.position.x + st.size.x / 2.f, st.position.y + st.size.y / 2.f });
    startText.setPosition(startButton.getPosition());

    // --- PRZYCISK LOAD GAME ---
    loadButton.setSize({ 250.f, 70.f });
    loadButton.setOutlineThickness(3.f);
    loadButton.setOutlineColor(sf::Color::Black);
    auto lb = loadButton.getLocalBounds();
    loadButton.setOrigin({ lb.position.x + lb.size.x / 2.f, lb.position.y + lb.size.y / 2.f });
    loadButton.setPosition({ width / 2.f, startY + buttonSpacing });

    loadText.setString("LOAD GAME");
    loadText.setCharacterSize(40);
    loadText.setFillColor(sf::Color::White);
    loadText.setStyle(sf::Text::Bold);
    auto lt = loadText.getLocalBounds();
    loadText.setOrigin({ lt.position.x + lt.size.x / 2.f, lt.position.y + lt.size.y / 2.f });
    loadText.setPosition(loadButton.getPosition());

    // --- PRZYCISK MAKE YOUR MAP ---
    editButton.setSize({ 250.f, 70.f });
    editButton.setOutlineThickness(3.f);
    editButton.setOutlineColor(sf::Color::Black);
    auto gb = editButton.getLocalBounds();
    editButton.setOrigin({ gb.position.x + gb.size.x / 2.f, gb.position.y + gb.size.y / 2.f });
    editButton.setPosition({ width / 2.f, startY + buttonSpacing * 2 });

    editText.setString("MAP MAKER");
    editText.setCharacterSize(30);
    editText.setFillColor(sf::Color::White);
    editText.setStyle(sf::Text::Bold);
    auto gt = editText.getLocalBounds();
    editText.setOrigin({ gt.position.x + gt.size.x / 2.f, gt.position.y + gt.size.y / 2.f });
    editText.setPosition(editButton.getPosition());

    // --- PRZYCISK EXIT ---
    exitButton.setSize({ 250.f, 70.f });
    exitButton.setOutlineThickness(3.f);
    exitButton.setOutlineColor(sf::Color::Black);
    auto eb = exitButton.getLocalBounds();
    exitButton.setOrigin({ eb.position.x + eb.size.x / 2.f, eb.position.y + eb.size.y / 2.f });
    exitButton.setPosition({ width / 2.f, startY + buttonSpacing * 3 });

    exitText.setString("EXIT");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::White);
    exitText.setStyle(sf::Text::Bold);
    auto et = exitText.getLocalBounds();
    exitText.setOrigin({ et.position.x + et.size.x / 2.f, et.position.y + et.size.y / 2.f });
    exitText.setPosition(exitButton.getPosition());

    // --- PRZYCISK BACK (LOAD SCREEN) ---
    backButton.setSize({ 250.f, 70.f });
    backButton.setOutlineThickness(3.f);
    backButton.setOutlineColor(sf::Color::Black);

    backText.setString("BACK");
    backText.setCharacterSize(40);
    backText.setFillColor(sf::Color::White);
    backText.setStyle(sf::Text::Bold);

    initSlots(arial); // inicjalizacja slotów
    autoSaveText.setString("LOAD AUTOSAVE");
}


// --- RYSOWANIE ---
void Menu::draw(sf::RenderWindow& window)
{
    window.draw(backgroundSpirite);
    window.draw(title);

    if (!inLoadScreen) {
        window.draw(startButton);
        window.draw(startText);
        window.draw(loadButton);
        window.draw(loadText);
        window.draw(exitButton);
        window.draw(exitText);
        window.draw(editButton);
        window.draw(editText);
    }
    else {
        for (int i = 0; i < slotButtons.size(); ++i) {
            window.draw(slotButtons[i]);
            window.draw(slotTexts[i]);
        }
        window.draw(backButton);
        window.draw(backText);
    }
}

// --- HOVER ---
void Menu::handleHover(sf::Vector2i mousepos)
{
    sf::Vector2f m((float)mousepos.x, (float)mousepos.y);

    if (!inLoadScreen) {
        startButton.setFillColor(startButton.getGlobalBounds().contains(m) ? sf::Color::Cyan : sf::Color::Blue);
        loadButton.setFillColor(loadButton.getGlobalBounds().contains(m) ? sf::Color::Cyan : sf::Color::Blue);
        exitButton.setFillColor(exitButton.getGlobalBounds().contains(m) ? sf::Color::Cyan : sf::Color::Blue);
        editButton.setFillColor(editButton.getGlobalBounds().contains(m) ? sf::Color::Cyan : sf::Color::Blue);
    }
    else {
        for (int i = 0; i < slotButtons.size(); ++i) {
            slotButtons[i].setFillColor(slotButtons[i].getGlobalBounds().contains(m) ? sf::Color::Cyan : sf::Color::White);
        }
        backButton.setFillColor(backButton.getGlobalBounds().contains(m) ? sf::Color::Cyan : sf::Color::Blue);
    }
}

// --- KLIKNIĘCIA PRZYCISKÓW ---
bool Menu::isStartClicked(sf::Vector2i mousepos)
{
    sf::Vector2f m((float)mousepos.x, (float)mousepos.y);
    return startButton.getGlobalBounds().contains(m) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
bool Menu::isLoadClicked(sf::Vector2i mousepos)
{
    sf::Vector2f m((float)mousepos.x, (float)mousepos.y);
    return !inLoadScreen && loadButton.getGlobalBounds().contains(m) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
bool Menu::isExitClicked(sf::Vector2i mousepos)
{
    sf::Vector2f m((float)mousepos.x, (float)mousepos.y);
    return exitButton.getGlobalBounds().contains(m) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
bool Menu::isEditClicked(sf::Vector2i mousepos)
{
    sf::Vector2f m((float)mousepos.x, (float)mousepos.y);
    return editButton.getGlobalBounds().contains(m) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
bool Menu::isBackClicked(sf::Vector2i mousePos)
{
    sf::Vector2f m((float)mousePos.x, (float)mousePos.y);
    backButton.setOrigin({ backButton.getSize().x / 2.f, backButton.getSize().y / 2.f });
    for (auto& slot : slotButtons)
        slot.setOrigin({ slot.getSize().x / 2.f, slot.getSize().y / 2.f });

    return inLoadScreen && backButton.getGlobalBounds().contains(m) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

// --- OBSŁUGA SLOTÓW ---
int Menu::slotClicked(sf::Vector2i mousePos)
{
    if (!inLoadScreen) return 0;
    sf::Vector2f m((float)mousePos.x, (float)mousePos.y);
    for (int i = 0; i < slotButtons.size(); ++i) {
        if (slotButtons[i].getGlobalBounds().contains(m)) return i + 1;
        if (slotButtons[i].getGlobalBounds().contains(m)
            && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            return i + 1;
        }
    }
    backButton.setOrigin({ backButton.getSize().x / 2.f, backButton.getSize().y / 2.f });
    for (auto& slot : slotButtons)
        slot.setOrigin({ slot.getSize().x / 2.f, slot.getSize().y / 2.f });

    return 0;
}

// --- INICJALIZACJA SLOTÓW ---
void Menu::initSlots(sf::Font& font)
{
    slotButtons.clear();
    slotTexts.clear();
    float totalHeight = 3 * SLOT_HEIGHT + 2 * SLOT_SPACING;
    float startY = height / 2.f - totalHeight / 2.f;

    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape slot({ SLOT_WIDTH, SLOT_HEIGHT });
        slot.setOrigin({ SLOT_WIDTH / 2.f, SLOT_HEIGHT / 2.f });
        slot.setPosition({ width / 2.f, startY + i * (SLOT_HEIGHT + SLOT_SPACING) + SLOT_HEIGHT / 2.f });
        slot.setFillColor(sf::Color(220, 220, 220));
        slot.setOutlineColor(sf::Color::Black);
        slot.setOutlineThickness(2.f);
        slotButtons.push_back(slot);

        sf::Text text(font);
        text.setString("Slot " + std::to_string(i + 1));
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::Black);
        auto bounds = text.getLocalBounds();
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
        text.setPosition(slot.getPosition());
        slotTexts.push_back(text);
    }

    // ustawienie BACK poniżej slotów
    float backY = startY + 3 * (SLOT_HEIGHT + SLOT_SPACING) + 50.f;
    backButton.setOrigin({ backButton.getSize().x / 2.f, backButton.getSize().y / 2.f });
    backButton.setPosition({ width / 2.f, backY });
    auto bb = backText.getLocalBounds();
    backText.setOrigin({ bb.position.x + bb.size.x / 2.f, bb.position.y + bb.size.y / 2.f });
    backText.setPosition({ width / 2.f, backY });
}

// --- OBSŁUGA LOAD SCREEN ---
void Menu::enterLoadScreen() {
    inLoadScreen = true;
    currentScreen = MenuScreen::LOAD_SLOTS;
}
void Menu::exitLoadScreen() {
    inLoadScreen = false;
    currentScreen = MenuScreen::MAIN;
}
bool Menu::backClicked(sf::Vector2i mousePos)
{
    return isBackClicked(mousePos);
}


