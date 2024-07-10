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
sf::RenderWindow window(sf::VideoMode(1500,800), "Dictionary");
sf::Event event;
sf::View view = window.getView();
float scrollSpeed = 50.f;

Button searchButton(715, 155, "Image/searchButton.png");
Button translatingButton(715, 305, "Image/translateButton.png");
Button addNewWordButton(715, 455, "Image/addButton.png");
Button qnaButton(715, 605, "Image/qnaButton.png");

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

Button heartKeyButton(45, 715, "Image/heart.png");
Button deleteKeyButton(180, 715, "Image/deleteButton.png");

Button guessByKey(95, 28, "Image/guessByKey.png");
Button guessByDef(95, 28, "Image/guessByDef.png");

Button nextQuestion(1342, 170, "Image/nextButton.png");

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

AnswerButton A(209, 442, "Image/answerKeyBox.png");
AnswerButton B(209, 616, "Image/answerKeyBox.png");
AnswerButton C(849, 442, "Image/answerKeyBox.png");
AnswerButton D(849, 616, "Image/answerKeyBox.png");

AnswerButton ADef(102, 295, "Image/answerDefBox.png");
AnswerButton BDef(102, 543, "Image/answerDefBox.png");
AnswerButton CDef(804, 295, "Image/answerDefBox.png");
AnswerButton DDef(804, 543, "Image/answerDefBox.png");


sf::Font font;
vector<wstring> transDef;
vector<string> searchDef;

EVTrie* rootEtoV = new EVTrie();
EETrie* rootEtoE = new EETrie();
VTrie* rootVtoE = new VTrie();
HashTable rootDtoE;

