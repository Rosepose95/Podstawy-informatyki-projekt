#include "PauseMenu.h"

PauseMenu::PauseMenu(float width, float height, sf::Font& font)
    : title(font)
    , resumeText(font)
    , restartText(font)
    , menuText(font)
{
    // pó³przezroczyste t³o
    background.setSize({ width, height });
    background.setFillColor(sf::Color(0, 0, 0, 150));

    // tytu³
    title.setString("PAUSED");
    title.setCharacterSize(70);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);

    auto tb = title.getLocalBounds();
    title.setOrigin({ tb.position.x + tb.size.x / 2.f, tb.position.y + tb.size.y / 2.f });
    title.setPosition({ width / 2.f, height * 0.25f });

    // RESUME
    resumeText.setString("RESUME");
    resumeText.setCharacterSize(40);
    resumeText.setPosition({ width / 2.f - 80.f, height * 0.45f });

    // RESTART
    restartText.setString("RESTART");
    restartText.setCharacterSize(40);
    restartText.setPosition({ width / 2.f - 90.f, height * 0.55f });

    // MENU
    menuText.setString("MENU");
    menuText.setCharacterSize(40);
    menuText.setPosition({ width / 2.f - 60.f, height * 0.65f });
}

void PauseMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(title);
    window.draw(resumeText);
    window.draw(restartText);
    window.draw(menuText);
}

void PauseMenu::handleHover(sf::Vector2i mousePos) {
    sf::Vector2f m((float)mousePos.x, (float)mousePos.y);

    resumeText.setFillColor(
        resumeText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White
    );

    restartText.setFillColor(
        restartText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White
    );

    menuText.setFillColor(
        menuText.getGlobalBounds().contains(m) ? sf::Color::Yellow : sf::Color::White
    );
}

bool PauseMenu::resumeClicked(sf::Vector2i mousePos) {
    return resumeText.getGlobalBounds().contains(
        sf::Vector2f((float)mousePos.x, (float)mousePos.y)
    );
}

bool PauseMenu::restartClicked(sf::Vector2i mousePos) {
    return restartText.getGlobalBounds().contains(
        sf::Vector2f((float)mousePos.x, (float)mousePos.y)
    );
}

bool PauseMenu::menuClicked(sf::Vector2i mousePos) {
    return menuText.getGlobalBounds().contains(
        sf::Vector2f((float)mousePos.x, (float)mousePos.y)
    );
}
