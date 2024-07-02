#include <SFML/Graphics.hpp>
#include "UI.h"
#include <iostream>
#include "Function.h"
#include <chrono>
sf::RenderWindow window(sf::VideoMode(1500,800), "Dictionary");
sf::Event event;
sf::View view = window.getView();
float scrollSpeed = 50.f;

Button searchButton(715, 155, "Image/searchButton.png");
Button translatingButton(715, 305, "Image/translateButton.png");

bool transType = 0;
Button VNtoEnButton(1034, 33, "Image/VNtoENButton.png");
Button ENtoVnButton(1034, 33, "Image/ENtoVNButton.png");

Button backButton(12, 18, "Image/backButton.png");
Button nextDefButton(1275, 710, "Image/nextButton.png");
Button backDefButton(820, 710, "Image/backDefButton.png");

bool searchingType = 0;
Button searchKeyButton(98, 138, "Image/searchKey.png");
Button searchDefButton(98, 237, "Image/searchDef.png");

InputBox inputVNBox(160, 300, "Image/InputBox.png", L"Nhập tại đây");
InputBox inputENBox(160, 300, "Image/InputBox.png", L"Text here");

InputBox searchKeyBox(28, 412, "Image/InputBox.png", L"Text here");
InputDef searchDefBox(28, 349, "Image/InputDef.png", L"Text here");

SubmitENButton translateEN(576, 600, "Image/translateSubmit.png");
SubmitVNButton translateVN(576, 600, "Image/translateSubmit.png");

SubmitENButton submitSearchButton(383, 729, "Image/searchSubmit.png");

sf::Font font;

vector<wstring> transDef;
vector<string> ENDef;
EVTrie* rootEtoV = new EVTrie();
EETrie* rootEtoE = new EETrie();
VTrie* rootVtoE = new VTrie();

int page = 0;
int orderDef = 0;
bool translateFlag = 0;
bool searchFlag = 0;

void handleWString(wstring& s,int row) {
	int end = 0;
	int l = 0;
	int cnt = 0;
	while (l < s.length()) {
		l++;
		cnt++;
		if (double(cnt / row) >= 1) {
			if (s[l] < L'a' || s[l]> L'z') {
				s[end] = '\n';
				cnt = 0;
				end = l;
			}
		}
	}
}

void handleString(string& s, int row) {
	int end = 0;
	int l = 0;
	int cnt = 0;
	while (l < s.length()) {
		l++;
		cnt++;
		if (double(cnt / row) >= 1) {
			if (s[l] < L'a' || s[l]> L'z') {
				s[end] = '\n';
				cnt = 0;
				end = l;
			}
		}
	}
}


void displayWDef(int x, int y, wstring meaning,int row) {
	handleWString(meaning, row);
	if (!meaning.empty()) {
		sf::Text def;
		def.setCharacterSize(30);
		def.setFont(font);
		def.setFillColor(sf::Color::Black);
		def.setPosition(x, y);
		def.setString(meaning);
		window.draw(def);
	}
}

