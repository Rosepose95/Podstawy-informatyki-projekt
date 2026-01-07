#include "PauseMenu.h"
#include "Menu.h"

PauseMenu::PauseMenu(float width, float height, sf::Font& font)
    : title(font)
    , resumeText(font)
    , restartText(font)
    , menuText(font)
    , saveText(font)
    , loadText(font)
	, slotButtons()
	, backText(font)
{
    // półprzezroczyste tło
    background.setSize({ width, height });
    background.setFillColor(sf::Color(0, 0, 0, 150));

    // tytuł
    title.setString("PAUSED");
    title.setCharacterSize(70);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    auto tb = title.getLocalBounds();
    title.setOrigin({ tb.position.x + tb.size.x / 2.f, tb.position.y + tb.size.y / 2.f });
    title.setPosition({ width / 2.f, height * 0.2f });

    // RESUME
    resumeText.setString("RESUME");
    resumeText.setCharacterSize(40);
    auto rb = resumeText.getLocalBounds();
    resumeText.setOrigin({ rb.position.x + rb.size.x / 2.f, rb.position.y + rb.size.y / 2.f });
    resumeText.setPosition({ width / 2.f, height * 0.35f });

    // RESTART
    restartText.setString("RESTART");
    restartText.setCharacterSize(40);
    auto re = restartText.getLocalBounds();
    restartText.setOrigin({ re.position.x + re.size.x / 2.f, re.position.y + re.size.y / 2.f });
    restartText.setPosition({ width / 2.f, height * 0.45f });

    // SAVE
    saveText.setString("SAVE");
    saveText.setCharacterSize(40);
    auto sb = saveText.getLocalBounds();
    saveText.setOrigin({ sb.position.x + sb.size.x / 2.f, sb.position.y + sb.size.y / 2.f });
    saveText.setPosition({ width / 2.f, height * 0.55f });

    // LOAD
    loadText.setString("LOAD");
    loadText.setCharacterSize(40);
    auto lb = loadText.getLocalBounds();
    loadText.setOrigin({ lb.position.x + lb.size.x / 2.f, lb.position.y + lb.size.y / 2.f });
    loadText.setPosition({ width / 2.f, height * 0.65f });

    // MENU
    menuText.setString("MENU");
    menuText.setCharacterSize(40);
    auto me = menuText.getLocalBounds();
    menuText.setOrigin({ me.position.x + me.size.x / 2.f, me.position.y + me.size.y / 2.f });
    menuText.setPosition({ width / 2.f, height * 0.75f });

    // --- SLOTY SAVE / LOAD ---
    float slotWidth = 480.f;  // szerszy slot
    float slotHeight = 70.f;
    float slotSpacing = 90.f;
    float slotStartY = height * 0.4f;

    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape slot({ slotWidth, slotHeight });
        slot.setFillColor(sf::Color(50, 50, 50));
        slot.setOutlineThickness(3.f);
        slot.setOutlineColor(sf::Color::White);
        slot.setOrigin({ slotWidth / 2.f, slotHeight / 2.f });
        slot.setPosition({ width / 2.f, slotStartY + i * slotSpacing });
        slotButtons.push_back(slot);


        sf::Text txt(font);
        txt.setCharacterSize(24);
        txt.setFillColor(sf::Color::White);
        txt.setString("Slot " + std::to_string(i + 1));
        auto b = txt.getLocalBounds();
        txt.setOrigin({
            b.position.x + b.size.x / 2.f,
            b.position.y + b.size.y / 2.f
            });
        txt.setPosition(slot.getPosition());
        slotTexts.push_back(txt);
    }
    backButton.setSize({ 360.f, 60.f });
    backButton.setFillColor(sf::Color(50, 50, 50));
    backButton.setOutlineThickness(3.f);
    backButton.setOutlineColor(sf::Color::White);
    backButton.setOrigin({ 180.f, 30.f });
    backButton.setPosition({ width / 2.f, height * 0.75f });

    backText.setFont(font);
    backText.setString("BACK");
    backText.setCharacterSize(32);
    backText.setFillColor(sf::Color::White);

    auto bb = backText.getLocalBounds();
    backText.setOrigin({
        bb.position.x + bb.size.x / 2.f,
        bb.position.y + bb.size.y / 2.f
        });
    backText.setPosition(backButton.getPosition());




}

void PauseMenu::draw(sf::RenderWindow& window) {
    window.draw(background);

    if (currentScreen == PauseScreen::MAIN) {
        window.draw(title);
        window.draw(resumeText);
        window.draw(restartText);
        window.draw(saveText);
        window.draw(loadText);
        window.draw(menuText);
    }
    else {
        // ekran slotów
        window.draw(title);
        for (int i = 0; i < slotButtons.size(); ++i) {
            window.draw(slotButtons[i]);
            window.draw(slotTexts[i]);
        }

        window.draw(backButton);
        window.draw(backText);


    }
}
void PauseMenu::handleHover(sf::Vector2i mousePos) {
    sf::Vector2f m((float)mousePos.x, (float)mousePos.y);

    if (currentScreen == PauseScreen::MAIN) {
        resumeText.setFillColor(resumeText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        restartText.setFillColor(restartText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        saveText.setFillColor(saveText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        loadText.setFillColor(loadText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        menuText.setFillColor(menuText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        backText.setFillColor(backText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);

    }
    else {
        for (int i = 0; i < slotButtons.size(); ++i) {
            slotButtons[i].setFillColor(
                slotButtons[i].getGlobalBounds().contains(m)
                ? sf::Color(80, 80, 80)
                : sf::Color(50, 50, 50)
            );
        }

        backButton.setFillColor(
            backButton.getGlobalBounds().contains(m)
            ? sf::Color(80, 80, 80)
            : sf::Color(50, 50, 50)
        );

    }
}

// przyciski główne
bool PauseMenu::resumeClicked(sf::Vector2i mousePos) {
    return resumeText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

bool PauseMenu::restartClicked(sf::Vector2i mousePos) {
    return restartText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

bool PauseMenu::menuClicked(sf::Vector2i mousePos) {
    return menuText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

bool PauseMenu::saveClicked(sf::Vector2i mousePos) {
    return saveText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

bool PauseMenu::loadClicked(sf::Vector2i mousePos) {
    return loadText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

// przyciski slotów
int PauseMenu::slotClicked(sf::Vector2i mousePos) {
    sf::Vector2f m((float)mousePos.x, (float)mousePos.y);
    for (int i = 0; i < slotButtons.size(); ++i) {
        if (slotButtons[i].getGlobalBounds().contains(m))
            return i + 1;
    }
    return 0;
}

void PauseMenu::setSlotText(int index, const std::string& text) {
    if (index < 1 || index > slotTexts.size()) return;

    auto& t = slotTexts[index - 1];
    t.setString(text);

    auto b = t.getLocalBounds();
    t.setOrigin({
        b.position.x + b.size.x / 2.f,
        b.position.y + b.size.y / 2.f
        });
}


bool PauseMenu::backClicked(sf::Vector2i mousePos) {
    return backText.getGlobalBounds().contains(
        sf::Vector2f((float)mousePos.x, (float)mousePos.y)
    );
}


