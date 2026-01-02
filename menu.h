
#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>

class Menu {
private:

	sf::Text title;
	sf::Text startText;
	sf::Text exitText;
	sf::Text editText;
	sf::RectangleShape startButton;
	sf::RectangleShape exitButton;//
	sf::RectangleShape editButton;

	sf::Texture backgroundTexture;	//dla grafiki na tle
	sf::Sprite backgroundSpirite;

public:
	Menu(float width, float height, sf::Font& arial);
	void draw(sf::RenderWindow& window);
	bool isStartClicked(sf::Vector2i mousepos);
	bool isExitClicked(sf::Vector2i mousepos);
	bool isEditClicked(sf::Vector2i mousepos);

	void handleHover(sf::Vector2i mousepos);	//podswietlanie jak sie najedzie

};
#endif

