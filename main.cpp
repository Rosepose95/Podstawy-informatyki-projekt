#include <SFML/Graphics.hpp> 
#include "Game.h"
#include "Enemy.h"
#include "Tower.h"
#include "Map.h"
#include "Menu.h"
#include "PauseMenu.h"

// stany gry – menu / rozgrywka
enum class GameState {
    MENU,
    PLAYING,
    PAUSED
};


int main() {
    sf::RenderWindow window(
        sf::VideoMode({ 1240,840 }),
        "Tower Defense SFML 3"
    );

    Game game;
    Map map;
    game.setMap(&map);

    sf::Font font;
    font.openFromFile("assets/ArialMT.ttf");
    Menu menu(1240.f, 840.f, font);
    PauseMenu pauseMenu(1240.f, 840.f, font);

    // aktualny stan gry
    GameState state = GameState::MENU;

    sf::Clock clock;

    // ===== zmienne do animacji przycisku pauzy =====
    bool pauseClicked = false;
    sf::Clock clickClock;
    float clickDuration = 0.1f; // czas trwania animacji w sekundach

    // PRZYCISK PAUZY (UI)
    sf::RectangleShape pauseButton({ 40.f, 40.f });
    pauseButton.setFillColor(sf::Color(200, 200, 200));
    pauseButton.setPosition({ 1240.f - 50.f, 10.f });

    sf::Text pauseText(font);
    pauseText.setString("||");
    pauseText.setCharacterSize(24);
    pauseText.setFillColor(sf::Color::Black);
    pauseText.setPosition({ 1240.f - 40.f, 12.f });
    sf::Color pauseNormal(200, 200, 200);
    sf::Color pauseHover(170, 170, 170);

    pauseButton.setFillColor(pauseNormal);

    while (window.isOpen()) {

        sf::Vector2i mousepos = sf::Mouse::getPosition(window);
        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }

            // ===== kliknięcie przycisku pauzy =====
            if (state == GameState::PLAYING) {
                if (const auto* mousePressed = ev->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mousePressed->button == sf::Mouse::Button::Left) {
                        sf::Vector2f clickPos = window.mapPixelToCoords(mousePressed->position);
                        if (pauseButton.getGlobalBounds().contains(clickPos)) {
                            pauseClicked = true;
                            clickClock.restart();
                        }
                    }
                }
            }


            // MENU GŁÓWNE
            if (state == GameState::MENU) {
                if (ev->is<sf::Event::MouseButtonPressed>()) {
                    if (menu.isStartClicked(mousepos)) {
                        state = GameState::PLAYING;
                        game.startGame();
                        game.startNextWave();
                        int ts = map.tileSize;

                        // STARTOWA WIEŻA
                        game.addTower(
                            Tower(20,
                                14 * ts + ts / 2.f,
                                11 * ts + ts / 2.f)
                        );
                    }
                    else if (menu.isExitClicked(mousepos)) {
                        window.close();
                    }
                }
            }

            else if (state == GameState::PLAYING) {

                /*// PAUZA - ESC
                if (ev->is<sf::Event::KeyPressed>()) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                        state = GameState::PAUSED;
                    }
                }*/

                // STAWIANIE WIEŻ
                if (const auto* mouse =
                    ev->getIf<sf::Event::MouseButtonPressed>()) {

                    if (mouse->button == sf::Mouse::Button::Left) {

                        sf::Vector2f worldPos =
                            window.mapPixelToCoords(mouse->position);

                        int tileSize = map.tileSize;
                        int tileX = static_cast<int>(worldPos.x) / tileSize;
                        int tileY = static_cast<int>(worldPos.y) / tileSize;

                        float centerX = tileX * tileSize + tileSize / 2.f;
                        float centerY = tileY * tileSize + tileSize / 2.f;

                        game.placeTower({ centerX, centerY });
                    }
                }
            }

            // PAUZA
            else if (state == GameState::PAUSED) {

                if (ev->is<sf::Event::MouseButtonPressed>()) {

                    if (pauseMenu.resumeClicked(mousepos)) {
                        state = GameState::PLAYING;
                    }
                    else if (pauseMenu.restartClicked(mousepos)) {
                        game.startGame();
                        game.startNextWave();

                        int ts = map.tileSize;
                        game.addTower(Tower(20, 14 * ts + ts / 2.f, 11 * ts + ts / 2.f));

                        state = GameState::PLAYING;
                    }

                    else if (pauseMenu.menuClicked(mousepos)) {
                        state = GameState::MENU;
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();

        if (state == GameState::PLAYING) {
            game.update(dt);
        }

        // hover tylko jeśli nie kliknięto
        if (!pauseClicked) {
            pauseButton.setFillColor(
                pauseButton.getGlobalBounds().contains(
                    sf::Vector2f{ static_cast<float>(mousepos.x), static_cast<float>(mousepos.y) }
                ) ? pauseHover : pauseNormal
            );
        }

        // RYSOWANIE
        window.clear(sf::Color::White);

        if (state == GameState::MENU) {
            menu.handleHover(mousepos);
            menu.draw(window);
        }
        else {
            // przycisk pauzy
            map.draw(window);
            game.draw(window);
            game.drawUI(window);

            if (state == GameState::PAUSED) {
                pauseMenu.handleHover(mousepos);
                pauseMenu.draw(window);
            }

            // ===== animacja kliknięcia przycisku pauzy =====
            if (pauseClicked) {
                float elapsed = clickClock.getElapsedTime().asSeconds();
                if (elapsed < clickDuration) {
                    // animacja wciśnięcia – zmniejszamy przycisk i zmieniamy kolor
                    pauseButton.setScale(sf::Vector2f(0.9f, 0.9f));
                    pauseButton.setFillColor(sf::Color(150, 150, 150));
                }
                else {
                    pauseClicked = false;
                    pauseButton.setScale(sf::Vector2f(1.f, 1.f));
                    pauseButton.setFillColor(pauseNormal);

                    // po animacji zmieniamy stan gry na pauzę
                    state = GameState::PAUSED;
                }
            }

            window.draw(pauseButton);
            window.draw(pauseText);
        }

        window.display();
    }

    return 0;
}

