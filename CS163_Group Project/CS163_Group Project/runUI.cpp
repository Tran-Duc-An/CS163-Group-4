#include <SFML/Graphics.hpp>
#include "UI.h"
#include <iostream>
#include "Function.h"
#include <chrono>
#include <io.h>
#include <fcntl.h>
sf::RenderWindow window(sf::VideoMode(1500,800), "Dictionary");
sf::Event event;
sf::View view = window.getView();
float scrollSpeed = 50.f;

Button searchButton(715, 155, "Image/searchButton.png");
Button translatingButton(715, 305, "Image/translateButton.png");
Button addNewWordButton(715, 455, "Image/addButton.png");


Button VNtoEnButton(1034, 33, "Image/VNtoENButton.png");
Button ENtoVnButton(1034, 33, "Image/ENtoVNButton.png");
Button ENtoENButton(1034, 33, "Image/ENtoENButton.png");


Button backButton(12, 18, "Image/backButton.png");
Button nextDefButton(1275, 710, "Image/nextButton.png");
Button backDefButton(820, 710, "Image/backDefButton.png");
Button heartButton(183, 600, "Image/heart.png");
Button deleteButton(349, 600, "Image/deleteButton.png");

Button searchKeyButton(98, 138, "Image/searchKey.png");
Button searchDefButton(98, 237, "Image/searchDef.png");

InputBox inputVNBox(160, 300, "Image/InputBox.png", L"Nhập tại đây");
InputBox inputENBox(160, 300, "Image/InputBox.png", L"Text here");

InputBox searchKeyBox(28, 412, "Image/InputBox.png", L"Text here");
InputDef searchDefBox(75, 400, "Image/InputDef.png", L"Text here");

SubmitENButton translateEN(576, 600, "Image/translateSubmit.png");
SubmitVNButton translateVN(576, 600, "Image/translateSubmit.png");

SubmitENButton submitSearchKey(383, 729, "Image/searchSubmit.png");
SubmitENButton submitSearchDef(1270, 716, "Image/searchSubmit.png");

InputBox inputWord(192, 117, "Image/InputBox.png", L"Enter word");
InputDef inputDef(192, 400, "Image/InputDef.png", L"Enter definition");

SubmitENButton addENButton(1157, 238, "Image/add.png");
SubmitVNButton addVNButton(1157, 238, "Image/add.png");

sf::Font font;
vector<wstring> transDef;
vector<string> searchDef;
EVTrie* rootEtoV = new EVTrie();
EETrie* rootEtoE = new EETrie();
VTrie* rootVtoE = new VTrie();

