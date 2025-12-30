#include <SFML/Graphics.hpp>

class Menu {


private:

	sf::Text title;
	sf::Text startText;
	sf::RectangleShape startButton;
	sf::Sprite backgroundSpirite;
	sf::Texture backgroundTexture;	//dla grafiki na tle

public:
	Menu(float width, float height, sf::Font& arial);
	void draw(sf::RenderWindow& window);
	bool isStartClicked(sf::Vector2i mousepos);
	void handleHover(sf::Vector2i mousepos);	//podswietlanie jak sie najedzie

};