void displayDef(int x, int y, string meaning, int row) {
	handleString(meaning, row);
	if (!meaning.empty()) {
		sf::Text def;
		def.setCharacterSize(30);
		def.setFont(font);
		def.setFillColor(sf::Color::Black);
		def.setPosition(x, y);
		def.setString(meaning);
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


void translating() {

	//setup background
	sf::Texture vnmese;
	sf::Texture eng;
	if (!vnmese.loadFromFile("Image/Vntag.png")) {

	}
	if (!eng.loadFromFile("Image/ENtag.png")) {

	}
	sf::Sprite VNtag;
	vnmese.setSmooth(1);
	VNtag.setTexture(vnmese);
	sf::Sprite ENtag;
	eng.setSmooth(1);
	ENtag.setTexture(eng);
	if (transType == 0) {
		VNtag.setPosition(141, 160);
		window.draw(VNtag);

		ENtag.setPosition(821, 160);
		window.draw(ENtag);
		inputVNBox.draw(window);
		VNtoEnButton.draw(window);
	}
	else {
		VNtag.setPosition(821, 160);
		window.draw(VNtag);

		ENtag.setPosition(141, 160);
		window.draw(ENtag);
		inputENBox.draw(window);
		ENtoVnButton.draw(window);
	}

	//handle 
	std::string word;
	std::wstring wword;
	while (window.pollEvent(event))
	{
		if (backButton.isClicked(window, event)) page = 0;
		backButton.isHover(window, "Image/backHover.png");

		if (event.type == sf::Event::Closed) window.close();

		if (transType == 0) {//Vietnamese to English
			if (VNtoEnButton.isClicked(window, event)) {
				transType = 1;
			}
			inputVNBox.isClicked(window, event);
			if (translateVN.isClicked(window, event, wword, inputVNBox.text)) {
				ENDef.clear();
				if (!VE::findWordMeaning(rootVtoE, wword, transDef)) transDef.push_back(L"No definition");
				translateFlag = 1;
				orderDef = 0;
			}
		}
		else {//English to Vietnamese
			if (ENtoVnButton.isClicked(window, event)) {
				transType = 0;
			}
			inputENBox.isClicked(window, event);
			if (translateEN.isClicked(window, event, word, inputENBox.text)) {
				transDef.clear();
				if (!EV::findWordMeaning(rootEtoV, word, transDef)) transDef.push_back(L"No definition");
				translateFlag = 1;
				orderDef = 0;
			}
			if (nextDefButton.isClicked(window, event) && orderDef < transDef.size() - 1) orderDef++;
			if (backDefButton.isClicked(window, event) && orderDef > 0) orderDef--;
		}

	}

	//display 
	if (transType == 0) {//Vietnamese to English
		VNtoEnButton.isHover(window, "Image/VNtoENHover.png");
		translateVN.isHover(window, "Image/transSubHover.png");
		translateVN.draw(window);
	}
	else {//English to Vietnamese
		if (translateFlag == 1) {
			displayWDef(870, 300, transDef[orderDef], 30);
		}
		ENtoVnButton.isHover(window, "Image/ENtoVNHover.png");
		translateEN.isHover(window, "Image/transSubHover.png");
		translateEN.draw(window);
	}

	if (translateFlag == 1) {
		backDefButton.draw(window);
		nextDefButton.draw(window);
		backDefButton.isHover(window, "Image/backDefHover.png");
		nextDefButton.isHover(window, "Image/nextDefHover.png");
	}

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

			if (submitSearchButton.isClicked(window, event, words, searchKeyBox.text)) {
				EE::findWordMeaning(rootEtoE, words, ENDef);
				for (int i = 0; i < ENDef.size(); i++)
					handleString(ENDef[i],30);
				translateFlag = 1;
				orderDef = 0;
			}

			if (nextDefButton.isClicked(window, event) && orderDef < ENDef.size() - 1) orderDef++;
			if (backDefButton.isClicked(window, event) && orderDef > 0) orderDef--;
		}
		else {
			searchDefBox.isClicked(window, event);
			if (submitSearchButton.isClicked(window, event, words, searchDefBox.text)) std::cout << words;
		}
		submitSearchButton.isHover(window, "Image/searchSubmitHover.png");
	}
	if (searchingType == 0) { //search by keyword
		searchKeyButton.texture.loadFromFile("Image/searchKeyHover.png");
		searchDefButton.texture.loadFromFile(searchDefButton.orgImage);
		searchKeyBox.draw(window);
		if (searchFlag == 1) {
			nextDefButton.draw(window);
			backDefButton.draw(window);
		}
	}
	else { //search by definition
		searchDefButton.texture.loadFromFile("Image/searchDefHover.png");
		searchKeyButton.texture.loadFromFile(searchKeyButton.orgImage);
		searchDefBox.draw(window);
	}
	if (searchFlag==1) 
	{
		displayDef(650, 100, ENDef[orderDef], 30);
	}

	if (searchKeyBox.text.getString() == "") translateFlag = 0;
	submitSearchButton.draw(window);
	searchKeyButton.draw(window);
	searchDefButton.draw(window);
	backButton.draw(window);

}

void homePage() {
	orderDef = 0;
	translateFlag = 0;
	searchingType = 0;
	transType = 0;

	searchButton.draw(window);
	translatingButton.draw(window);
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) window.close();

		if (searchButton.isClicked(window, event)) {
			page = 1;
		}
		if (translatingButton.isClicked(window, event)) {
			page = 2;
			inputENBox.text.setString("");
			inputVNBox.text.setString("");
		}
		searchButton.isHover(window, "Image/searchHover.png");
		translatingButton.isHover(window, "Image/translateHover.png");
	}
}

bool loadData() {
	//Loading screen
	sf::Text loading;
	loading.setFont(font);
	loading.setCharacterSize(50);
	loading.setPosition(700, 400);
	loading.setString("Please wait.....");
	loading.setFillColor(sf::Color::Black);
	window.draw(loading);
	window.display();

	if (!EV::loadTriefromFile(rootEtoV, "Dataset/TrieENVN.bin")) {
		if (!EV::loadRawData(rootEtoV, "Dataset/ENVN.txt")) return 0;
		EV::saveTrietoFile(rootEtoV, "Dataset/TrieENVN.bin");
	}
	if (!EE::loadTrieFromFile(rootEtoE, "Dataset/TrieEN.bin")) {
		if (!EE::loadRawData(rootEtoE, "Dataset/englishDictionary.csv")) return 0;
		EE::saveTrietoFile(rootEtoE, "Dataset/TrieEN.bin");
	}
	if (!VE::loadTrieFromFile(rootVtoE, "Dataset/TrieVNEN.bin")) {
		if (!VE::loadRawData(rootVtoE, "Dataset/Dataset/VE.csv")) return 0;
		VE::saveTrieToFile(rootVtoE, "Dataset/TrieVNEN.bin");
	}

	return 1;
}

int run() {
	font.loadFromFile("Font/ARIAL.TTF");
	setBackground();
	if (!loadData()) return 0;

	while (window.isOpen()) {
		setBackground();
		switch (page)
		{
		case 0: {

			homePage();
			break;
		}
		case 1: {
			search();
			break;
		}
		case 2: {
			translating();
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


