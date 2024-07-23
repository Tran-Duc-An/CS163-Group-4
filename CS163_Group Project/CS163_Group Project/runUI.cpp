#include <SFML/Graphics.hpp>
#include "UI.h"
#include <iostream>
#include "Function.h"
#include <chrono>
#include <io.h>
#include <fcntl.h>
#include <locale>
#include <codecvt>
#include <random>
#include <stack>

 sf::RenderWindow window(sf::VideoMode(1500, 800), "Dictionary");
 sf::Event event;

// home page
Button searchButton(715, 155, "Image/searchButton.png");
Button translatingButton(715, 305, "Image/translateButton.png");
Button addNewWordButton(715, 455, "Image/addButton.png");
Button qnaButton(715, 605, "Image/qnaButton.png");
Button historyButton(30, 650, "Image/historyButton.png");
Button isLikedButton(30, 500, "Image/likedButton.png");
Button resetButton(30, 363, "Image/resetButton.png");
//switch dataset
Button VNtoEnButton(1034, 33, "Image/VNtoENButton.png");
Button ENtoVnButton(1034, 33, "Image/ENtoVNButton.png");
Button ENtoENButton(1034, 33, "Image/ENtoENButton.png");

//function button
Button backButton(12, 18, "Image/backButton.png");
Button nextDefButton(1275, 710, "Image/nextButton.png");
Button backDefButton(820, 710, "Image/backDefButton.png");
Button heartButton(183, 600, "Image/heart.png");
Button deleteButton(349, 600, "Image/deleteButton.png");
Button submitResetButton(1220, 582, "Image/submitResetButton.png");
Button tickEEButton(70, 82, "Image/untickedboxEE.png");
Button tickEVButton(70, 232, "Image/untickedboxEV.png");
Button tickVEButton(70, 382, "Image/untickedboxVE.png");

//searching
Button searchKeyButton(98, 138, "Image/searchKey.png");
Button searchDefButton(98, 237, "Image/searchDef.png");

InputBox searchKeyBox(28, 412, "Image/InputBox.png", L"Text here");
InputDef searchDefBox(75, 400, "Image/InputDef.png", L"Text here", 6, 50);

SubmitENButton submitSearchKey(383, 729, "Image/searchSubmit.png");
SubmitENButton submitSearchDef(1270, 716, "Image/searchSubmit.png");


Button heartKeyButton(45, 715, "Image/heart.png");
Button deleteKeyButton(180, 715, "Image/deleteButton.png");

//QNA
Button guessByKey(95, 28, "Image/guessByKey.png");
Button guessByDef(95, 28, "Image/guessByDef.png");
Button nextQuestion(1342, 170, "Image/nextButton.png");

AnswerButton A(209, 442, "Image/answerKeyBox.png");
AnswerButton B(209, 616, "Image/answerKeyBox.png");
AnswerButton C(849, 442, "Image/answerKeyBox.png");
AnswerButton D(849, 616, "Image/answerKeyBox.png");

AnswerButton ADef(102, 295, "Image/answerDefBox.png");
AnswerButton BDef(102, 543, "Image/answerDefBox.png");
AnswerButton CDef(804, 295, "Image/answerDefBox.png");
AnswerButton DDef(804, 543, "Image/answerDefBox.png");


//Translating
InputBox inputVNBox(160, 300, "Image/InputBox.png", L"Nhập tại đây");
InputBox inputENBox(160, 300, "Image/InputBox.png", L"Text here");

SubmitENButton translateEN(576, 600, "Image/translateSubmit.png");
SubmitVNButton translateVN(576, 600, "Image/translateSubmit.png");

//Adding
InputBox inputWord(192, 117, "Image/InputBox.png", L"Enter word");
InputDef inputDef(192, 400, "Image/InputDef.png", L"Enter definition",6,50);

SubmitENButton addENButton(1157, 238, "Image/add.png");
SubmitVNButton addVNButton(1157, 238, "Image/add.png");

// hint
Button guideUserTickButton(160, 13, "Image/hintToTick.png");
Button warningButton(45, 582, "Image/warning.png");


sf::Font font;
vector<wstring> transDef;
vector<string> searchDef;

EVTrie* rootEtoV = new EVTrie();
EETrie* rootEtoE = new EETrie();
VTrie* rootVtoE = new VTrie();
HashTable rootDtoE;



std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

stack<int> page;
int orderDef = 0;
bool translateFlag = 0;
bool searchFlag = 0;
bool transType = 0;
bool searchingType = 0;
int addingType = 0;
int qnaType = 0;

vector<wstring> favWordsVE;
vector<wstring> favDefsVE;

vector<string> favWordsEV;
vector<wstring> favDefsEV;

vector<string> favWordsEE;
vector<string> favDefsEE;

void setBackground();
void translating();
void searching();
void adding();
void QnA();
void history();
void isLiked();
void homePage();
bool loadData();

