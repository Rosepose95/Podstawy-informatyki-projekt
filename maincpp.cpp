#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;


int main() {
    RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML 3 Dziala!");
    CircleShape shape(100.f);
    shape.setFillColor(Color::Green);

    while (window.isOpen()) {
        // W SFML 3 pollEvent zwraca std::optional, nie przyjmuje argumentu
        while (const optional event = window.pollEvent()) {
            // Sprawdzamy typ zdarzenia za pomoc¹ metody .is<>()
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}