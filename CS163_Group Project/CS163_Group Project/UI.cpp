#include "UI.h"
#include <SFML/Graphics.hpp>
#include <string>


Button::Button(int x, int y, std::string imagePath) {
	if (texture.loadFromFile(imagePath)) {
		texture.setSmooth(1);
		sprite.setTexture(texture);
		sprite.setPosition(x, y);
	}
}

bool Button::isClicked(sf::RenderWindow& window, sf::Event event) {
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) return 1;
	}
	return 0;
}

void Button::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}

InputBox::InputBox(int x, int y, std::string imagePath, std::string name) : Button(x, y, imagePath) {
	if (!font.loadFromFile("Font/ARIAL.TTF")) {

	}
	if (!texture.loadFromFile(imagePath)) {

	}

	nameHolder.setString(name);
	nameHolder.setFont(font);
	nameHolder.setCharacterSize(40);
	nameHolder.setPosition(x + 10, y + 10);
	nameHolder.setFillColor(sf::Color::Black);
	nameHolder.setStyle(sf::Text::Italic);

	texture.setSmooth(1);
	sprite.setTexture(texture);
	sprite.setPosition(x, y);

	text.setFont(font);
	text.setCharacterSize(40);
	text.setPosition(x + 10, y + 10);
	text.setFillColor(sf::Color::Black);

	textDisplay = text;
}


void InputBox::isClicked(sf::RenderWindow& window, sf::Event& event) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) active = 1;
		else active = 0;
	}

	if (active) {
		std::string str = text.getString();
		if (str.length() < 20) {
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode > 31 && event.text.unicode < 128) {
					text.setString(text.getString() + static_cast<char>(event.text.unicode));
				}
			}
		}
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::BackSpace) {
				std::string str = text.getString().toAnsiString();
				if (!str.empty()) {
					str.pop_back();
					text.setString(str);
				}
			}
		}
	}
}

void InputBox::draw(sf::RenderWindow& window) {
	window.draw(sprite);
	std::string str = text.getString().toAnsiString();
	if (str.empty() && active == 0) {
		window.draw(nameHolder);
	}
	if (active) {
		textDisplay.setString(text.getString() + "|");
		window.draw(textDisplay);
	}
	else {
		window.draw(text);
	}
}

InputDef::InputDef(int x, int y, std::string imagePath, std::string name) : InputBox(x, y, imagePath, name) {

}



SubmitButton::SubmitButton(int x, int y, std::string imagePath) :Button(x, y, imagePath) {
}

bool SubmitButton::isClicked(sf::RenderWindow& window, sf::Event& event, std::string& word, sf::Text& text) {
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos)))
		{
			word = text.getString().toAnsiString();
			return 1;
		}
	}
	else {
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Enter)) {
			word = text.getString().toAnsiString();
			return 1;
		}
	}
	return 0;
}

void InputDef::isClicked(sf::RenderWindow& window, sf::Event& event) {

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) active = 1;
		else active = 0;
	}

	if (active) {
		std::string str = text.getString();
		if (str.length() / 20 < 7) {
			if (str.length() % 20 == 0 && str.length() > 0) text.setString(text.getString() + '\n');
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode > 31 && event.text.unicode < 128) {
					text.setString(text.getString() + static_cast<char>(event.text.unicode));
				}
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::BackSpace) {
					std::string str = text.getString().toAnsiString();
					if (!str.empty()) {
						str.pop_back();
						text.setString(str);
					}
				}
			}
		}
	}
}

void InputDef::draw(sf::RenderWindow& window) {
	window.draw(sprite);
	std::string str = text.getString().toAnsiString();
	if (str.empty() && active == 0) {
		window.draw(nameHolder);
	}
	if (active) {
		textDisplay.setString(text.getString() + "|");
		window.draw(textDisplay);
	}
	else {
		window.draw(text);
	}
}