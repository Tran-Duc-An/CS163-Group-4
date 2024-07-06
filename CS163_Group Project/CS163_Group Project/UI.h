#pragma once
#pragma once
#include <SFML/Graphics.hpp>

struct Button {
	sf::Texture texture;
	sf::Sprite sprite;
	std::string orgImage;
	Button(int x, int y, std::string imagePath);
	bool isClicked(sf::RenderWindow& window, sf::Event event);
	void draw(sf::RenderWindow& window);
	void isHover(sf::RenderWindow& window,std::string path);
};



struct InputBox : Button {
	sf::Text text;
	sf::Font font;
	sf::Text textDisplay;
	sf::Text nameHolder;
	std::wstring name;
	bool active = 0;
	InputBox(int x, int y, std::string imagePath, std::wstring name);
	void isClicked(sf::RenderWindow& window, sf::Event& event);
	void draw(sf::RenderWindow& window);
};

struct SubmitVNButton : Button {
	bool isClicked(sf::RenderWindow& window, sf::Event& event, std::wstring& word, sf::Text& text);
	SubmitVNButton(int x, int y, std::string imagePath);
};

struct SubmitENButton:Button {
	bool isClicked(sf::RenderWindow& window, sf::Event& event, std::string& word, sf::Text& text);
	SubmitENButton(int x, int y, std::string imagePath);

};


struct InputDef : InputBox {
	InputDef(int x, int y, std::string imagePath, std::wstring name);
	void draw(sf::RenderWindow& window);
	void isClicked(sf::RenderWindow& window, sf::Event& event);
};

struct AnswerButton :Button {
	bool isRightAnswer = 0;
	std::wstring content;
	sf::Text text;
	AnswerButton(int x, int y, std::string imagePath);
	void draw(sf::RenderWindow& window);
};

int run();
