#include <SFML/Graphics.hpp>
#include "UI.h"
#include <iostream>
#include "Function.h"
sf::RenderWindow window(sf::VideoMode(1500, 800), "Dictionary");
sf::Event event;
sf::View view = window.getView();
float scrollSpeed = 50.f;

Button VNtoEnButton(520, 218, "Image/VNtoENButton.png");
Button ENtoVnButton(520, 340, "Image/ENtoVNButton.png");
Button searchButton(520, 452, "Image/searchButton.png");
Button backButton(20, 18, "Image/backButton.png");


bool searchingType = 0;
Button searchKeyButton(98, 138, "Image/searchKey.png");
Button searchDefButton(98, 237, "Image/searchDef.png");

InputBox inputVNBox(218, 236, "Image/InputBox.png", L"Nhập tại đây");
InputBox inputENBox(218, 236, "Image/InputBox.png", L"Text here");

InputBox searchKeyBox(28, 412, "Image/InputBox.png", L"Text here");
InputDef searchDefBox(28, 349, "Image/InputDef.png", L"Text here");

SubmitButton translate(548, 368, "Image/translateSubmit.png");
SubmitButton submitSearchButton(383, 729, "Image/searchSubmit.png");

sf::Font font;

vector<wstring> VNDef;

TrieEng* root = new TrieEng();


int page = 0;

void handleString(wstring& s) {
	int end = 0;
	int l = 0;
	int cnt = 0;
	while (l < s.length()) {
		l++;
		cnt++;
		if (double(cnt / 20) > 1 && s[l] == ' ') {
			s[end] = '\n';
			cnt = 0;
		}
		if (s[l] == ' ') end = l;
	}
}

void displayDef(int x, int y, vector<wstring> meaning) {
	for (int i = 0; i < meaning.size(); i++) {
		sf::Text def;
		def.setCharacterSize(40);
		def.setFont(font);
		def.setFillColor(sf::Color::Black);
		def.setPosition(x, y);
		def.setString(meaning[i]);
		window.draw(def);
	}
}

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
	VNtag.setPosition(189, 63);
	window.draw(VNtag);

	sf::Texture eng;
	if (!eng.loadFromFile("Image/ENtag.png")) {

	}
	eng.setSmooth(1);
	sf::Sprite ENtag;
	ENtag.setTexture(eng);
	ENtag.setPosition(860, 63);
	window.draw(ENtag);

	std::string word;

	while (window.pollEvent(event))
	{
		if (backButton.isClicked(window, event)) page = 0;
		backButton.isHover(window, "Image/backHover.png");

		if (event.type == sf::Event::Closed) window.close();

		inputVNBox.isClicked(window, event);
		if (translate.isClicked(window, event, word, inputVNBox.text)) std::cout << word;
		translate.isHover(window, "Image/translateHover.png");
	}
	backButton.draw(window);
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
	VNtag.setPosition(860, 63);
	window.draw(VNtag);

	sf::Texture eng;
	if (!eng.loadFromFile("Image/ENtag.png")) {

	}
	eng.setSmooth(1);
	sf::Sprite ENtag;
	ENtag.setTexture(eng);
	ENtag.setPosition(189, 63);
	window.draw(ENtag);
	std::string word;

	while (window.pollEvent(event))
	{
		if (backButton.isClicked(window, event)) page = 0;
		backButton.isHover(window, "Image/backHover.png");

		if (event.type == sf::Event::Closed) window.close();
		inputENBox.isClicked(window, event);
		if (translate.isClicked(window, event, word, inputENBox.text)) {
			findWordMeaning(root, word, VNDef);
			for (int i = 0; i < VNDef.size(); i++)
				handleString(VNDef[i]);
		}
		translate.isHover(window, "Image/translateHover.png");
	}
	if (inputENBox.text.getString() != "")
	{
		displayDef(880, 236, VNDef);
	}
	translate.draw(window);
	inputENBox.draw(window);
	backButton.draw(window);
}

void search() {
	sf::Texture layout2;
	if (!layout2.loadFromFile("Image/layout2.png")) {

	}
	layout2.setSmooth(1);
	sf::Sprite sprite;
	sprite.setTexture(layout2);
	sprite.setPosition(0, 0);
	window.draw(sprite);
	std::string words;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event)) page = 0;
		backButton.isHover(window, "Image/backHover.png");

		if (searchKeyButton.isClicked(window, event)) searchingType = 0;
		if (searchDefButton.isClicked(window, event)) searchingType = 1;

		if (searchingType == 0) {
			searchKeyBox.isClicked(window, event);
			if (submitSearchButton.isClicked(window, event, words, searchKeyBox.text)) std::cout << words;
		}
		else {
			searchDefBox.isClicked(window, event);
			if (submitSearchButton.isClicked(window, event, words, searchDefBox.text)) std::cout << words;
		}
		submitSearchButton.isHover(window, "Image/searchSubmitHover.png");
	}
	if (searchingType == 0) {
		searchKeyButton.texture.loadFromFile("Image/searchKeyHover.png");
		searchDefButton.texture.loadFromFile(searchDefButton.orgImage);
		searchKeyBox.draw(window);
	}
	else {
		searchDefButton.texture.loadFromFile("Image/searchDefHover.png");
		searchKeyButton.texture.loadFromFile(searchKeyButton.orgImage);
		searchDefBox.draw(window);
	}
	submitSearchButton.draw(window);
	searchKeyButton.draw(window);
	searchDefButton.draw(window);
	backButton.draw(window);

}

void homePage() {
	VNtoEnButton.draw(window);
	ENtoVnButton.draw(window);
	searchButton.draw(window);
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) window.close();

		if (VNtoEnButton.isClicked(window, event)) {
			page = 1;
			inputVNBox.text.setString("");
		}
		if (ENtoVnButton.isClicked(window, event)) {
			page = 2;
			inputENBox.text.setString("");
			for (int i = 0; i < VNDef.size(); i++) VNDef[i].clear();
		}
		if (searchButton.isClicked(window, event)) {
			page = 3;
			searchDefBox.text.setString("");
			searchKeyBox.text.setString("");
		}
		ENtoVnButton.isHover(window, "Image/ENtoVNHover.png");
		VNtoEnButton.isHover(window, "Image/VNtoENHover.png");
		searchButton.isHover(window, "Image/searchHover.png");
	}
}


int run() {
	createTrieEngFromFile(root, "Dataset/ENVN.txt");
	font.loadFromFile("Font/ARIAL.TTF");
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
		case 3: {
			search();
			break;
		}
		default:
			break;
		}
		window.display();
		window.clear(sf::Color::White);
	}
	return 0;
}


