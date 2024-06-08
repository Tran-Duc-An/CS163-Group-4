#pragma once
#include <SFML/Graphics.hpp>
struct Button {
	sf::Texture texture;
	sf::Sprite sprite;
	Button(int x, int y, std::string imagePath);
	bool isClicked(sf::RenderWindow& window, sf::Event event);
	void draw(sf::RenderWindow& window);
};



struct InputBox : Button {
	sf::Text text;
	sf::Font font;
	sf::Text textDisplay;
	sf::Text nameHolder;
	std::string name;
	bool active = 0;
	InputBox(int x, int y, std::string imagePath, std::string name);
	void isClicked(sf::RenderWindow& window, sf::Event& event);
	void draw(sf::RenderWindow& window);
};

struct SubmitButton : Button {
	bool isClicked(sf::RenderWindow& window, sf::Event& event, std::string& word, sf::Text& text);
	SubmitButton(int x, int y, std::string imagePath);
};

struct InputDef : InputBox {
	InputDef(int x, int y, std::string imagePath, std::string name);
	void draw(sf::RenderWindow& window);
	void isClicked(sf::RenderWindow& window, sf::Event& event);
};