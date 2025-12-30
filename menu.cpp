#include "menu.h"
#include <vector>
#include <iostream>

Menu::Menu(float width, float height, sf::Font& arial)
	: title(arial)
	, startText(arial)
	, backgroundSpirite(backgroundTexture)
{ 
	if (!backgroundTexture.loadFromFile("assets/menugraf.png")) {
		std::cout << "Nie zaladowalo tla" <<std::endl;
	}
	//tlo
	backgroundSpirite.setTexture(backgroundTexture);
	sf::Vector2u texturesize = backgroundTexture.getSize();
	float scalex = width / (float)texturesize.x;
	float scaley = height / (float)texturesize.y;
	backgroundSpirite.setScale({scalex, scaley});


	//tytul
	title.setString("TOWER DEFFENCE");
	title.setCharacterSize(80);
	title.setFillColor(sf::Color(0,0,128));
	title.setStyle(sf::Text::Bold | sf::Text::Italic);

		//wysrodkowanie dla title
		auto b = title.getLocalBounds();
		title.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
		title.setPosition({ width / 2.f, height* 0.25f });

		startButton.setSize({ 250.f, 70.f });	//wielokosc przycisku
		startButton.setOutlineThickness(3.f);
		startButton.setOutlineColor(sf::Color::Black);

		//wysrodkowanie guzika
		auto c = startButton.getLocalBounds();
		startButton.setOrigin({ c.position.x + c.size.x / 2.f, c.position.y + c.size.y / 2.f });
		startButton.setPosition({ width / 2.f, height / 2.f });


	//tekst na przycisku
	startText.setString("START");
	startText.setCharacterSize(40);
	startText.setFillColor(sf::Color::White);
	startText.setStyle(sf::Text::Bold);



	//wysrodkowanie napisu na guziku
	auto d = startText.getLocalBounds();
	startText.setOrigin({ d.position.x + d.size.x / 2.f, d.position.y + d.size.y / 2.f });
	startText.setPosition({ width / 2.f, height / 2.f });

}
void Menu::draw(sf::RenderWindow& window) {
	window.draw(backgroundSpirite);
	window.draw(startButton);
	window.draw(startText);
	window.draw(title);
}
void Menu::handleHover(sf::Vector2i mousepos){
	sf::Vector2f mousePosF((int)mousepos.x, (int)mousepos.y);
	
	//zmiana koloru po najechaniu
	if (startButton.getGlobalBounds().contains(mousePosF)) {
		startButton.setFillColor(sf::Color::Cyan);
	}
	else {//jesli myszka nie jest na nim podstawowy kolor
		startButton.setFillColor(sf::Color::Blue);
	}
}
bool Menu::isStartClicked(sf::Vector2i mousepos) {
	sf::Vector2f mousePosF((int)mousepos.x, (int)mousepos.y);

	return startButton.getGlobalBounds().contains(mousePosF) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
