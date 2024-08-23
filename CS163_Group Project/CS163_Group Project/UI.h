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
	sf::RectangleShape cursor;
	size_t cursorPosition = 0;
	bool active = 0;
	InputBox(int x, int y, std::string imagePath, std::wstring name);
	void isClicked(sf::RenderWindow& window, sf::Event& event);
	void draw(sf::RenderWindow& window);
	void updateCursor();
	void pasteFromClipboard(int numRow, int numChar);
	void reset();
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
	int numRow = 1;
	int numChar = 0;
	InputDef(int x, int y, std::string imagePath, std::wstring name,int numRows,int numChars);
	void updateCursor();
	void draw(sf::RenderWindow& window);
	void isClicked(sf::RenderWindow& window, sf::Event& event);
};

struct ChoiceButton :Button {
	std::wstring content = L"";
	int xx = 0;
	int yy = 0;
	ChoiceButton(int x, int y, std::string imagePath);
	void draw(sf::RenderWindow& window);
}; 
struct AnswerButton :Button {
	bool isRightAnswer = 0;
	std::wstring content;
	int xx = 0;
	int yy = 0;

	AnswerButton(int x, int y, std::string imagePath);
	void draw(sf::RenderWindow& window);
	int isClicked(sf::RenderWindow& window, sf::Event event);
};