int run() {
	setBackground();
	font.loadFromFile("Font/ARIAL.TTF");
	//if (!loadData()) return 0;
	page.push(0);

	while (window.isOpen()) {
		setBackground();
		switch (page.top())
		{
		case 0: {
			homePage();
			break;
		}
		case 1: {
			searching();
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
		case 4: {
			QnA();
			break;
		}
		case 5: {
			history();
			break;
		}
		case 6: {
			isLiked();
			break;
		}
		
		default:
			break;
		}
		window.display();
		window.clear(sf::Color::White);
	}

	EV::saveFavWord(favWordsEV, favDefsEV, "Dataset/favWordsEV.txt");
	VE::saveFavWord(favWordsVE, favDefsVE, "Dataset/favWordsVE.txt");
	EE::saveFavWord(favWordsEE, favDefsEE, "Dataset/favWordsEE.txt");

	EV::deleteTrie(rootEtoV);
	VE::deleteTrie(rootVtoE);
	EE::deleteTrie(rootEtoE);
	return 0;
}


void handleWString(wstring& s, int row, int maxRows) {
	int l = 0;
	double cnt = 0;
	int insertedRows = 0;

	while (l < s.length()) {
		cnt++;
		if (cnt >= row) {
			if (s[l] < L'a' || s[l] > L'z') {
				s.insert(l, 1, L'\n');
				cnt = 0;
				l++; // Move past the inserted newline
				insertedRows++; // Increment the number of inserted new lines
				if (insertedRows >= maxRows) {
					s = s.substr(0, l) + L"...";
					break;
				}
			}
		}
		l++;
	}
}
void handleString(string& s, int row, int maxRows) {
	int l = 0;
	double cnt = 0;
	int insertedRows = 0;

	while (l < s.length()) {
		cnt++;
		if (cnt >= row) {
			if (s[l] < 'a' || s[l] > 'z') {
				s.insert(l, 1, '\n');
				cnt = 0;
				l++; // Move past the inserted newline
				insertedRows++; // Increment the number of inserted new lines
				if (insertedRows >= maxRows) {
					s = s.substr(0, l) + "...";
					break;
				}
			}
		}
		l++;
	}
}

void removeEndline(string& s) {
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == L'\n') s.erase(s.begin() + i);
	}
}
void removeWEndline(wstring& s) {
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == L'\n') s.erase(s.begin() + i);
	}
}

