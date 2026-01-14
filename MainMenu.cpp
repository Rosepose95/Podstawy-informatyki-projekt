#include "MainMenu.h"

MainMenu::MainMenu(float width, float height, sf::Font& font)
    : adventureBtn(font), endlessBtn(font),
    easyBtn(font), normalBtn(font), hardBtn(font),
    startBtn(font), backBtn(font)
{
    
    sf::RectangleShape background({ width, height });
    background.setFillColor(sf::Color(30, 30, 40));

    // pozycjonowanie i teksty przycisków
    adventureBtn.text.setString("ADVENTURE");
    adventureBtn.box.setSize({ 300.f, 60.f });
    adventureBtn.box.setPosition({ width / 2.f - 150.f, 150.f });

    endlessBtn.text.setString("ENDLESS");
    endlessBtn.box.setSize({ 300.f, 60.f });
    endlessBtn.box.setPosition({ width / 2.f - 150.f, 250.f });

    easyBtn.text.setString("EASY");
    easyBtn.box.setSize({ 200.f, 50.f });
    easyBtn.box.setPosition({ width / 2.f - 100.f, 350.f });

    normalBtn.text.setString("NORMAL");
    normalBtn.box.setSize({ 200.f, 50.f });
    normalBtn.box.setPosition({ width / 2.f - 100.f, 420.f });

    hardBtn.text.setString("HARD");
    hardBtn.box.setSize({ 200.f, 50.f });
    hardBtn.box.setPosition({ width / 2.f - 100.f, 490.f });

    startBtn.text.setString("START");
    startBtn.box.setSize({ 200.f, 60.f });
    startBtn.box.setPosition({ width / 2.f - 100.f, 600.f });

    backBtn.text.setString("BACK");
    backBtn.box.setSize({ 200.f, 60.f });
    backBtn.box.setPosition({ width / 2.f - 100.f, 680.f });

    // style tekstu
    Button* buttons[] = { &adventureBtn, &endlessBtn, &easyBtn, &normalBtn, &hardBtn, &startBtn, &backBtn };
    for (auto b : buttons) {
        b->text.setCharacterSize(24);
        b->text.setFillColor(sf::Color::Black);
        auto bounds = b->text.getLocalBounds();
        b->text.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });

        b->text.setPosition(b->box.getPosition() + sf::Vector2f(b->box.getSize().x / 2.f, b->box.getSize().y / 2.f));
        b->box.setFillColor(sf::Color(220, 220, 220));
        b->box.setOutlineThickness(2.f);
        b->box.setOutlineColor(sf::Color::Black);
    }
}

void MainMenu::update(sf::Vector2f mousePos) {
    Button* buttons[] = { &adventureBtn, &endlessBtn, &easyBtn, &normalBtn, &hardBtn, &startBtn, &backBtn };

    for (auto b : buttons) {
        if (b->box.getGlobalBounds().contains(mousePos)) {
            b->hovered = true;
            if (!b->selected)
                b->box.setFillColor(sf::Color(200, 200, 50)); // hover, gdy nie wybrany
        }
        else {
            b->hovered = false;
            if (!b->selected)
                b->box.setFillColor(sf::Color(220, 220, 220)); // normalny kolor
        }
    }
}
void MainMenu::draw(sf::RenderWindow& window) {
    Button* buttons[] = { &adventureBtn, &endlessBtn, &easyBtn, &normalBtn, &hardBtn, &startBtn, &backBtn };

    for (auto b : buttons) {
        // Kolor przycisku – jeœli wybrany, ca³y przycisk na ¿ó³to
        if (b->selected) b->box.setFillColor(sf::Color(255, 220, 0));

        window.draw(b->box);
        window.draw(b->text);

        // Podkreœlenie tylko dla wybranych przycisków (opcjonalnie)
        if (b->selected) {
            b->box.setOutlineColor(sf::Color::Red);
        }
    }
}





void MainMenu::handleClick(sf::Vector2f mousePos) {
    backPressed = false;
    confirmed = false;

    // --- TRYBY GRY (Adventure / Endless) ---
    if (adventureBtn.box.getGlobalBounds().contains(mousePos)) {
        selectedMode = GameMode::Adventure;
        adventureBtn.selected = true;
        endlessBtn.selected = false;
    }
    if (endlessBtn.box.getGlobalBounds().contains(mousePos)) {
        selectedMode = GameMode::Endless;
        endlessBtn.selected = true;
        adventureBtn.selected = false;
    }

    // --- POZIOMY TRUDNOŒCI (Easy / Normal / Hard) ---
    if (easyBtn.box.getGlobalBounds().contains(mousePos)) {
        selectedDifficulty = Difficulty::Easy;
        easyBtn.selected = true;
        normalBtn.selected = false;
        hardBtn.selected = false;
    }
    if (normalBtn.box.getGlobalBounds().contains(mousePos)) {
        selectedDifficulty = Difficulty::Normal;
        normalBtn.selected = true;
        easyBtn.selected = false;
        hardBtn.selected = false;
    }
    if (hardBtn.box.getGlobalBounds().contains(mousePos)) {
        selectedDifficulty = Difficulty::Hard;
        hardBtn.selected = true;
        easyBtn.selected = false;
        normalBtn.selected = false;
    }

    // --- START / BACK ---
    if (startBtn.box.getGlobalBounds().contains(mousePos)) confirmed = true;
    if (backBtn.box.getGlobalBounds().contains(mousePos)) backPressed = true;
}


void MainMenu::reset() {
    confirmed = false;
    backPressed = false;
    selectedMode = GameMode::Adventure;
    selectedDifficulty = Difficulty::Normal;
}