int page = 0;
int orderDef = 0;
bool translateFlag = 0;
bool searchFlag = 0;
bool transType = 0;
bool searchingType = 0;
int addingType = 0;
void handleWString(wstring& s, int row) {
	int end = 0;
	int l = 0;
	double cnt = 0;
	while (l < s.length()) {
		cnt++;
		if (cnt >= row) {
			if (s[l] < L'a' || s[l] > L'z') {
				s.insert(l, 1, L'\n');
				cnt = 0;
				l++; // Move past the inserted newline
			}
		}
		l++;
	}
}
void handleString(string& s, int row) {
	int end = 0;
	int l = 0;
	double cnt = 0;
	while (l < s.length()) {
		cnt++;
		if (cnt >= row) {
			if (s[l] < 'a' || s[l] > 'z') {
				s.insert(l, 1, '\n');
				cnt = 0;
				l++; // Move past the inserted newline
			}
		}
		l++;
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
				transDef.clear();
				VTrie* node = nullptr;
				if (!VE::findWordMeaning(rootVtoE, wword, transDef,node)) transDef.push_back(L"No definition");
				else {
					if (node->isLiked == 1) {
						heartButton.texture.loadFromFile("Image/heartHover.png");
						if (heartButton.isClicked(window, event)) {
							node->isLiked = 0;
							heartButton.texture.loadFromFile("Image/heart.png");
						}
					}
					else {
						heartButton.texture.loadFromFile("Image/heart.png");
						if (heartButton.isClicked(window, event)) {
							node->isLiked = 1;
							heartButton.texture.loadFromFile("Image/heartHover.png");
						}
					}
				}
				translateFlag = 1;
				orderDef = 0;
			}
			if (deleteButton.isClicked(window, event)) {
				VE::deleteAWord(rootVtoE, wword);
			}
		}
		else {//English to Vietnamese
			if (ENtoVnButton.isClicked(window, event)) {
				transType = 0;
			}
			inputENBox.isClicked(window, event);
			if (translateEN.isClicked(window, event, word, inputENBox.text)) {
				transDef.clear();
				EVTrie* node = nullptr;
				if (!EV::findWordMeaning(rootEtoV, word, transDef,node)) transDef.push_back(L"No definition");
				else {
					if (node->isLiked == 1) {
						heartButton.texture.loadFromFile("Image/heartHover.png");
						if (heartButton.isClicked(window, event)) {
							node->isLiked = 0;
							heartButton.texture.loadFromFile("Image/heart.png");
						}
					}
					else {
						heartButton.texture.loadFromFile("Image/heart.png");
						if (heartButton.isClicked(window, event)) {
							node->isLiked = 1;
							heartButton.texture.loadFromFile("Image/heartHover.png");
						}
					}

				}
				translateFlag = 1;
				orderDef = 0;
			}
			if (deleteButton.isClicked(window, event)) {
				EV::deleteAWord(rootEtoV, word);
			}
		}
		if (nextDefButton.isClicked(window, event) && orderDef < transDef.size() - 1) orderDef++;
		if (backDefButton.isClicked(window, event) && orderDef > 0) orderDef--;

	}

	//display 
	if (transType == 0) {//Vietnamese to English

		if (translateFlag == 1) {
			displayWDef(870, 300, transDef[orderDef], 30);
		}

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

	if (translateFlag == 1 && transDef[0]!=L"No definition") {

		backDefButton.draw(window);
		nextDefButton.draw(window);
		heartButton.draw(window);
		deleteButton.draw(window);
		backDefButton.isHover(window, "Image/backDefHover.png");
		nextDefButton.isHover(window, "Image/nextDefHover.png");
		deleteButton.isHover(window, "Image/deleteHover.png");
		
	}

	backButton.draw(window);
}

void search() {
	sf::Texture layout2;
	if (searchingType == 0) {
		if (!layout2.loadFromFile("Image/layout2.png")) {

		}
	}
	else {
		if (!layout2.loadFromFile("Image/layout3.png")) {

		}
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

		if (searchingType == 0) {//Search with keyword

			searchKeyBox.isClicked(window, event);

			if (submitSearchKey.isClicked(window, event, words, searchKeyBox.text)) {
				searchDef.clear();
				if(!EE::findWordMeaning(rootEtoE, words, searchDef)) searchDef.push_back("No definition");
				searchFlag = 1;
				orderDef = 0;
			}

			if (nextDefButton.isClicked(window, event) && orderDef < searchDef.size() - 1) orderDef++;
			if (backDefButton.isClicked(window, event) && orderDef > 0) orderDef--;
		}
		else {
			searchDefBox.isClicked(window, event);
			if (submitSearchKey.isClicked(window, event, words, searchDefBox.text)) std::cout << words;
		}
		
	}
	if (searchingType == 0) { //search by keyword
		searchKeyBox.draw(window);

		if (searchFlag == 1) {
			nextDefButton.draw(window);
			backDefButton.draw(window);

			displayDef(650, 100, searchDef[orderDef], 50);
		}
		
		searchKeyButton.texture.loadFromFile("Image/searchKeyHover.png");
		searchDefButton.texture.loadFromFile(searchDefButton.orgImage);
	}
	else { //search by definition
		searchDefButton.texture.loadFromFile("Image/searchDefHover.png");
		searchKeyButton.texture.loadFromFile(searchKeyButton.orgImage);
		searchDefBox.draw(window);
	}

	submitSearchKey.draw(window);
	submitSearchKey.isHover(window, "Image/searchSubmitHover.png");

	if (searchKeyBox.text.getString() == "") searchFlag = 0;

	searchKeyButton.draw(window);
	searchDefButton.draw(window);
	backButton.draw(window);

}