void displayWDef(int x, int y, wstring meaning, int row) {
	handleWString(meaning, row, 5);
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
	handleString(meaning, row, 5);
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

VTrie* nodeV = nullptr;
EVTrie* nodeE = nullptr;
std::string transWord;
std::wstring transWword;


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


	while (window.pollEvent(event))
	{
		if (backButton.isClicked(window, event)) page.pop();
		backButton.isHover(window, "Image/backHover.png");

		if (event.type == sf::Event::Closed) window.close();

		if (transType == 0) {//Vietnamese to English
			if (VNtoEnButton.isClicked(window, event)) {
				transType = 1;
				translateFlag = 0;
			}
			inputVNBox.isClicked(window, event);

			if (translateVN.isClicked(window, event, transWword, inputVNBox.text)) {
				transDef.clear();
				orderDef = 0;
				translateFlag = 1;
				removeWEndline(transWword);
				if (!VE::findWordMeaning(rootVtoE, transWword, transDef, nodeV)) transDef.push_back(L"No definition");
				else {
					addToHistory(transWword, transDef[0], "Dataset/History.txt");
				}

			}
			if (heartButton.isClicked(window, event)) {
				if (nodeV != nullptr) {
					nodeV->isLiked = 1 - nodeV->isLiked;
					if (nodeV->isLiked == 1) {
						favWordsVE.push_back(transWword);
						favDefsVE.push_back(transDef[0]);
					}
					else {
						VE::unLikeAWord(favWordsVE, favDefsVE, transWword, transDef[0]);
					}
				}
			}
			if (deleteButton.isClicked(window, event)) {
				VE::deleteAWord(rootVtoE, transWword);
				inputVNBox.text.setString("");
				transDef.clear();
				translateFlag = 0;
			}
		}
		else {//English to Vietnamese
			if (ENtoVnButton.isClicked(window, event)) {
				transType = 0;
				translateFlag = 0;
			}
			inputENBox.isClicked(window, event);
			if (translateEN.isClicked(window, event, transWord, inputENBox.text)) {
				transDef.clear();
				translateFlag = 1;
				orderDef = 0;
				removeEndline(transWord);
				if (!EV::findWordMeaning(rootEtoV, transWord, transDef, nodeE)) transDef.push_back(L"No definition");
				else {
					addToHistory(converter.from_bytes(transWord), transDef[0], "Dataset/History.txt");
				}
			}
			if (heartButton.isClicked(window, event)) {
				if (nodeE != nullptr) {
					nodeE->isLiked = 1 - nodeE->isLiked;
					if (nodeE->isLiked == 1) {
						favWordsEV.push_back(transWord);
						favDefsEV.push_back(transDef[0]);
					}
					else {
						EV::unLikeAWord(favWordsEV, favDefsEV, transWord, transDef[0]);
					}
				}
			}
			if (deleteButton.isClicked(window, event)) {
				EV::deleteAWord(rootEtoV, transWord);
				inputENBox.text.setString("");
				transDef.clear();
				translateFlag = 0;
			}
		}
		if (nextDefButton.isClicked(window, event) && orderDef < transDef.size() - 1) orderDef++;
		if (backDefButton.isClicked(window, event) && orderDef > 0) orderDef--;

	}



	//display 

	if (transType == 0) {//Vietnamese to English

		if (inputVNBox.text.getString() == L"") {
			translateFlag = 0;
			heartButton.texture.loadFromFile("Image/heart.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}

		if (translateFlag == 1) {
			displayWDef(870, 300, transDef[orderDef], 30);

			if (nodeV != nullptr && nodeV->isLiked == 0) {
				heartButton.texture.loadFromFile("Image/heart.png");
				heartButton.sprite.setTexture(heartButton.texture);
			}
			else if (nodeV != nullptr && nodeV->isLiked == 1) {
				heartButton.texture.loadFromFile("Image/heartHover.png");
				heartButton.sprite.setTexture(heartButton.texture);
			}
		}



		VNtoEnButton.isHover(window, "Image/VNtoENHover.png");
		translateVN.isHover(window, "Image/transSubHover.png");
		translateVN.draw(window);

	}
	else {//English to Vietnamese

		if (inputENBox.text.getString() == L"") {
			translateFlag = 0;
			heartButton.texture.loadFromFile("Image/heart.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
		if (translateFlag == 1) {
			displayWDef(870, 300, transDef[orderDef], 30);
			if (nodeE != nullptr && nodeE->isLiked == 0) {
				heartButton.texture.loadFromFile("Image/heart.png");
				heartButton.sprite.setTexture(heartButton.texture);
			}
			else if (nodeE != nullptr && nodeE->isLiked == 1) {
				heartButton.texture.loadFromFile("Image/heartHover.png");
				heartButton.sprite.setTexture(heartButton.texture);
			}
		}

		ENtoVnButton.isHover(window, "Image/ENtoVNHover.png");
		translateEN.isHover(window, "Image/transSubHover.png");
		translateEN.draw(window);
	}

	if (translateFlag == 1 && transDef[0] != L"No definition") {

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

EETrie* nodeEE = nullptr;
std::string def = "";
std::string word = "";

void searching() {
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

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event)) page.pop();
		backButton.isHover(window, "Image/backHover.png");

		if (searchKeyButton.isClicked(window, event)) {
			searchingType = 0;
		}
		if (searchDefButton.isClicked(window, event)) {
			searchingType = 1;
		}

		if (searchingType == 0) {//Search with keyword

			searchKeyBox.isClicked(window, event);

			if (submitSearchKey.isClicked(window, event, word, searchKeyBox.text)) {
				searchDef.clear();
				orderDef = 0;
				removeEndline(word);
				if (!EE::findWordMeaning(rootEtoE, word, searchDef, nodeEE)) searchDef.push_back("No definition");
				else {
					addToHistory(converter.from_bytes(word), converter.from_bytes(searchDef[0]), "Dataset/History.txt");
				}
				searchFlag = 1;

			}

			if (heartKeyButton.isClicked(window, event)) {
				if (nodeEE != nullptr) {
					nodeEE->isLiked = 1 - nodeEE->isLiked;
					if (nodeEE->isLiked == 1) {
						favWordsEE.push_back(word);
						favDefsEE.push_back(searchDef[0]);
					}
					else {
						EE::unLikeAWord(favWordsEE, favDefsEE, word, searchDef[0]);
					}
				}
			}

			if (deleteKeyButton.isClicked(window, event)) {
				EE::deleteAWord(rootEtoE, word);
				searchKeyBox.text.setString("");
				searchDef.clear();
				searchFlag = 0;
			}

			if (nextDefButton.isClicked(window, event) && orderDef < searchDef.size() - 1) orderDef++;
			if (backDefButton.isClicked(window, event) && orderDef > 0) orderDef--;
		}
		else {//search with definition
			searchDefBox.isClicked(window, event);
			if (submitSearchDef.isClicked(window, event, def, searchDefBox.text)) {
				removeEndline(def);
				word = Def::findWordMeaning(rootDtoE, def);
				if (word != "") {
					addToHistory(converter.from_bytes(word), converter.from_bytes(def), "Dataset/History.csv");
				}
				searchFlag = 1;
			}

		}

	}


	if (searchingType == 0) { //search by keyword
		searchKeyBox.draw(window);
		if (searchKeyBox.text.getString() == "") {
			heartButton.texture.loadFromFile("Image/heart.png");
			heartButton.sprite.setTexture(heartButton.texture);
			searchFlag = 0;
		}
		if (searchFlag == 1) {
			nextDefButton.draw(window);
			backDefButton.draw(window);

			displayDef(650, 100, searchDef[orderDef], 50);
			if (nodeEE != nullptr && nodeEE->isLiked == 0) {
				heartKeyButton.texture.loadFromFile("Image/heart.png");
				heartKeyButton.sprite.setTexture(heartKeyButton.texture);
			}
			else if (nodeEE != nullptr && nodeEE->isLiked == 1) {
				heartKeyButton.texture.loadFromFile("Image/heartHover.png");
				heartKeyButton.sprite.setTexture(heartKeyButton.texture);
			}

			heartKeyButton.draw(window);
			deleteKeyButton.draw(window);
			deleteKeyButton.isHover(window, "Image/deleteHover.png");
		}
		
		searchKeyButton.texture.loadFromFile("Image/searchKeyHover.png");
		searchDefButton.texture.loadFromFile(searchDefButton.orgImage);
		submitSearchKey.draw(window);
		submitSearchKey.isHover(window, "Image/searchSubmitHover.png");
	}
	else { //search by definition
		searchDefBox.draw(window);
		if (searchFlag == 1) {
			sf::Text voca;
			voca.setCharacterSize(40);
			voca.setFont(font);
			voca.setFillColor(sf::Color::Black);
			voca.setPosition(775, 45);
			voca.setString(word);
			window.draw(voca);
		}

		if (searchDefBox.text.getString() == "") searchFlag = 0;

		searchDefButton.texture.loadFromFile("Image/searchDefHover.png");
		searchKeyButton.texture.loadFromFile(searchKeyButton.orgImage);
		submitSearchDef.draw(window);
		submitSearchDef.isHover(window, "Image/searchSubmitHover.png");
	}



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
		if (backButton.isClicked(window, event)) page.pop();
		backButton.isHover(window, "Image/backHover.png");


		inputWord.isClicked(window, event);
		inputDef.isClicked(window, event);


		if (addingType == 0) {
			if (ENtoVnButton.isClicked(window, event)) {
				addingType = 1;
			}
			if (addENButton.isClicked(window, event, word, inputWord.text) &&
				addVNButton.isClicked(window, event, wdef, inputDef.text)) {
				removeEndline(word);
				removeWEndline(wdef);
				EV::insertWord(rootEtoV, word, wdef);
			}
		}
		else if (addingType == 1) {
			if (VNtoEnButton.isClicked(window, event)) {
				addingType = 2;
			}
			if (addVNButton.isClicked(window, event, wword, inputWord.text) &&
				addVNButton.isClicked(window, event, wdef, inputDef.text)) {
				removeWEndline(wword);
				removeWEndline(wdef);
				VE::insertWord(rootVtoE, wword, wdef);
			}
		}
		else {
			if (ENtoENButton.isClicked(window, event))
				addingType = 0;
			if (addENButton.isClicked(window, event, word, inputWord.text) &&
				addENButton.isClicked(window, event, def, inputDef.text)) {
				removeEndline(word);
				removeEndline(def);
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

int posx = 0;
int posy = 0;
int answerFlag = 2;
bool guessType = 0;


std::string rightWord;
std::string wrongWord1;
std::string wrongWord2;
std::string wrongWord3;

std::string rightDef;
std::string wrongDef1;
std::string wrongDef2;
std::string wrongDef3;



std::wstring wrightWord;
std::wstring wrightDef;

std::wstring wwrongDef1;
std::wstring wwrongDef2;
std::wstring wwrongDef3;
vector<std::wstring> ansWord;
vector<std::wstring> ansDef;

void handleKeyAnswer() {
	if (A.isClicked(window, event) == 0) {
		answerFlag = 0;
		posx = A.xx;
		posy = A.yy;
	}
	else if (A.isClicked(window, event) == 1) {
		answerFlag = 1;
		posx = A.xx;
		posy = A.yy;
	}
	else if (B.isClicked(window, event) == 0) {
		answerFlag = 0;
		posx = B.xx;
		posy = B.yy;
	}
	else if (B.isClicked(window, event) == 1) {
		answerFlag = 1;
		posx = B.xx;
		posy = B.yy;
	}
	else if (C.isClicked(window, event) == 0) {
		answerFlag = 0;
		posx = C.xx;
		posy = C.yy;
	}
	else if (C.isClicked(window, event) == 1) {
		answerFlag = 1;
		posx = C.xx;
		posy = C.yy;
	}
	else if (D.isClicked(window, event) == 0) {
		answerFlag = 0;
		posx = D.xx;
		posy = D.yy;
	}
	else if (D.isClicked(window, event) == 1) {
		answerFlag = 1;
		posx = D.xx;
		posy = D.yy;
	}
}

void handleDefAnswer() {
	if (ADef.isClicked(window, event) == 0) {
		answerFlag = 0;
		posx = ADef.xx;
		posy = ADef.yy;
	}
	else if (ADef.isClicked(window, event) == 1) {
		answerFlag = 1;
		posx = ADef.xx;
		posy = ADef.yy;
	}
	else if (BDef.isClicked(window, event) == 0) {
		answerFlag = 0;
		posx = BDef.xx;
		posy = BDef.yy;
	}
	else if (BDef.isClicked(window, event) == 1) {
		answerFlag = 1;
		posx = BDef.xx;
		posy = BDef.yy;
	}
	else if (CDef.isClicked(window, event) == 0) {
		answerFlag = 0;
		posx = CDef.xx;
		posy = CDef.yy;
	}
	else if (CDef.isClicked(window, event) == 1) {
		answerFlag = 1;
		posx = CDef.xx;
		posy = CDef.yy;
	}
	else if (DDef.isClicked(window, event) == 0) {
		answerFlag = 0;
		posx = DDef.xx;
		posy = DDef.yy;
	}
	else if (DDef.isClicked(window, event) == 1) {
		answerFlag = 1;
		posx = DDef.xx;
		posy = DDef.yy;
	}
}
bool rangeRandom[4] = { 1,1,1,1 };

int getRandom(int min, int max) {
	// Create a random device and use it to seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	// Generate and return the random number
	return dis(gen);
}

void randomKeyAnswer(vector<std::wstring> ansWord, int k) {
	if (ansWord.empty()) return;
	if (k < 0) return;
	int rand = getRandom(0, 3);
	while (rangeRandom[rand] != 1)
		rand = getRandom(0, 3);
	rangeRandom[rand] = 0;

	if (rand == 0) {
		A.content = ansWord[k];
		if (k == 0)
			A.isRightAnswer = 1;
		else A.isRightAnswer = 0;
	}
	else if (rand == 1) {
		B.content = ansWord[k];
		if (k == 0)
			B.isRightAnswer = 1;
		else B.isRightAnswer = 0;
	}
	else if (rand == 2) {
		C.content = ansWord[k];
		if (k == 0)
			C.isRightAnswer = 1;
		else C.isRightAnswer = 0;
	}
	else if (rand == 3) {
		D.content = ansWord[k];
		if (k == 0)
			D.isRightAnswer = 1;
		else D.isRightAnswer = 0;
	}
	randomKeyAnswer(ansWord, --k);
}

void randomDefAnswer(vector<std::wstring> ansDef, int k) {
	if (ansDef.empty()) return;
	if (k < 0) return;
	int rand = getRandom(0, 3);
	while (rangeRandom[rand] != 1)
		rand = getRandom(0, 3);
	rangeRandom[rand] = 0;

	if (rand == 0) {
		ADef.content = ansDef[k];
		if (k == 0)
			ADef.isRightAnswer = 1;
		else ADef.isRightAnswer = 0;
	}
	else if (rand == 1) {
		BDef.content = ansDef[k];
		if (k == 0)
			BDef.isRightAnswer = 1;
		else BDef.isRightAnswer = 0;
	}
	else if (rand == 2) {
		CDef.content = ansDef[k];
		if (k == 0)
			CDef.isRightAnswer = 1;
		else CDef.isRightAnswer = 0;
	}
	else if (rand == 3) {
		DDef.content = ansDef[k];
		if (k == 0)
			DDef.isRightAnswer = 1;
		else DDef.isRightAnswer = 0;
	}
	randomDefAnswer(ansDef, --k);
}

void rightorwrongKey() {
	if (answerFlag == 0) {
		sf::Texture wrong;
		if (wrong.loadFromFile("Image/wrong.png")) {
			sf::Sprite sprite;
			sprite.setTexture(wrong);
			sprite.setPosition(posx + 350, posy + 65);
			window.draw(sprite);
		}
	}
	else if (answerFlag == 1) {
		sf::Texture correct;
		if (correct.loadFromFile("Image/tick.png")) {
			sf::Sprite sprite;
			sprite.setTexture(correct);
			sprite.setPosition(posx + 350, posy + 65);
			window.draw(sprite);
		}
	}
}

void rightorwrongDef() {
	if (answerFlag == 0) {
		sf::Texture wrong;
		if (wrong.loadFromFile("Image/wrong.png")) {
			sf::Sprite sprite;
			sprite.setTexture(wrong);
			sprite.setPosition(posx + 500, posy + 120);
			window.draw(sprite);
		}
	}
	else if (answerFlag == 1) {
		sf::Texture correct;
		if (correct.loadFromFile("Image/tick.png")) {
			sf::Sprite sprite;
			sprite.setTexture(correct);
			sprite.setPosition(posx + 500, posy + 120);
			window.draw(sprite);
		}
	}
}

void QnA() {
	sf::Texture questionLayout;
	sf::Sprite qSprite;
	qSprite.setPosition(95, 105);
	questionLayout.setSmooth(1);
	if (guessType == 0) {
		if (!questionLayout.loadFromFile("Image/askDefBox.png")) return;
	}
	else {
		if (!questionLayout.loadFromFile("Image/askKeyBox.png")) return;
	}
	qSprite.setTexture(questionLayout);
	window.draw(qSprite);
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();
		if (backButton.isClicked(window, event)) page.pop();
		backButton.isHover(window, "Image/backHover.png");

		//change type of question
		if (qnaType == 0) {
			if (VNtoEnButton.isClicked(window, event)) {
				qnaType = 1;
			}
		}
		else if (qnaType == 1) {

			if (ENtoVnButton.isClicked(window, event)) {
				qnaType = 2;
				answerFlag = 2;
			}
			if (nextQuestion.isClicked(window, event)) {

				answerFlag = 2;

				if (guessType == 0) {//guess by definition
					ansWord.clear();

					wrightDef = L"";
					rightWord = "";
					wrongWord1 = "";
					wrongWord2 = "";
					wrongWord3 = "";

					EV::randomADefinitionAnd4Words(rootEtoV, wrightDef, rightWord, wrongWord1, wrongWord2, wrongWord3);
					handleWString(wrightDef, 80, 6);
					ansWord.push_back(converter.from_bytes(rightWord));
					ansWord.push_back(converter.from_bytes(wrongWord1));
					ansWord.push_back(converter.from_bytes(wrongWord2));
					ansWord.push_back(converter.from_bytes(wrongWord3));

					randomKeyAnswer(ansWord, 3);
					memset(rangeRandom, 1, 4);
				}
				else {
					ansDef.clear();
					rightWord = "";
					wrightDef = L"";
					wwrongDef1 = L"";
					wwrongDef2 = L"";
					wwrongDef3 = L"";
					EV::randomAWordAnd4Definitions(rootEtoV, rightWord, wrightDef, wwrongDef1, wwrongDef2, wwrongDef3);

					wrightWord = converter.from_bytes(rightWord);
					ansDef.push_back(wrightDef);
					handleWString(ansDef[0], 40, 4);
					ansDef.push_back(wwrongDef1);
					handleWString(ansDef[1], 40, 4);
					ansDef.push_back(wwrongDef2);
					handleWString(ansDef[2], 40, 4);
					ansDef.push_back(wwrongDef3);
					handleWString(ansDef[3], 40, 4);

					randomDefAnswer(ansDef, 3);
					memset(rangeRandom, 1, 4);
				}
			}
		}
		else {

			if (ENtoENButton.isClicked(window, event)) {
				qnaType = 0;
				answerFlag = 2;
			}
			if (nextQuestion.isClicked(window, event)) {

				answerFlag = 2;

				if (guessType == 0) {//guess by definition
					ansWord.clear();

					rightDef = "";
					rightWord = "";
					wrongWord1 = "";
					wrongWord2 = "";
					wrongWord3 = "";

					EE::randomADefinitionAnd4Words(rootEtoE, rightDef, rightWord, wrongWord1, wrongWord2, wrongWord3);


					wrightDef = converter.from_bytes(rightDef);
					handleWString(wrightDef, 80, 6);
					ansWord.push_back(converter.from_bytes(rightWord));
					ansWord.push_back(converter.from_bytes(wrongWord1));
					ansWord.push_back(converter.from_bytes(wrongWord2));
					ansWord.push_back(converter.from_bytes(wrongWord3));

					randomKeyAnswer(ansWord, 3);
					memset(rangeRandom, 1, 4);
				}
				else {
					ansDef.clear();
					rightWord = "";
					rightDef = "";
					wrongDef1 = "";
					wrongDef2 = "";
					wrongDef3 = "";
					EE::randomAWordAnd4Definitions(rootEtoE, rightWord, rightDef, wrongDef1, wrongDef2, wrongDef3);

					wrightWord = converter.from_bytes(rightWord);
					ansDef.push_back(converter.from_bytes(rightDef));
					handleWString(ansDef[0], 40, 4);
					ansDef.push_back(converter.from_bytes(wrongDef1));
					handleWString(ansDef[1], 40, 4);
					ansDef.push_back(converter.from_bytes(wrongDef2));
					handleWString(ansDef[2], 40, 4);
					ansDef.push_back(converter.from_bytes(wrongDef3));
					handleWString(ansDef[3], 40, 4);

					randomDefAnswer(ansDef, 3);
					memset(rangeRandom, 1, 4);
				}
			}
		}

		//hanlde guess by keyword or definition
		if (guessType == 0) {
			if (guessByDef.isClicked(window, event)) guessType = 1;
			handleKeyAnswer();
			//answerFlag = 2;
		}
		else {
			if (guessByKey.isClicked(window, event)) guessType = 0;
			handleDefAnswer();
			//answerFlag = 2;
		}


	}

	if (qnaType == 0) {
		VNtoEnButton.draw(window);
		VNtoEnButton.isHover(window, "Image/VNtoENHover.png");
	}
	else if (qnaType == 1) {
		ENtoVnButton.draw(window);
		ENtoVnButton.isHover(window, "Image/ENtoVNHover.png");
	}
	else if (qnaType == 2) {//Eng to Eng
		ENtoENButton.draw(window);
		ENtoENButton.isHover(window, "Image/ENtoENHover.png");
	}

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::Black);
	text.setPosition(100, 110);

	if (guessType == 0) {//Guess by definition
		guessByDef.draw(window);
		guessByDef.isHover(window, "Image/guessByDefHover.png");
		if (ansWord.empty())
			text.setString("ARE YOU READY!!!!");
		else text.setString(wrightDef);
		window.draw(text);

		A.draw(window);
		B.draw(window);
		C.draw(window);
		D.draw(window);

		rightorwrongKey();

	}
	else {//guess by keyword
		guessByKey.draw(window);
		guessByKey.isHover(window, "Image/guessByKeyHover.png");
		if (ansDef.empty())
			text.setString("ARE YOU READY!!!!");
		else text.setString(rightWord);
		window.draw(text);


		ADef.draw(window);
		BDef.draw(window);
		CDef.draw(window);
		DDef.draw(window);

		rightorwrongDef();
	}

	nextQuestion.draw(window);
	nextQuestion.isHover(window, "Image/nextDefHover.png");
}

AnswerButton h1(180, 110, "Image/displayBox.png");
AnswerButton h2(180, 280, "Image/displayBox.png");
AnswerButton h3(180, 450, "Image/displayBox.png");
AnswerButton h4(180, 620, "Image/displayBox.png");
AnswerButton h5(780, 110, "Image/displayBox.png");
AnswerButton h6(780, 280, "Image/displayBox.png");
AnswerButton h7(780, 450, "Image/displayBox.png");
AnswerButton h8(780, 620, "Image/displayBox.png");

Button nextHisButton(1325, 725, "Image/nextButton.png");
Button backHisButton(10, 725, "Image/backDefButton.png");

void history() {

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event)) page.pop();
		backButton.isHover(window, "Image/backHover.png");
	}

	h1.draw(window);
	h2.draw(window);
	h3.draw(window);
	h4.draw(window);
	h5.draw(window);
	h6.draw(window);
	h7.draw(window);
	h8.draw(window);

	backButton.draw(window);
}

int favType = 0;
int orderFav = 0;
bool firstTime = 0;
void setContentEV() {
	if (orderFav < favWordsEV.size())
		h1.content = converter.from_bytes(favWordsEV[orderFav]) + L" :" + favDefsEV[orderFav];
	else h1.content = L"";
	if (orderFav + 1 < favWordsEV.size())
		h2.content = converter.from_bytes(favWordsEV[orderFav + 1]) + L" :" + favDefsEV[orderFav + 1];
	else h2.content = L"";
	if (orderFav + 2 < favWordsEV.size())
		h3.content = converter.from_bytes(favWordsEV[orderFav + 2]) + L" :" + favDefsEV[orderFav + 2];
	else h3.content = L"";
	if (orderFav + 3 < favWordsEV.size())
		h4.content = converter.from_bytes(favWordsEV[orderFav + 3]) + L" :" + favDefsEV[orderFav + 3];
	else h4.content = L"";
	if (orderFav + 4 < favWordsEV.size())
		h5.content = converter.from_bytes(favWordsEV[orderFav + 4]) + L" :" + favDefsEV[orderFav + 4];
	else h5.content = L"";
	if (orderFav + 5 < favWordsEV.size())
		h6.content = converter.from_bytes(favWordsEV[orderFav + 5]) + L" :" + favDefsEV[orderFav + 5];
	else h6.content = L"";
	if (orderFav + 6 < favWordsEV.size())
		h7.content = converter.from_bytes(favWordsEV[orderFav + 6]) + L" :" + favDefsEV[orderFav + 6];
	else h7.content = L"";
	if (orderFav + 7 < favWordsEV.size())
		h8.content = converter.from_bytes(favWordsEV[orderFav + 7]) + L" :" + favDefsEV[orderFav + 7];
	else h8.content = L"";
	handleWString(h1.content, 35, 2);
	handleWString(h2.content, 35, 2);
	handleWString(h3.content, 35, 2);
	handleWString(h4.content, 35, 2);
	handleWString(h5.content, 35, 2);
	handleWString(h6.content, 35, 2);
	handleWString(h7.content, 35, 2);
	handleWString(h8.content, 35, 2);

}

void setContentVE() {
	if (orderFav < favWordsVE.size())
		h1.content = favWordsVE[orderFav] + L" :" + favDefsVE[orderFav];
	else h1.content = L"";
	if (orderFav + 1 < favWordsVE.size())
		h2.content = favWordsVE[orderFav + 1] + L" :" + favDefsVE[orderFav + 1];
	else h2.content = L"";
	if (orderFav + 2 < favWordsVE.size())
		h3.content = favWordsVE[orderFav + 2] + L" :" + favDefsVE[orderFav + 2];
	else h3.content = L"";
	if (orderFav + 3 < favWordsVE.size())
		h4.content = favWordsVE[orderFav + 3] + L" :" + favDefsVE[orderFav + 3];
	else h4.content = L"";
	if (orderFav + 4 < favWordsVE.size())
		h5.content = favWordsVE[orderFav + 4] + L" :" + favDefsVE[orderFav + 4];
	else h5.content = L"";
	if (orderFav + 5 < favWordsVE.size())
		h6.content = favWordsVE[orderFav + 5] + L" :" + favDefsVE[orderFav + 5];
	else h6.content = L"";
	if (orderFav + 6 < favWordsVE.size())
		h7.content = favWordsVE[orderFav + 6] + L" :" + favDefsVE[orderFav + 6];
	else h7.content = L"";
	if (orderFav + 7 < favWordsVE.size())
		h8.content = favWordsVE[orderFav + 7] + L" :" + favDefsVE[orderFav + 7];
	else h8.content = L"";

	handleWString(h1.content, 35, 2);
	handleWString(h2.content, 35, 2);
	handleWString(h3.content, 35, 2);
	handleWString(h4.content, 35, 2);
	handleWString(h5.content, 35, 2);
	handleWString(h6.content, 35, 2);
	handleWString(h7.content, 35, 2);
	handleWString(h8.content, 35, 2);
}

void setContentEE() {
	if (orderFav < favWordsEE.size())
		h1.content = converter.from_bytes(favWordsEE[orderFav]) + L" :" + converter.from_bytes(favDefsEE[orderFav]);
	else h1.content = L"";
	if (orderFav+1 < favWordsEE.size())
		h2.content = converter.from_bytes(favWordsEE[orderFav+1]) + L" :" + converter.from_bytes(favDefsEE[orderFav+1]);
	else h2.content = L"";
	if (orderFav+2 < favWordsEE.size())
		h3.content = converter.from_bytes(favWordsEE[orderFav+2]) + L" :" + converter.from_bytes(favDefsEE[orderFav+2]);
	else h3.content = L"";
	if (orderFav+3 < favWordsEE.size())
		h4.content = converter.from_bytes(favWordsEE[orderFav+3]) + L" :" + converter.from_bytes(favDefsEE[orderFav+3]);
	else h4.content = L"";
	if (orderFav+4 < favWordsEE.size())
		h5.content = converter.from_bytes(favWordsEE[orderFav+4]) + L" :" + converter.from_bytes(favDefsEE[orderFav+4]);
	else h5.content = L"";
	if (orderFav+5 < favWordsEE.size())
		h6.content = converter.from_bytes(favWordsEE[orderFav+5]) + L" :" + converter.from_bytes(favDefsEE[orderFav+5]);
	else h6.content = L"";
	if (orderFav+6 < favWordsEE.size())
		h7.content = converter.from_bytes(favWordsEE[orderFav+6]) + L" :" + converter.from_bytes(favDefsEE[orderFav+6]);
	else h7.content = L"";
	if (orderFav+7 < favWordsEE.size())
		h8.content = converter.from_bytes(favWordsEE[orderFav+7]) + L" :" + converter.from_bytes(favDefsEE[orderFav+7]);
	else h8.content = L"";

	handleWString(h1.content, 35, 2);
	handleWString(h2.content, 35, 2);
	handleWString(h3.content, 35, 2);
	handleWString(h4.content, 35, 2);
	handleWString(h5.content, 35, 2);
	handleWString(h6.content, 35, 2);
	handleWString(h7.content, 35, 2);
	handleWString(h8.content, 35, 2);
}

void handleChangePageEV() {
	if (h1.isClicked(window, event) == 0) {
		page.push(2);
		transType = 1;
		if (orderFav < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav]);
	}
	if (h2.isClicked(window, event) == 0) {
		page.push(2);
		transType = 1;
		if (orderFav+1 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+1]);
	}
	if (h3.isClicked(window, event) == 0) {
		page.push(2);
		transType = 1;
		if (orderFav+2 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+2]);
	}
	if (h4.isClicked(window, event) == 0) {
		page.push(2);
		transType = 1;
		if (orderFav+3 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+3]);
	}
	if (h5.isClicked(window, event) == 0) {
		page.push(2);
		transType = 1;
		if (orderFav+4 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+4]);
	}
	if (h6.isClicked(window, event) == 0) {
		page.push(2);
		transType = 1;
		if (orderFav+5 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+5]);
	}
	if (h7.isClicked(window, event) == 0) {
		page.push(2);
		transType = 1;
		if (orderFav+6< favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+6]);
	}
	if (h8.isClicked(window, event) == 0) {
		page.push(2);
		transType = 1;
		if (orderFav+7 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+7]);
	}
}

