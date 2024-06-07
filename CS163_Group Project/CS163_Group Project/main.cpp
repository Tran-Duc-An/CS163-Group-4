#include <SFML/Graphics.hpp>
#include "UI.h"
#include <iostream>

sf::RenderWindow window(sf::VideoMode(1500, 800), "Dictionary");
sf::Event event;
Button VNtoEnButton(520, 218, "Image/VNtoENButton.png");
Button ENtoVnButton(520, 340, "Image/ENtoVNButton.png");
InputBox inputVNBox(80, 250, "Image/InputBox.png", "Nhap tai day");
InputBox inputENBox(80, 250, "Image/InputBox.png", "Text here");
SubmitButton translate(695, 242, "Image/translateButton.png");

int page = 0;
void setBackground() {
	sf::Texture texture;
	if (!texture.loadFromFile("Image/background.png")) {

	}
	sf::Sprite sprite;
	texture.setSmooth(1);
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);
	window.draw(sprite);
}

void VNtoEnglish() {
	sf::Texture vnmese;
	if (!vnmese.loadFromFile("Image/Vntag.png")) {

	}
	sf::Sprite VNtag;
	vnmese.setSmooth(1);
	VNtag.setTexture(vnmese);
	VNtag.setPosition(51, 77);
	window.draw(VNtag);

	sf::Texture eng;
	if (!eng.loadFromFile("Image/ENtag.png")) {

	}
	eng.setSmooth(1);
	sf::Sprite ENtag;
	ENtag.setTexture(eng);
	ENtag.setPosition(863, 77);
	window.draw(ENtag);

	std::string word;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) window.close();
		inputVNBox.isClicked(window, event);
		if (translate.isClicked(window, event, word, inputVNBox.text)) std::cout << word;
	}
	translate.draw(window);
	inputVNBox.draw(window);

}

void ENtoVietnames() {
	sf::Texture vnmese;
	if (!vnmese.loadFromFile("Image/Vntag.png")) {

	}
	sf::Sprite VNtag;
	vnmese.setSmooth(1);
	VNtag.setTexture(vnmese);
	VNtag.setPosition(863, 77);
	window.draw(VNtag);

	sf::Texture eng;
	if (!eng.loadFromFile("Image/ENtag.png")) {

	}
	eng.setSmooth(1);
	sf::Sprite ENtag;
	ENtag.setTexture(eng);
	ENtag.setPosition(51, 77);
	window.draw(ENtag);

	std::string word;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) window.close();
		inputENBox.isClicked(window, event);
		if (translate.isClicked(window, event, word, inputENBox.text)) std::cout << word;
	}
	translate.draw(window);
	inputENBox.draw(window);

}


void homePage() {
	VNtoEnButton.draw(window);
	ENtoVnButton.draw(window);
	while (window.pollEvent(event)) {
		if (VNtoEnButton.isClicked(window, event)) page = 1;
		if (ENtoVnButton.isClicked(window, event)) page = 2;
	}
}

int main() {
	while (window.isOpen()) {
		setBackground();
		switch (page)
		{
		case 0: {
			homePage();
			break;
		}
		case 1: {
			VNtoEnglish();
			break;
		}
		case 2: {
			ENtoVietnames();
			break;
		}
		default:
			break;
		}
		window.display();
		window.clear(sf::Color::White);
	}
}