int page = 0;
int orderDef = 0;
bool translateFlag = 0;
bool searchFlag = 0;
bool transType = 0;
bool searchingType = 0;
int addingType = 0;
int qnaType = 0;

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
		if (backButton.isClicked(window, event)) page = 0;
		backButton.isHover(window, "Image/backHover.png");

		if (event.type == sf::Event::Closed) window.close();

		if (transType == 0) {//Vietnamese to English
			if (VNtoEnButton.isClicked(window, event)) {
				transType = 1;
			}
			inputVNBox.isClicked(window, event);

			if (translateVN.isClicked(window, event, transWword, inputVNBox.text)) {
				transDef.clear();
				if (!VE::findWordMeaning(rootVtoE, transWword, transDef,nodeV)) transDef.push_back(L"No definition");
				translateFlag = 1;
				orderDef = 0;
			}
			if (heartButton.isClicked(window, event)) {
				if (nodeV != nullptr) nodeV->isLiked = 1 - nodeV->isLiked;
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
			}
			inputENBox.isClicked(window, event);
			if (translateEN.isClicked(window, event, transWord, inputENBox.text)) {
				transDef.clear();
				translateFlag = 1;
				orderDef = 0;
				if (!EV::findWordMeaning(rootEtoV, transWord, transDef,nodeE)) transDef.push_back(L"No definition");
			}
			if (heartButton.isClicked(window, event)) {
				if (nodeV != nullptr) nodeV->isLiked = 1 - nodeV->isLiked;
			}
			if (deleteButton.isClicked(window, event)) {
				EV::deleteAWord(rootEtoV, transWord);
				inputVNBox.text.setString("");
				transDef.clear();
				translateFlag = 0;
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
		if (nodeV!=nullptr && nodeV->isLiked == 0) {
			heartButton.texture.loadFromFile("Image/heart.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
		else if (nodeV!=nullptr && nodeV->isLiked == 1) {
			heartButton.texture.loadFromFile("Image/heartHover.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
		VNtoEnButton.isHover(window, "Image/VNtoENHover.png");
		translateVN.isHover(window, "Image/transSubHover.png");
		translateVN.draw(window);

	}
	else {//English to Vietnamese
		if (translateFlag == 1) {
			displayWDef(870, 300, transDef[orderDef], 30);
		}
		if (nodeE!=nullptr && nodeE->isLiked == 0) {
			heartButton.texture.loadFromFile("Image/heart.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
		else if (nodeE!=nullptr && nodeE->isLiked == 1) {
			heartButton.texture.loadFromFile("Image/heartHover.png");
			heartButton.sprite.setTexture(heartButton.texture);
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

EETrie* nodeEE = nullptr;
std::string def = "";
std::string word = "";

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

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event)) page = 0;
		backButton.isHover(window, "Image/backHover.png");

		if (searchKeyButton.isClicked(window, event)) searchingType = 0;
		if (searchDefButton.isClicked(window, event)) searchingType = 1;

		if (searchingType == 0) {//Search with keyword
			word = "";
			def = "";
			searchKeyBox.isClicked(window, event);

			if (submitSearchKey.isClicked(window, event, word, searchKeyBox.text)) {
				searchDef.clear();
				if(!EE::findWordMeaning(rootEtoE, word, searchDef,nodeEE)) searchDef.push_back("No definition");
				searchFlag = 1;
				orderDef = 0;
			}

			if (heartKeyButton.isClicked(window, event)) {
				if (nodeEE != nullptr) nodeEE->isLiked = 1 - nodeEE->isLiked;
			}
			if (deleteKeyButton.isClicked(window, event)) {
				EE::deleteAWord(rootEtoE, word);
				inputVNBox.text.setString("");
				transDef.clear();
				translateFlag = 0;
			}

			if (nextDefButton.isClicked(window, event) && orderDef < searchDef.size() - 1) orderDef++;
			if (backDefButton.isClicked(window, event) && orderDef > 0) orderDef--;
		}
		else {//search with definition
			word = "";
			def = "";
			searchDefBox.isClicked(window, event);
			if (submitSearchDef.isClicked(window, event, def, searchDefBox.text)) {
				word = Def::findWordMeaning(rootDtoE, def);
			}
		}
		
	}
	if (searchingType == 0) { //search by keyword
		searchKeyBox.draw(window);

		if (searchFlag == 1) {
			nextDefButton.draw(window);
			backDefButton.draw(window);

			displayDef(650, 100, searchDef[orderDef], 50);
			if (nodeEE != nullptr && nodeEE->isLiked == 0) {
				heartKeyButton.texture.loadFromFile("Image/heart.png");
				heartKeyButton.sprite.setTexture(heartButton.texture);
			}
			else if (nodeE != nullptr && nodeE->isLiked == 1) {
				heartKeyButton.texture.loadFromFile("Image/heartHover.png");
				heartKeyButton.sprite.setTexture(heartButton.texture);
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

		searchDefButton.texture.loadFromFile("Image/searchDefHover.png");
		searchKeyButton.texture.loadFromFile(searchKeyButton.orgImage);
		submitSearchDef.draw(window);
		submitSearchDef.isHover(window, "Image/searchSubmitHover.png");
	}

	

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


std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
std::wstring wrightWord;
std::wstring wrightDef;

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
			sprite.setPosition(posx + 500, posy + 150);
			window.draw(sprite);
		}
	}
	else if (answerFlag == 1) {
		sf::Texture correct;
		if (correct.loadFromFile("Image/tick.png")) {
			sf::Sprite sprite;
			sprite.setTexture(correct);
			sprite.setPosition(posx + 500, posy + 150);
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
		if (backButton.isClicked(window, event)) page = 0;
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
			}
		}
		else {
			if (ENtoENButton.isClicked(window, event)) {
				qnaType = 0;
			}
			if (nextQuestion.isClicked(window, event)) {
				answerFlag = 2;

				if (guessType == 0) {//guess by definition
					ansWord.clear();

					std::string right;
					EE::randomADefinitionAnd4Words(rootEtoE, right, rightWord, wrongWord1, wrongWord2, wrongWord3);
					std::cout << right << endl;

					wrightDef = converter.from_bytes(rightDef);
					handleWString(wrightDef, 80);
					ansWord.push_back(converter.from_bytes(rightWord));
					ansWord.push_back(converter.from_bytes(wrongWord1));
					ansWord.push_back(converter.from_bytes(wrongWord2));
					ansWord.push_back(converter.from_bytes(wrongWord3));

					randomKeyAnswer(ansWord, 3);
					memset(rangeRandom, 1, 4);
				}
				else {	
					EE::randomAWordAnd4Definitions(rootEtoE, rightWord, rightDef, wrongDef1, wrongDef2, wrongDef3);
					
					wrightWord = converter.from_bytes(rightWord);
					ansDef.push_back(converter.from_bytes(rightDef));
					handleWString(ansDef[0],40);
					ansDef.push_back(converter.from_bytes(wrongDef1));
					handleWString(ansDef[1], 40);
					ansDef.push_back(converter.from_bytes(wrongDef2));
					handleWString(ansDef[2], 40);
					ansDef.push_back(converter.from_bytes(wrongDef3));
					handleWString(ansDef[3], 40);

					randomDefAnswer(ansDef, 3);
					memset(rangeRandom, 1, 4);
				}
			}
		}

		//hanlde guess by keyword or definition
		if (guessType == 0) {
			if (guessByDef.isClicked(window, event)) guessType = 1;
			handleKeyAnswer();
		}
		else {
			if (guessByKey.isClicked(window, event)) guessType = 0;
			handleDefAnswer();
		}


	}

	if (qnaType == 0) {
		VNtoEnButton.draw(window);
		VNtoEnButton.isHover(window,"Image/VNtoENHover.png");
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
	qnaButton.draw(window);
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
		if (qnaButton.isClicked(window, event)) {
			page = 4;
		}
		addNewWordButton.isHover(window, "Image/addnewwordHover.png");
		searchButton.isHover(window, "Image/searchHover.png");
		translatingButton.isHover(window, "Image/translateHover.png");
		qnaButton.isHover(window, "Image/qnaHover.png");
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

	/*if (!EV::loadTriefromFile(rootEtoV, "Dataset/TrieENVN.bin")) {
		if (!EV::loadRawData(rootEtoV, "Dataset/ENVN.txt")) return 0;
		EV::saveTrietoFile(rootEtoV, "Dataset/TrieENVN.bin");
	}*/
	if (!EE::loadTrieFromFile(rootEtoE, "Dataset/TrieEN.bin")) {
		if (!EE::loadRawData(rootEtoE, "Dataset/englishDictionary.csv")) return 0;
		EE::saveTrietoFile(rootEtoE, "Dataset/TrieEN.bin");
	}
	/*if (!VE::loadTrieFromFile(rootVtoE, "Dataset/TrieVNEN.bin")) {
		if (!VE::loadRawData(rootVtoE, "Dataset/VE.csv")) return 0;
		VE::saveTrieToFile(rootVtoE, "Dataset/TrieVNEN.bin");
	}
	
	if (!Def::loadHashTable(rootDtoE, "Dataset/HashTableDef.bin")) {
		if (!Def::loadRawData(rootDtoE, 10000, "Dataset/Definition.bin")) return 0;
		Def::saveHashtable(rootDtoE, "Dataset / HashTableDef.bin");
	}*/

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
		case 3: {
			adding();
			break;
		}
		case 4: {
			QnA();
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