void handleChangePageVE() {
	if (h1.isClicked(window, event) == 0) {
		page.push(2);
		transType = 0;
		if (orderFav < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav]);
	}
	if (h2.isClicked(window, event) == 0) {
		page.push(2);
		transType = 0;
		if (orderFav+1 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+1]);
	}
	if (h3.isClicked(window, event) == 0) {
		page.push(2);
		transType = 0;
		if (orderFav+2 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+2]);
	}
	if (h4.isClicked(window, event) == 0) {
		page.push(2);
		transType = 0;
		if (orderFav+3 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+3]);
	}
	if (h5.isClicked(window, event) == 0) {
		page.push(2);
		transType = 0;
		if (orderFav+4 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+4]);
	}	
	if (h6.isClicked(window, event) == 0) {
		page.push(2);
		transType = 0;
		if (orderFav+5 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+5]);
	}
	if (h7.isClicked(window, event) == 0) {
		page.push(2);
		transType = 0;
		if (orderFav+6 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+6]);
	}
	if (h8.isClicked(window, event) == 0) {
		page.push(2);
		transType = 0;
		if (orderFav+7 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+7]);
	}

}

void handleChangePageEE() {
	if (h1.isClicked(window, event) == 0) {
		page.push(1);
		searchingType = 0;
		if (orderFav < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav]);
	}
	if (h2.isClicked(window, event) == 0) {
		page.push(1);
		searchingType = 0;
		if (orderFav+1 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+1]);
	}
	if (h3.isClicked(window, event) == 0) {
		page.push(1);
		searchingType = 0;
		if (orderFav+2 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+2]);
	}
	if (h4.isClicked(window, event) == 0) {
		page.push(1);
		searchingType = 0;
		if (orderFav+3 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+3]);
	}
	if (h5.isClicked(window, event) == 0) {
		page.push(1);
		searchingType = 0;
		if (orderFav+4 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+4]);
	}
	if (h6.isClicked(window, event) == 0) {
		page.push(1);
		searchingType = 0;
		if (orderFav+5 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+5]);
	}
	if (h7.isClicked(window, event) == 0) {
		page.push(1);
		searchingType = 0;
		if (orderFav+6 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+6]);
	}
	if (h8.isClicked(window, event) == 0) {
		page.push(1);
		searchingType = 0;
		if (orderFav+7 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+7]);
	}

}

