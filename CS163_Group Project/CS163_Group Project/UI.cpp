#include "UI.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Function.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <SFML/System/Clipboard.hpp>
#endif

Button::Button(int x, int y, std::string imagePath) {
	if (texture.loadFromFile(imagePath)) {
		texture.setSmooth(1);
		sprite.setTexture(texture);
		sprite.setPosition(x, y);
	}
	orgImage = imagePath;
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

void Button::isHover(sf::RenderWindow& window,std::string path) {
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
		texture.loadFromFile(path);
	}
	else texture.loadFromFile(orgImage);
}

InputBox::InputBox(int x, int y, std::string imagePath, std::wstring name) : Button(x, y, imagePath) {
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




void InputBox::pasteFromClipboard(int numRow,int numChar) {
#ifdef _WIN32
	if (OpenClipboard(nullptr)) {
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
		if (pszText) {
			std::wstring str = text.getString();
			str += pszText;
			handleWString(str, numChar, numRow);
			text.setString(str);
			GlobalUnlock(hData);	
		}
		CloseClipboard();
	}
#else
	name = sf::Clipboard::getString().toWideString();
	textDisplay.setString(name);
#endif
}

void InputBox::reset() {
	cursorPosition = 0;
	text.setString("");
}


void InputBox::isClicked(sf::RenderWindow& window, sf::Event& event) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
			active = true;
		}
		else {
			active = false;
		}
	}

	if (active) {
		std::wstring str = text.getString(); // Directly use sf::Text as wstring
		if (str.length() / 20 < 4) {
			if (str.length() % 20 == 0 && str.length() > 0) {
				str += L'\n';
				text.setString(str);
			}
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode > 31 && event.text.unicode != 127) {
					str.insert(cursorPosition, 1, static_cast<wchar_t>(event.text.unicode));
					cursorPosition = str.length();
					text.setString(str);
				}
			}	
		}

		if (event.type == sf::Event::KeyPressed) {
			if (event.key.control && event.key.code == sf::Keyboard::V) {
				pasteFromClipboard(4, 20);
			}
			else if (event.key.code == sf::Keyboard::BackSpace) {
				if (cursorPosition > 0) {
					if (str[cursorPosition - 1] == L'\n') {
						str.erase(cursorPosition - 1, 1);
					}
					str.erase(cursorPosition - 1, 1);
					cursorPosition--;
					text.setString(str);
				}
			}
			else if (event.key.code == sf::Keyboard::Left) {
				if (cursorPosition > 0) {
					cursorPosition--;
				}
			}
			else if (event.key.code == sf::Keyboard::Right) {
				if (cursorPosition < str.length()) {
					cursorPosition++;
				}
			}
		}

		// Update the cursor position after handling input
		updateCursor();
	}
}

void InputBox::updateCursor() {
	// Calculate the position of the cursor
	std::wstring str = text.getString();
	sf::Vector2f cursorPos = text.getPosition();

	for (size_t i = 0; i < cursorPosition; ++i) {
		if (str[i] == L'\n') {
			cursorPos.y += text.getCharacterSize();
			cursorPos.x = text.getPosition().x;
		}
		else {
			cursorPos.x += text.getFont()->getGlyph(str[i], text.getCharacterSize(), false).advance;
		}
	}

	// Update the cursor shape
	cursor.setPosition(cursorPos);
	cursor.setSize(sf::Vector2f(2.f, text.getCharacterSize()));
	cursor.setFillColor(sf::Color::Black);
}

void InputBox::draw(sf::RenderWindow& window) {
	window.draw(sprite);
	window.draw(text);
	if (active) {
		window.draw(cursor);
	}

	std::wstring str = text.getString();
	if (str.empty() && active == 0) {
		window.draw(nameHolder);
	}
}

InputDef::InputDef(int x, int y, std::string imagePath, std::wstring name,int numRows,int numChars) : InputBox(x, y, imagePath, name) {
	numRow = numRows;
	numChar = numChars;
}



SubmitVNButton::SubmitVNButton(int x, int y, std::string imagePath) :Button(x, y, imagePath) {
}

