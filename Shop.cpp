#include "Shop.h"
#include "Menu.h"

ShopMenu::ShopMenu(float width, float height, sf::Font& font)
    : title(font)
    , resumeText(font)
    , restartText(font)
    , menuText(font)
    , saveText(font)
    , loadText(font)
    , slotButtons()
    , backText(font)
{
    // pó³przezroczyste t³o
    background.setSize({ width, height });
    background.setFillColor(sf::Color(0, 0, 0, 150));

    // tytu³
    title.setString("SHOP");
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

    // SLOTY – wyrównane do tego samego X co przyciski pauzy
    for (int i = 0; i < 3; ++i)
    {
        sf::Text slot(font);
        slot.setString("Slot " + std::to_string(i + 1));
        slot.setCharacterSize(35);
        slot.setFillColor(sf::Color::White);

        slot.setPosition({ width / 2.f, height * 0.4f + i * 70.f });

        slotButtons.push_back(slot);
    }

    // tekst BACK – ta sama pozycja co prostok¹t
    backText.setFont(font);
    backText.setString("BACK");
    backText.setCharacterSize(40);
    backText.setFillColor(sf::Color::White);

    auto bb = backText.getLocalBounds();
    backText.setOrigin({
        bb.position.x + bb.size.x / 2.f,
        bb.position.y + bb.size.y / 2.f
        });

    // pod slotami
    backText.setPosition({ width / 2.f, height * 0.75f });



}

void ShopMenu::draw(sf::RenderWindow& window) {
    window.draw(background);

    if (currentScreen == ShopScreen::MAIN) {
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
        for (auto& s : slotButtons)
            window.draw(s);

        if (currentScreen != ShopScreen::MAIN) {
            for (auto& s : slotButtons)
                window.draw(s);

            window.draw(backText);
        }

    }
}
void ShopMenu::handleHover(sf::Vector2i mousePos) {
    sf::Vector2f m((float)mousePos.x, (float)mousePos.y);

    if (currentScreen == ShopScreen::MAIN) {
        resumeText.setFillColor(resumeText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        restartText.setFillColor(restartText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        saveText.setFillColor(saveText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        loadText.setFillColor(loadText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        menuText.setFillColor(menuText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
        backText.setFillColor(backText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);

    }
    else {
        for (auto& s : slotButtons)
            s.setFillColor(s.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White);
    }
}

// przyciski g³ówne
bool ShopMenu::resumeClicked(sf::Vector2i mousePos) {
    return resumeText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

bool ShopMenu::restartClicked(sf::Vector2i mousePos) {
    return restartText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

bool ShopMenu::menuClicked(sf::Vector2i mousePos) {
    return menuText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

bool ShopMenu::saveClicked(sf::Vector2i mousePos) {
    return saveText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

bool ShopMenu::loadClicked(sf::Vector2i mousePos) {
    return loadText.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
}

// przyciski slotów
int ShopMenu::slotClicked(sf::Vector2i mousePos) {
    for (int i = 0; i < slotButtons.size(); ++i) {
        if (slotButtons[i].getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y))) {
            return i + 1;
        }
    }
    return 0;
}
void ShopMenu::setSlotText(int index, const std::string& text) {
    if (index < 1 || index > slotButtons.size()) return;

    auto& s = slotButtons[index - 1];
    s.setString(text);

    auto b = s.getLocalBounds();
    s.setOrigin({
        b.position.x + b.size.x / 2.f,
        b.position.y + b.size.y / 2.f
        });
}

bool ShopMenu::backClicked(sf::Vector2i mousePos) {
    return backText.getGlobalBounds().contains(
        sf::Vector2f((float)mousePos.x, (float)mousePos.y)
    );
}



