#include "menu.h"
#include <vector>
#include <iostream>

Menu::Menu(float width, float height, sf::Font& arial)
	: title(arial)
	, startText(arial)
	, exitText(arial)
	, editText(arial)
	, backgroundSpirite(backgroundTexture)
{ 
	if (!backgroundTexture.loadFromFile("assets/menugraf.png")) {
		std::cout << "Nie zaladowalo tla" <<std::endl;
	}

	//tlo
	backgroundSpirite.setTexture(backgroundTexture, true);
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

		//przycisk startButton
		startButton.setSize({ 250.f, 70.f });	
		startButton.setOutlineThickness(3.f);
		startButton.setOutlineColor(sf::Color::Black);

		//przycisk exitButton
		exitButton.setSize({ 250.f, 70.f });	
		exitButton.setOutlineThickness(3.f);
		exitButton.setOutlineColor(sf::Color::Black);

		//przycisk editButton
		editButton.setSize({ 250.f, 70.f });
		editButton.setOutlineThickness(3.f);
		editButton.setOutlineColor(sf::Color::Black);

		//wysrodkowanie guzika startButton
		auto c = startButton.getLocalBounds();
		startButton.setOrigin({ c.position.x + c.size.x / 2.f, c.position.y + c.size.y / 2.f });
		startButton.setPosition({ width / 2.f, height / 2.f });

		//wysrodkowanie guzika exitButton
		auto d = exitButton.getLocalBounds();
		exitButton.setOrigin({ d.position.x + d.size.x / 2.f, d.position.y + d.size.y / 2.f });
		exitButton.setPosition({ width / 2.f, height * 0.85f });

		//wysrodkowanie guzika editButton
		auto g = editButton.getLocalBounds();
		editButton.setOrigin({ g.position.x + g.size.x / 2.f, g.position.y + g.size.y / 2.f });
		editButton.setPosition({ width / 2.f, height * 0.65f });

	//tekst na przycisku startButton
	startText.setString("START");
	startText.setCharacterSize(40);
	startText.setFillColor(sf::Color::White);
	startText.setStyle(sf::Text::Bold);

	//tekst na przycisku exitButton
	exitText.setString("EXIT");
	exitText.setCharacterSize(40);
	exitText.setFillColor(sf::Color::White);
	exitText.setStyle(sf::Text::Bold);

	//tekst na przycisku editButton
	editText.setString("Make your map");
	editText.setCharacterSize(30);
	editText.setFillColor(sf::Color::White);
	editText.setStyle(sf::Text::Bold);

	//wysrodkowanie napisu na guziku startButton
	auto e = startText.getLocalBounds();
	startText.setOrigin({ e.position.x + e.size.x / 2.f, e.position.y + e.size.y / 2.f });
	startText.setPosition({ width / 2.f, height / 2.f });

	//wysrodkowanie napisu na guziku exitButton
	auto f = exitText.getLocalBounds();
	exitText.setOrigin({ f.position.x + f.size.x / 2.f, f.position.y + f.size.y / 2.f });
	exitText.setPosition({ width / 2.f, height * 0.85f });

	//wysrodkowanie napisu na guziku exitButton
	auto h = editText.getLocalBounds();
	editText.setOrigin({ h.position.x + h.size.x / 2.f, h.position.y + h.size.y / 2.f });
	editText.setPosition({ width / 2.f, height * 0.65f });

}
void Menu::draw(sf::RenderWindow& window) {
	window.draw(backgroundSpirite);
	window.draw(startButton);
	window.draw(startText);
	window.draw(title);
	window.draw(exitButton);
	window.draw(exitText);
	window.draw(editButton);
	window.draw(editText);
}
void Menu::handleHover(sf::Vector2i mousepos){
	sf::Vector2f mousePosF((int)mousepos.x, (int)mousepos.y);
	
	//zmiana koloru po najechaniu startButton
	if (startButton.getGlobalBounds().contains(mousePosF)) {
		startButton.setFillColor(sf::Color::Cyan);
	}
	else {//jesli myszka nie jest na nim podstawowy kolor
		startButton.setFillColor(sf::Color::Blue);
	}

	if (exitButton.getGlobalBounds().contains(mousePosF)) {
		exitButton.setFillColor(sf::Color::Cyan);
	}
	else {
		exitButton.setFillColor(sf::Color::Blue);
	}

	if (editButton.getGlobalBounds().contains(mousePosF)) {
		editButton.setFillColor(sf::Color::Cyan);
	}
	else {
		editButton.setFillColor(sf::Color::Blue);
	}
}
bool Menu::isStartClicked(sf::Vector2i mousepos) {
	sf::Vector2f mousePosF((int)mousepos.x, (int)mousepos.y);

	return startButton.getGlobalBounds().contains(mousePosF) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
bool Menu::isExitClicked(sf::Vector2i mousepos) {
	sf::Vector2f mousePosF((int)mousepos.x, (int)mousepos.y);

	return exitButton.getGlobalBounds().contains(mousePosF) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
bool Menu::isEditClicked(sf::Vector2i mousepos) {
	sf::Vector2f mousePosF((int)mousepos.x, (int)mousepos.y);

	return editButton.getGlobalBounds().contains(mousePosF) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}