bool SubmitVNButton::isClicked(sf::RenderWindow& window, sf::Event& event, std::wstring& word, sf::Text& text) {
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos)))
		{
			word = text.getString();
			return 1;
		}
	}
	else {
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Enter)) {
			word = text.getString();
			return 1;
		}
	}
	return 0;
}

SubmitENButton::SubmitENButton(int x, int y, std::string imagePath) :Button(x, y, imagePath) {

}

bool SubmitENButton::isClicked(sf::RenderWindow& window, sf::Event& event, std::string& word, sf::Text& text) {
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
		if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
			active = true;
		}
		else {
			active = false;
		}
	}

	if (active) {
		std::wstring str = text.getString(); // Directly use sf::Text as wstring
		if (str.length() / numChar < numRow) {
			if (str.length() % numChar == 0 && str.length() > 0) {
				str += L'\n';
				text.setString(str);
			}
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode > 31 && event.text.unicode != 127) {
					str += static_cast<wchar_t>(event.text.unicode);
					cursorPosition = str.length(); // Move cursor to the end of the string
					text.setString(str);
				}
			}
		}

		if (event.type == sf::Event::KeyPressed) {
			if (event.key.control && event.key.code == sf::Keyboard::V) {
				pasteFromClipboard(numRow, numChar);
			}
			else if (event.key.code == sf::Keyboard::BackSpace) {
				if (!str.empty()) {
					if (str.back() == L'\n') str.pop_back();
					str.pop_back();
					cursorPosition = str.length(); // Move cursor to the end of the string
					text.setString(str);
				}
			}
			else if (event.key.code == sf::Keyboard::Left) {
				if (cursorPosition > 0) {
					cursorPosition--;
				}
			}
			else if (event.key.code == sf::Keyboard::Right) {
				if (cursorPosition < str.length()) {
					cursorPosition++;
				}
			}
		}

		// Update the cursor position after handling input
		updateCursor();
	}
}

void InputDef::updateCursor() {
	// Calculate the position of the cursor
	std::wstring str = text.getString();
	sf::Vector2f cursorPos = text.getPosition();

	for (size_t i = 0; i < cursorPosition; ++i) {
		if (str[i] == L'\n') {
			cursorPos.y += text.getCharacterSize();
			cursorPos.x = text.getPosition().x;
		}
		else {
			cursorPos.x += text.getFont()->getGlyph(str[i], text.getCharacterSize(), false).advance;
		}
	}

	// Update the cursor shape
	cursor.setPosition(cursorPos);
	cursor.setSize(sf::Vector2f(2.f, text.getCharacterSize()));
	cursor.setFillColor(sf::Color::Black);
}

void InputDef::draw(sf::RenderWindow& window) {
	window.draw(sprite);
	std::string str = text.getString().toAnsiString();
	if (str.empty() && !active) {
		window.draw(nameHolder);
	}
	if (active) {
		window.draw(text);
		window.draw(cursor); // Draw the cursor
	}
	else {
		window.draw(text);
	}
}



AnswerButton::AnswerButton(int x, int y, std::string imagePath) :Button(x, y, imagePath) {
	xx = x;
	yy = y;
}

void AnswerButton::draw(sf::RenderWindow& window) {
	window.draw(sprite);
	sf::Text text;

	text.setString(content);
	text.setCharacterSize(30);
	sf::Font font;
	if (!font.loadFromFile("Font/ARIAL.TTF"));
	text.setFont(font);
	text.setPosition(xx + 10, yy + 10);
	text.setFillColor(sf::Color::Black);
	window.draw(text);
}


int AnswerButton::isClicked(sf::RenderWindow& window, sf::Event event) {
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (sprite.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
			if (isRightAnswer == 1) {
				return 1;
			}
			else {
				return 0;
			}

		}
	}
	return 2;
}

ChoiceButton::ChoiceButton(int x, int y, std::string imagePath):Button(x,y,imagePath) {
	xx = x;
	yy = y;
}

void ChoiceButton::draw(sf::RenderWindow& window) {
	window.draw(sprite);
	sf::Text text;

	text.setString(content);
	text.setCharacterSize(30);
	sf::Font font;
	if (!font.loadFromFile("Font/ARIAL.TTF"));
	text.setFont(font);
	text.setPosition(xx + 10, yy + 10);
	text.setFillColor(sf::Color::Black);
	window.draw(text);
}