void isLiked() {
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event)) page.pop();
		backButton.isHover(window, "Image/backHover.png");

		if (favType == 0) {
			if (h1.content == L"") setContentEV;
			if (ENtoVnButton.isClicked(window, event)) {
				favType = 1;
				setContentVE();
			}

			handleChangePageEV();

			if (nextHisButton.isClicked(window, event))
				if (orderFav + 8 < favWordsEV.size()) orderFav += 8;
			if (backHisButton.isClicked(window, event))
				if (orderFav >= 8) orderFav -= 8;
		}
		else if (favType == 1) {
			if (VNtoEnButton.isClicked(window, event)) {
				favType = 2;
				setContentEE();
			}

			handleChangePageVE();

			if (nextHisButton.isClicked(window, event))
				if (orderFav + 8 < favWordsVE.size()) orderFav += 8;
			if (backHisButton.isClicked(window, event))
				if (orderFav >= 8) orderFav -= 8;
		}
		else {
			if (ENtoENButton.isClicked(window, event)) {
				favType = 0;
				setContentEV();
			}

			handleChangePageEE();

			if (nextHisButton.isClicked(window, event))
				if (orderFav + 8 < favWordsEE.size()) orderFav += 8;
			if (backHisButton.isClicked(window, event))
				if (orderFav >= 8) orderFav -= 8;
		}
		
	}


	if (favType == 0) {
		ENtoVnButton.draw(window);
		ENtoVnButton.isHover(window, "Image/ENtoVNHover.png");
		
	}
	else if (favType == 1) {
		VNtoEnButton.draw(window);
		VNtoEnButton.isHover(window, "Image/VNtoENHover.png");
	}
	else if (favType == 2) {
		ENtoENButton.draw(window);
		ENtoENButton.isHover(window, "Image/ENtoENHover.png");
	}

	h1.draw(window);
	h2.draw(window);
	h3.draw(window);
	h4.draw(window);
	h5.draw(window);
	h6.draw(window);
	h7.draw(window);
	h8.draw(window);

	nextHisButton.draw(window);
	nextHisButton.isHover(window, "Image/nextDefHover.png");
	backHisButton.draw(window);
	backHisButton.isHover(window, "Image/backDefHover.png");
	backButton.draw(window);
}

