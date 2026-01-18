
#include "Game.h"
#include "Enemy.h"
#include "Tower.h"
#include "Map.h"
#include "Menu.h"
#include "PauseMenu.h"
#include <SFML/Graphics.hpp>
#include "Shop.h"
// --- stany gry ---
enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    EDITOR // dodany z pierwszego kodu
};

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1240, 840 }), "Tower Defense SFML 3");

    Game game;
    Map map;
    game.setMap(&map);

    sf::Font font;
    font.openFromFile("assets/ArialMT.ttf");
    Menu menu(1240.f, 840.f, font);
    PauseMenu pauseMenu(1240.f, 840.f, font);

    GameState state = GameState::MENU;      // aktualny stan gry
    GameState nextState = GameState::PLAYING; // stan po animacji przycisku

    sf::Clock clock;

    // --- zmienne animacji przycisku pauzy ---
    bool pauseClicked = false;
    sf::Clock clickClock;
    float clickDuration = 0.1f; // czas trwania animacji

    // --- PRZYCISK PAUZY ---
    sf::RectangleShape pauseButton({ 40.f, 40.f });
    pauseButton.setPosition({ 1240.f - 50.f, 10.f });
    sf::Color pauseNormal(200, 200, 200);
    sf::Color pauseHover(170, 170, 170);
    pauseButton.setFillColor(pauseNormal);

    sf::Texture pauseTexture;
    pauseTexture.loadFromFile("assets/pause.png");
    sf::Sprite pauseIcon(pauseTexture);
    pauseIcon.setScale({ 0.8f, 0.8f });
    pauseIcon.setPosition({ 1240.f - 46.f, 14.f });

    sf::Texture playTexture;
    playTexture.loadFromFile("assets/play.png");
    sf::Sprite playIcon(playTexture);
    playIcon.setScale({ 0.8f, 0.8f });
    playIcon.setPosition({ 1240.f - 46.f, 14.f });

    sf::RectangleShape pauseOverlay({ 1240.f, 840.f });
    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // półprzezroczyste tło pauzy

    // --- HUD edytora mapy ---
    sf::Text editorHUD(font);
    editorHUD.setString(
        "Esc - powrot do MENU\n"
        "ENTER - start gry na stworzonej mapie\n"
        "1 - Trawa\n"
        "2 - Droga\n"
        "3 - Meta\n"
        "4 - Wieza Startowa\n"
        "LPM - Wstawianie"
    );
    editorHUD.setCharacterSize(20);
    editorHUD.setPosition({ 20.f, 20.f });
    editorHUD.setFillColor(sf::Color::Black);
    editorHUD.setStyle(sf::Text::Bold);

    sf::RectangleShape hudBG;
    hudBG.setSize(sf::Vector2f(80.f, 80.f));
    hudBG.setPosition({ 1100.f, 10.f });
    hudBG.setOutlineThickness(5.f);
    hudBG.setOutlineColor(sf::Color::Black);

    bool canPaint = false;
    char currentBrush = '#';
    auto updateBrushPreview = [&]() {
        const sf::Texture* tex = nullptr;

        if (currentBrush == 'T') {
            tex = &game.getTowerTexture(0);   // startowa wieża do podglądu
        }
        else {
            tex = &map.getBrushTexture(currentBrush); // trawa/path/meta itd.
        }
        hudBG.setFillColor(sf::Color::White);
        hudBG.setTexture(tex, true);
        };

    while (window.isOpen()) {
        sf::Vector2i mousepos = sf::Mouse::getPosition(window);

        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }
            // --- KLIKNIĘCIA GAME OVER ---
            if (game.isGameOver()) {
                if (const auto* mouse = ev->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f clickPos = window.mapPixelToCoords(mouse->position);
                        game.tryRestart(clickPos);//restart gry
                        //aby nie stawialo wiezy podczas klikniecia restart
                        if (!game.isGameOver()) {
                            continue;
                        }
                        if (game.tryExit(clickPos))  // wyjście z gry
                            window.close();
                    }
                }
            }


            // --- obsługa przycisku pauzy ---
            if (state != GameState::MENU) {
                if (const auto* mousePressed = ev->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mousePressed->button == sf::Mouse::Button::Left) {
                        sf::Vector2f clickPos = window.mapPixelToCoords(mousePressed->position);
                        if (pauseButton.getGlobalBounds().contains(clickPos)) {
                            pauseClicked = true;
                            clickClock.restart();
                            nextState = (state == GameState::PLAYING)
                                ? GameState::PAUSED
                                : GameState::PLAYING;
                        }
                    }
                }
            }

            // --- MENU GŁÓWNE ---
            if (state == GameState::MENU) {
                if (const auto* mouse = ev->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {

                        sf::Vector2f clickPos = window.mapPixelToCoords(mouse->position);

                        // --- EKRAN LOAD ---
                        if (menu.isInLoadScreen()) {
                            int slot = menu.slotClicked(mousepos);
                            if (slot > 0 && game.saveExists(slot)) {
                                game.loadGame(slot);
                                state = GameState::PLAYING;
                                menu.exitLoadScreen();
                            }
                            else if (menu.backClicked(mousepos)) {
                                menu.exitLoadScreen();
                            }
                        }
                        else { // MENU GŁÓWNE
                            if (menu.isStartClicked(mousepos)) {
                                game.isCustomMap = false;
                                map.loadMap();
                                map.refreshLogic();
                                state = GameState::PLAYING;
                                game.startGame();
                            }
                            else if (menu.isLoadClicked(mousepos)) {
                                menu.enterLoadScreen();
                            }
                            else if (menu.isExitClicked(mousepos)) {
                                window.close();
                            }
                            else if (menu.isEditClicked(mousepos)) { // tryb edytora
                                state = GameState::EDITOR;
                                map.clearMap();
                                canPaint = false;
                                updateBrushPreview();
                            }
                        }
                    }
                }
            }




            // --- TRYB EDITORA ---
            else if (state == GameState::EDITOR) {
                if (const auto* key = ev->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Num1) { currentBrush = '.'; updateBrushPreview(); }
                    if (key->code == sf::Keyboard::Key::Num2) { currentBrush = '#'; updateBrushPreview(); }
                    if (key->code == sf::Keyboard::Key::Num3) { currentBrush = '*'; updateBrushPreview(); }
                    if (key->code == sf::Keyboard::Key::Num4) { currentBrush = 'T'; updateBrushPreview(); }


                    if (key->code == sf::Keyboard::Key::Enter) {

                        game.isCustomMap = true;

                        int ts = map.tileSize;
                        bool TowerFound = false;

                        for (int i = 0; i < map.getHeight(); i++) {
                            for (int j = 0; j < map.getWidth(); j++) {

                                if (map.getTile(j, i) == 'T') {
                                    float centerx = j * ts + ts / 2.f;
                                    float centery = (i + 1) * ts;

                                    game.setStartTPos({ centerx, centery });

                                    TowerFound = true;

                                    map.setTile(j, i, '.');
                                }
                            }
                        }
                        if (!TowerFound) {
                            game.setStartTPos({ 14 * ts + ts / 2.f, 11 * ts + ts / 2.f });
                        }
                        map.refreshLogic();
                        game.startGame();
                        state = GameState::PLAYING;

                    }
                    if (key->code == sf::Keyboard::Key::Escape) {
                        state = GameState::MENU;
                    }
                }
                if (const auto* mouse = ev->getIf<sf::Event::MouseButtonPressed>()) {   //kiedy klikamy guzik zamalowuje kratke
                    if (mouse->button == sf::Mouse::Button::Left)
                        canPaint = true;
                }
                if (const auto* mouse = ev->getIf<sf::Event::MouseButtonReleased>()) {  //kiedy guzik odpusczamy nie maluje
                    if (mouse->button == sf::Mouse::Button::Left)
                        canPaint = false;
                }
                auto updateBrushPreview = [&]() {
                    const sf::Texture* tex = nullptr;

                    if (currentBrush == 'T') {
                        tex = &game.getTowerTexture(0); // preview startowej wieży
                    }
                    else {
                        tex = &map.getBrushTexture(currentBrush); // trawa/path/meta/woda
                    }
                    hudBG.setTexture(tex, true);

                    hudBG.setFillColor(sf::Color::White); 
                    hudBG.setTexture(tex, true);          
                    };
            }

            // --- ROZGRYWKA ---
            else if (state == GameState::PLAYING && !game.isGameOver()) {

                // stawianie wież
                if (const auto* mouse = ev->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f clickPos = window.mapPixelToCoords(mouse->position);

                        if (game.isClickOnUI(clickPos)) {
                            game.tryTowerType(clickPos);
                        }
                        else if (pauseButton.getGlobalBounds().contains(clickPos)) {}
                        else {
                            int tileSize = map.tileSize;
                            int tileX = static_cast<int>(clickPos.x) / tileSize;
                            int tileY = static_cast<int>(clickPos.y) / tileSize;
                            float centerX = tileX * tileSize + tileSize / 2.f;
                            float centerY = tileY * tileSize + tileSize / 2.f;

                            game.placeTower({ centerX, centerY });
                        }
                    }
                }
}

            // --- PAUZA ---
            else if (state == GameState::PAUSED) {
                if (ev->is<sf::Event::MouseButtonPressed>()) {
                    if (pauseMenu.currentScreen == PauseScreen::MAIN) {
                        if (pauseMenu.resumeClicked(mousepos)) state = GameState::PLAYING;
                        else if (pauseMenu.restartClicked(mousepos)) {
                            game.startGame();
                            state = GameState::PLAYING;
                        }
                        else if (pauseMenu.menuClicked(mousepos)) state = GameState::MENU;
                        else if (pauseMenu.saveClicked(mousepos)) {
                            pauseMenu.currentScreen = PauseScreen::SAVE_SLOTS;
                            for (int i = 1; i <= 3; ++i) {
                                if (!game.saveExists(i)) {
                                    game.saveGame(i);
                                    pauseMenu.currentScreen = PauseScreen::MAIN;
                                    break;
                                }
                            }
                        }
                        else if (pauseMenu.loadClicked(mousepos)) pauseMenu.currentScreen = PauseScreen::LOAD_SLOTS;
                    }
                    else { // ekran slotów
                        int slot = pauseMenu.slotClicked(mousepos);
                        if (pauseMenu.currentScreen == PauseScreen::SAVE_SLOTS && slot > 0) {
                            game.saveGame(slot);
                            pauseMenu.currentScreen = PauseScreen::MAIN;
                        }
                        else if (pauseMenu.currentScreen == PauseScreen::LOAD_SLOTS && slot > 0) {
                            if (game.saveExists(slot)) {
                                game.loadGame(slot);
                                state = GameState::PLAYING;
                                pauseMenu.currentScreen = PauseScreen::MAIN;
                            }
                        }
                        if (pauseMenu.backClicked(mousepos)) pauseMenu.currentScreen = PauseScreen::MAIN;
                    }
                }
            }
            //rysowanie w editorze
            if (state == GameState::EDITOR && canPaint) {

                sf::Vector2f wordlpos = window.mapPixelToCoords(mousepos);

                int ts = map.tileSize;

                int tilex = (int)wordlpos.x / ts;
                int tiley = (int)wordlpos.y / ts;

                // sprawdzamy czy nie wychodzi poza mapę
                if (tilex >= 0 && tilex < map.getWidth() && tiley >= 0 && tiley < map.getHeight()) {

                    // jeśli pędzel to wieża - usuń starą wieżę (stary znak 'T')
                    if (currentBrush == 'T') {
                        for (int i = 0; i < map.getHeight(); i++) {
                            for (int j = 0; j < map.getWidth(); j++) {
                                if (map.getTile(j, i) == 'T') {
                                    map.setTile(j, i, '.');
                                }
                            }
                        }
                    }
                    map.setTile(tilex, tiley, currentBrush);
                }
            }
        }

        float dt = clock.restart().asSeconds();
        if (state == GameState::PLAYING) game.update(dt);
        if (!pauseClicked) {
            pauseButton.setFillColor(
                pauseButton.getGlobalBounds().contains(sf::Vector2f{ static_cast<float>(mousepos.x), static_cast<float>(mousepos.y) })
                ? pauseHover : pauseNormal
            );
        }

        // --- RYSOWANIE ---
        window.clear(sf::Color::White);

        if (state == GameState::MENU) {
            menu.handleHover(mousepos);
            menu.draw(window);
        }
        else if (state == GameState::EDITOR) {
            map.draw(window);
            window.draw(editorHUD);
            window.draw(hudBG);
        }
        else {
            map.draw(window);
            game.draw(window);
            game.drawUI(window);

            if (state == GameState::PAUSED) {
                window.draw(pauseOverlay);
                pauseMenu.handleHover(mousepos);
                pauseMenu.draw(window);

                if (pauseMenu.currentScreen != PauseScreen::MAIN) {
                    for (int i = 1; i <= 3; ++i) {
                        if (game.saveExists(i)) {
                            auto info = game.getSaveInfo(i);
                            pauseMenu.setSlotText(i,
                                "Slot " + std::to_string(i) +
                                " | Wave " + std::to_string(info.wave) +
                                " | HP " + std::to_string(info.health) +
                                " | Gold " + std::to_string(info.gold)
                            );
                        }
                        else {
                            pauseMenu.setSlotText(i, "Slot " + std::to_string(i) + " | EMPTY");
                        }
                    }
                }
            }

            // --- ANIMACJA PRZYCISKU PAUZY ---
            if (pauseClicked) {
                float elapsed = clickClock.getElapsedTime().asSeconds();
                if (elapsed < clickDuration) {
                    pauseButton.setScale({ 0.9f, 0.9f });
                    pauseButton.setFillColor(sf::Color(150, 150, 150));
                    pauseIcon.setScale({ 0.72f, 0.72f });
                    playIcon.setScale({ 0.72f, 0.72f });
                }
                else {
                    pauseClicked = false;
                    pauseButton.setScale({ 1.f, 1.f });
                    pauseButton.setFillColor(pauseNormal);
                    pauseIcon.setScale({ 0.8f, 0.8f });
                    playIcon.setScale({ 0.8f, 0.8f });
                    state = nextState;
                }
            }
            if (game.isGameOver())
                game.HandleHover(mousepos);
            if (!game.isGameOver()) {
                window.draw(pauseButton);
                if (state == GameState::PLAYING) window.draw(pauseIcon);
                else window.draw(playIcon);
            }
        }

        window.display();
    }

    return 0;
}

