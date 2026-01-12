#include "Menu.h"
#include <vector>
#include <iostream>
#include "PauseMenu.h"
#include "SaveSystem.h"
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
	, autoSlotText(arial)
{
    // --- £ADOWANIE T£A ---
    if (!backgroundTexture.loadFromFile("assets/menugraf.png")) {
        std::cout << "Nie zaladowalo tla" << std::endl;
    }
    backgroundSpirite.setTexture(backgroundTexture, true);
    sf::Vector2u texturesize = backgroundTexture.getSize();
    backgroundSpirite.setScale({ width / (float)texturesize.x, height / (float)texturesize.y });

    // --- TYTU£ ---
    title.setString("TOWER DEFFENCE");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color(0, 0, 128));
    title.setStyle(sf::Text::Bold | sf::Text::Italic);
    auto b = title.getLocalBounds();
    title.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
    title.setPosition({ width / 2.f, height * 0.25f });

    // --- PRZYCISKI MENU ---
    float buttonSpacing = 100.f; // odstêp miêdzy przyciskami
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
    backButton.setSize({ 260.f, 70.f });
    backButton.setFillColor(sf::Color::Blue);
    backButton.setOutlineThickness(3.f);
    backButton.setOutlineColor(sf::Color::Black);
    backButton.setOrigin({ 130.f, 35.f });
    backButton.setPosition({ width / 2.f, height * 0.8f });

    backText.setString("BACK");
    backText.setCharacterSize(36);
    backText.setFillColor(sf::Color::White);
    initSlots(arial); // inicjalizacja slotów

    float liftUp = -50.f; 

    // --- AUTOSAVE BUTTON ---
    autoSlotButton.setSize({ SLOT_WIDTH, SLOT_HEIGHT });
    autoSlotButton.setFillColor(sf::Color(220, 220, 220)); // TAKI SAM KOLOR JAK SLOTY
    autoSlotButton.setOutlineThickness(2.f);
    autoSlotButton.setOutlineColor(sf::Color::Black);
    autoSlotButton.setOrigin({ SLOT_WIDTH / 2.f, SLOT_HEIGHT / 2.f });

    float autoSlotY = height * 0.48f + liftUp;
    autoSlotButton.setPosition({ width / 2.f, autoSlotY });

    // --- AUTOSAVE TEXT ---
    autoSlotText.setString("[AUTO] " + SaveSystem::getDescription(0));
    autoSlotText.setCharacterSize(28);          // TAKA SAMA WIELKOŒÆ JAK SLOTY
    autoSlotText.setFillColor(sf::Color::Black);

    // RÊCZNA korekta (JEDYNE MO¯LIWE ROZWI¥ZANIE)
    autoSlotText.setPosition({
        autoSlotButton.getPosition().x - 140.f,  // lewo/prawo
        autoSlotButton.getPosition().y - 20.f   // góra/dó³
        });
    float slotStartY =
        autoSlotButton.getPosition().y + SLOT_HEIGHT + 30.f;

    for (int i = 0; i < 3; ++i) {
        slotButtons[i].setPosition({
            width / 2.f,
            slotStartY + i * (SLOT_HEIGHT + SLOT_SPACING)
            });

        slotTexts[i].setPosition({
            slotButtons[i].getPosition().x - 70.f,
            slotButtons[i].getPosition().y - 17.f
            });
    }

    float backY =
        slotButtons[2].getPosition().y + SLOT_HEIGHT + 40.f;

    backButton.setPosition({
        width / 2.f,
        backY
        });
    backText.setPosition(backButton.getPosition());


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
        window.draw(autoSlotButton);
        window.draw(autoSlotText);
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
        autoSlotButton.setFillColor(autoSlotButton.getGlobalBounds().contains(m) ? sf::Color::Cyan : sf::Color(200, 200, 50));
        for (int i = 0; i < slotButtons.size(); ++i) {
            slotButtons[i].setFillColor(slotButtons[i].getGlobalBounds().contains(m) ? sf::Color::Cyan : sf::Color::White);
        }
        backButton.setFillColor(backButton.getGlobalBounds().contains(m) ? sf::Color::Cyan : sf::Color::Blue);
    }
}