void adding() {
	std::string word;
	std::wstring wword;
	std::string def;
	std::wstring wdef;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();
		if (backButton.isClicked(window, event)) page = 0;
		backButton.isHover(window, "Image/backHover.png");


		inputWord.isClicked(window, event);
		inputDef.isClicked(window, event);


		if (addingType == 0) {
			if (ENtoVnButton.isClicked(window, event)) {
				addingType = 1;
			}
			if (addENButton.isClicked(window, event, word, inputWord.text) &&
				addVNButton.isClicked(window, event, wdef, inputDef.text)) {
				EV::insertWord(rootEtoV, word, wdef);
			}
		}
		else if (addingType == 1) {
			if (VNtoEnButton.isClicked(window, event)) {
				addingType = 2;
			}
			if (addVNButton.isClicked(window, event, wword, inputWord.text) &&
				addVNButton.isClicked(window, event, wdef, inputDef.text)) {
				VE::insertWord(rootVtoE, wword, wdef);
			}
		}
		else {
			if (ENtoENButton.isClicked(window, event))
				addingType = 0;
			if (addENButton.isClicked(window, event, word, inputWord.text) &&
				addENButton.isClicked(window, event, def, inputDef.text)) {
				EE::insertWord(rootEtoE, word, def);
			}
		}

	}
	if (addingType == 0) {//English to Vietnamese
		inputWord.name = L"Enter word";
		inputWord.nameHolder.setString(inputWord.name);
		inputDef.name = L"Nhập định nghĩa";
		inputDef.nameHolder.setString(inputDef.name);
		ENtoVnButton.draw(window);
		ENtoVnButton.isHover(window, "Image/ENtoVNHover.png");

		addENButton.draw(window);
		addVNButton.draw(window);
		addENButton.isHover(window, "Image/addHover.png");
		addVNButton.isHover(window, "Image/addHover.png");
	}
	else if (addingType == 1) {//Vietnamese to English
		inputWord.name = L"Nhập từ";
		inputWord.nameHolder.setString(inputWord.name);
		inputDef.name = L"Enter definition";
		inputDef.nameHolder.setString(inputDef.name);
		VNtoEnButton.draw(window);
		VNtoEnButton.isHover(window, "Image/VNtoENHover.png");

		addVNButton.draw(window);
		addVNButton.isHover(window, "Image/addHover.png");
	}
	else {//English to English
		inputWord.name = L"Enter word";
		inputWord.nameHolder.setString(inputWord.name);
		inputDef.name = L"Enter definition";
		inputDef.nameHolder.setString(inputDef.name);
		ENtoENButton.draw(window);
		ENtoENButton.isHover(window, "Image/ENtoENHover.png");

		addENButton.draw(window);
		addENButton.isHover(window, "Image/addHover.png");
	}
	inputWord.draw(window);
	inputDef.draw(window);
	backButton.draw(window);
}

void homePage() {
	orderDef = 0;
	translateFlag = 0;
	searchFlag = 0;
	searchingType = 0;
	transType = 0;
	addingType = 0;

	searchButton.draw(window);
	translatingButton.draw(window);
	addNewWordButton.draw(window);
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) window.close();

		if (searchButton.isClicked(window, event)) {
			page = 1;
			searchKeyBox.text.setString("");
			searchDefBox.text.setString("");
		}
		if (translatingButton.isClicked(window, event)) {
			page = 2;
			inputENBox.text.setString("");
			inputVNBox.text.setString("");
		}
		if (addNewWordButton.isClicked(window, event)) {
			page = 3;
		}
		addNewWordButton.isHover(window, "Image/addnewwordHover.png");
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
		if (!VE::loadRawData(rootVtoE, "Dataset/VE.csv")) return 0;
		VE::saveTrieToFile(rootVtoE, "Dataset/TrieVNEN.bin");
	}

	return 1;
}

int run() {
	font.loadFromFile("Font/ARIAL.TTF");
	setBackground();
	//if (!loadData()) return 0;
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
		case 3: {
			adding();
			break;
		}
		default:
			break;
		}
		window.display();
		window.clear(sf::Color::White);
	}
	EV::deleteTrie(rootEtoV);
	VE::deleteTrie(rootVtoE);
	EE::deleteTrie(rootEtoE);
	return 0;
}