void homePage() {
	orderDef = 0;
	translateFlag = 0;
	searchFlag = 0;
	searchingType = 0;
	transType = 0;
	addingType = 0;

	h1.content = L"";
	h2.content = L"";
	h3.content = L"";
	h4.content = L"";
	h5.content = L"";
	h6.content = L"";
	h7.content = L"";
	h8.content = L"";

	sf::Texture layout;
	if (!layout.loadFromFile("Image/layout1.png"))
		return;
	layout.setSmooth(1);
	sf::Sprite spriteLayout;
	spriteLayout.setTexture(layout);
	spriteLayout.setOrigin(0, 0);
	window.draw(spriteLayout);

	searchButton.draw(window);
	translatingButton.draw(window);
	addNewWordButton.draw(window);
	qnaButton.draw(window);
	historyButton.draw(window);
	isLikedButton.draw(window);
	resetButton.draw(window);
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) window.close();

		if (searchButton.isClicked(window, event)) {
			page.push(1);
			searchKeyBox.text.setString("");
			searchDefBox.text.setString("");
		}
		if (translatingButton.isClicked(window, event)) {
			page.push(2);
			inputENBox.text.setString("");
			inputVNBox.text.setString("");
		}
		if (addNewWordButton.isClicked(window, event)) {
			page.push(3);
		}
		if (qnaButton.isClicked(window, event)) {
			page.push(4);
		}
		if (historyButton.isClicked(window, event)) {
			page.push(5);
		}
		if (isLikedButton.isClicked(window, event)) {
			page.push(6);
		}
	}
	addNewWordButton.isHover(window, "Image/addnewwordHover.png");
	searchButton.isHover(window, "Image/searchHover.png");
	translatingButton.isHover(window, "Image/translateHover.png");
	qnaButton.isHover(window, "Image/qnaHover.png");
	historyButton.isHover(window, "Image/historyHover.png");
	isLikedButton.isHover(window, "Image/likedHover.png");
	resetButton.isHover(window, "Image/resetButtonHover.png");
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

	auto start = chrono::high_resolution_clock::now();
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

	if (!Def::loadHashTable(rootDtoE, "Dataset/HashTableDef.bin")) {
		if (!Def::loadRawData(rootDtoE, 10000, "Dataset/englishDictionary.csv")) return 0;
		Def::saveHashtable(rootDtoE, "Dataset/HashTableDef.bin");
	}

	EV::loadFavWord(rootEtoV, favWordsEV, favDefsEV, "Dataset/favWordsEV.txt");
	VE::loadFavWord(rootVtoE, favWordsVE, favDefsVE, "Dataset/favWordsVE.txt");
	EE::loadFavWord(rootEtoE, favWordsEE, favDefsEE, "Dataset/favWordsEE.txt");

	auto end = chrono::high_resolution_clock::now();
	/*chrono::duration<double> duration = end - start;
	cout << L"Time taken to load dataset: " << duration.count() << " seconds" << endl;*/

	return 1;
}