// --- KLIKNIÊCIA PRZYCISKÓW ---
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

// --- OBS£UGA SLOTÓW ---
int Menu::slotClicked(sf::Vector2i mousePos)
{
    if (!inLoadScreen) return -1;
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) return -1;

    float mx = (float)mousePos.x;
    float my = (float)mousePos.y;
    sf::Vector2f m((float)mousePos.x, (float)mousePos.y);
    // AUTOSLOT
    if (autoSlotButton.getGlobalBounds().contains(m)
        && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        return 0;
    }



    // NORMALNE SLOTY
    for (int i = 0; i < (int)slotButtons.size(); ++i) {
        if (slotButtons[i].getGlobalBounds().contains({ mx, my })) {
            return i + 1;
        }
    }

    return -1;
}



// --- INICJALIZACJA SLOTÓW ---
void Menu::initSlots(sf::Font& font)
{
    slotButtons.clear();
    slotTexts.clear();
    float totalHeight = 3 * SLOT_HEIGHT + 2 * SLOT_SPACING;
    float autoSlotBottom = autoSlotButton.getPosition().y + SLOT_HEIGHT / 2.f;
    float startY = autoSlotBottom + SLOT_SPACING; // slot 1 startuje poni¿ej autosave

    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape slot({ SLOT_WIDTH, SLOT_HEIGHT });
        slot.setOrigin({ SLOT_WIDTH / 2.f, SLOT_HEIGHT / 2.f });
        slot.setPosition({ width / 2.f, startY + i * (SLOT_HEIGHT + SLOT_SPACING) });
        slot.setFillColor(sf::Color(220, 220, 220));
        slot.setOutlineColor(sf::Color::Black);
        slot.setOutlineThickness(2.f);
        slotButtons.push_back(slot);

        sf::Text text(font);
        text.setString("Slot " + std::to_string(i + 1));
        text.setCharacterSize(28);
        text.setFillColor(sf::Color::Black);
        auto bounds = text.getLocalBounds();
        text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
        text.setPosition(slot.getPosition());
        slotTexts.push_back(text);
    }

    // BACK BUTTON
    float backY = startY + 3 * (SLOT_HEIGHT + SLOT_SPACING) + 60.f;
    backButton.setOrigin({ backButton.getSize().x / 2.f, backButton.getSize().y / 2.f });
    backButton.setPosition({ width / 2.f, backY });

    auto bb = backText.getLocalBounds();
    backText.setOrigin({ bb.position.x + bb.size.x / 2.f, bb.position.y + bb.size.y / 2.f });
    backText.setPosition(backButton.getPosition());


}

// --- OBS£UGA LOAD SCREEN ---
void Menu::enterLoadScreen() {
    inLoadScreen = true;
    currentScreen = MenuScreen::LOAD_SLOTS;

    for (int i = 1; i <= 3; ++i)
        setSlotText(i, "Slot " + std::to_string(i) + " | EMPTY");
}

void Menu::exitLoadScreen() {
    inLoadScreen = false;
    currentScreen = MenuScreen::MAIN;
}
bool Menu::backClicked(sf::Vector2i mousePos)
{
    if (!inLoadScreen) return false;

    sf::Vector2f m((float)mousePos.x, (float)mousePos.y);
    return backButton.getGlobalBounds().contains(m)
        && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}


void Menu::setSlotText(int slot, const std::string& text)
{
    if (slot < 1 || slot > 3) return;

    sf::Text& t = slotTexts[slot - 1];
    t.setString(text);

    // przeliczanie origin i ustawienie pozycji na œrodku slotu
    auto bounds = t.getLocalBounds();
    t.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
    t.setPosition(slotButtons[slot - 1].getPosition());

}
void Menu::setAutoSlotText(const std::string& txt) {
    autoSlotText.setString(txt);
}

void Menu::refreshAutoSlot() {
    autoSlotText.setString("[AUTO] " + SaveSystem::getDescription(0));
    

}
