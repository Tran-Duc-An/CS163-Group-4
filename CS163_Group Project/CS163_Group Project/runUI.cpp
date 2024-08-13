#include "Function.h"
#include "UI.h"
#include <chrono>
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <locale>
#include <random>
#include <SFML/Graphics.hpp>
#include <stack>
#include <thread>
#include <fstream>

 sf::RenderWindow window(sf::VideoMode(1500, 800), "Dictionary");
 sf::Event event;

// home page
Button searchButton(850, 200, "Image/searchButton.png");
Button translatingButton(850, 350, "Image/translateButton.png");
Button addNewWordButton(850, 500, "Image/addButton.png");
Button extendButton(1350, 100, "Image/extendButton.png");

Button qnaButton(1350, 400, "Image/qnaButton.png");
Button emojiSearchButton(1350, 250, "Image/emojiButton.png");
Button randomWordButton(1350,550, "Image/randomButton.png");
Button extendLayout(1350, 220, "Image/extendLayout.png");

Button historyButton(30, 350, "Image/historyButton.png");
Button isLikedButton(30, 500, "Image/likedButton.png");
Button resetButton(30, 650, "Image/resetButton.png");


//switch dataset
Button VNtoEnButton(1034, 33, "Image/VNtoENButton.png");
Button ENtoVnButton(1034, 33, "Image/ENtoVNButton.png");
Button ENtoENButton(1034, 33, "Image/ENtoENButton.png");

//function button
Button backButton(12, 18, "Image/backButton.png");

Button nextMeanButton(1300, 710, "Image/nextButton.png");
Button backMeanButton(900, 710, "Image/backDefButton.png");

Button heartButton(183, 600, "Image/heart.png");
Button deleteButton(349, 600, "Image/deleteButton.png");
Button submitResetButton(1200, 582, "Image/submitResetButton.png");
Button tickEEButton(80, 100, "Image/untickedboxEE.png");
Button tickEVButton(80, 300, "Image/untickedboxEV.png");
Button tickVEButton(80, 500, "Image/untickedboxVE.png");

Button editButton(1300, 260, "Image/editButton.png");

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
Button guessByKey(130, 28, "Image/guessByKey.png");
Button guessByDef(130, 28, "Image/guessByDef.png");
Button nextQuestion(1360, 170, "Image/nextButton.png");

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

//Edit
ChoiceButton editedWord(192, 117, "Image/InputBox.png");
InputDef inputEditedDef(192, 400, "Image/InputDef.png", L"Enter definition", 6, 50);
Button submitEdit(1157, 238, "Image/submitResetButton.png");

// hint
Button guideUserTickButton(160, 13, "Image/hintToTick.png");
Button warningButton(45, 582, "Image/warning.png");


sf::Font font;
sf::Font fontEmoji;
vector<wstring> transDef;
vector<string> searchDef;

EVTrie* rootEtoV = new EVTrie();
EETrie* rootEtoE = new EETrie();
VTrie* rootVtoE = new VTrie();
vector<pair<string, string>> table;
Emo emojiTable;

vector<std::wstring> searchHistory;
vector<std::wstring> searchRealTime;

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

stack<int> page;
int orderDef = 0;
bool translateFlag = 0;
bool searchFlag = 0;
bool transType = 0;

int addingType = 0;
int qnaType = 0;

int editType = 0;

vector<wstring> favWordsVE;
vector<wstring> favDefsVE;

vector<string> favWordsEV;
vector<wstring> favDefsEV;

vector<string> favWordsEE;
vector<string> favDefsEE;


int run() {
	setBackground();
	font.loadFromFile("Font/ARIAL.TTF");
	if (!loadData()) return 0;
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
		case 7: {
			emoji();
			break;
		}
		case 8: {
			reset();
			break;
		}
		case 9: {
			edit();
			break;
		}
		case 10: {
			randomWords();
			break;
		}
		default:
			break;
		}
		window.display();
		window.clear(sf::Color::White);
	}
	auto start = chrono::high_resolution_clock::now();

	// call thread with reference to save data faster
	thread t1(EV::saveFavWord, ref(favWordsEV), ref(favDefsEV), "Dataset/favWordsEV.txt");
	thread t2(VE::saveFavWord, ref(favWordsVE), ref(favDefsVE), "Dataset/favWordsVE.txt");
	thread t3(EE::saveFavWord, ref(favWordsEE), ref(favDefsEE), "Dataset/favWordsEE.txt");
	thread t4(EE::saveTrietoFile, ref(rootEtoE), "Dataset/UserTrieEN.bin");
	thread t5(VE::saveTrieToFile, ref(rootVtoE), "Dataset/UserTrieVNEN.bin");
	thread t6(EV::saveTrietoFile, ref(rootEtoV), "Dataset/UserTrieENVN.bin");
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();



	thread t7(EV::deleteTrie, ref(rootEtoV));
	thread t8(VE::deleteTrie, ref(rootVtoE));
	thread t9(EE::deleteTrie, ref(rootEtoE));
	t7.join();
	t8.join();
	t9.join();

	auto end = chrono::high_resolution_clock::now();
	// print out time to save and delete data in ms
	wcout << L"Time to save and delete data: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << L"ms" << endl;

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
					s = s.substr(0, l-3) + L"...";
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
		if (backButton.isClicked(window, event)) {
			page.pop();
			inputENBox.reset();
			inputVNBox.reset();
		}
		

		if (event.type == sf::Event::Closed) window.close();

		

		if (transType == 0) {//Vietnamese to English
			if (VNtoEnButton.isClicked(window, event)) {
				transType = 1;
				translateFlag = 0;
			}
			inputVNBox.isClicked(window, event);

			if (translateVN.isClicked(window, event, transWword, inputVNBox.text)) {
			translateVNtoEN:

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

			if (editButton.isClicked(window, event)) {
				page.push(9);
				editType = 0;
				editedWord.content = transWword;
				handleWString(transDef[orderDef],50,7);
				inputEditedDef.text.setString(transDef[orderDef]);
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

			if (editButton.isClicked(window, event)) {
				page.push(9);
				editType = 1;
				editedWord.content = converter.from_bytes(transWord);
				handleWString(transDef[orderDef], 50, 7);
				inputEditedDef.text.setString(transDef[orderDef]);
			}
		}
		if (nextMeanButton.isClicked(window, event) && orderDef < transDef.size() - 1) orderDef++;
		if (backMeanButton.isClicked(window, event) && orderDef > 0) orderDef--;

	
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
		
		editButton.draw(window);
		editButton.isHover(window, "Image/editButtonHover.png");

		if (orderDef > 0) {
			backMeanButton.draw(window);
			backMeanButton.isHover(window, "Image/backDefHover.png");
		}
		if (orderDef < transDef.size() - 1) {
			nextMeanButton.draw(window);
			nextMeanButton.isHover(window, "Image/nextDefHover.png");
		}
		heartButton.draw(window);
		deleteButton.draw(window);
		deleteButton.isHover(window, "Image/deleteHover.png");

	}

	backButton.draw(window);
	backButton.isHover(window, "Image/backHover.png");
}

EETrie* nodeEE = nullptr;
std::string def = "";
std::string word = "";
stack<bool> searchingType;
vector<string> words;
int orderKey = 0;
ChoiceButton searchDefRes(800, 125, "Image/searchDefRes.png");
Button nextKeyButton(1335, 320, "Image/nextButton.png");
Button backKeyButton(700, 320, "Image/backDefButton.png");

Button editSearchButton(1335, 50, "Image/editButton.png");

Button nextDefButton(1300, 700, "Image/nextButton.png");
Button backDefButton(800, 700, "Image/backDefButton.png");

void searching() {
	sf::Texture layout2;
	if (searchingType.top() == 0) {
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

		if (backButton.isClicked(window, event)) {
			page.pop();
			if (!searchingType.empty()) searchingType.pop();

			searchKeyBox.reset();
			searchDefBox.reset();
			return;
		}


		if (searchKeyButton.isClicked(window, event)) {
			searchingType.push(0);
			searchFlag = 0;	
		}
		if (searchDefButton.isClicked(window, event)) {
			searchingType.push(1);
			searchFlag = 0;
		}
		if (!searchingType.empty()) {
			if (searchingType.top() == 0) {//Search with keyword

				searchKeyBox.isClicked(window, event);

				if (submitSearchKey.isClicked(window, event, word, searchKeyBox.text)) {

				searchwithKeyword:

					if (!searchDef.empty()) searchDef.clear();
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
				
				if (editSearchButton.isClicked(window, event)) {
					page.push(9);
					editType = 2;
					editedWord.content = converter.from_bytes(word);
					handleString(searchDef[orderDef],50,7);
					inputEditedDef.text.setString(searchDef[orderDef]);
				}
			}
			else {//search with definition
				searchDefBox.isClicked(window, event);
				if (submitSearchDef.isClicked(window, event, def, searchDefBox.text)) {
					removeEndline(def);
					orderKey = 0;
					if (!words.empty()) words.clear();
					words = Def::searchByDef(table, def);
					if (!words.empty())
						searchFlag = 1;
				}

				if (searchDefRes.isClicked(window, event)) {
					page.push(1);
					searchingType.push(0);
					searchKeyBox.text.setString(searchDefRes.content);
					word = converter.to_bytes(searchDefRes.content);
					goto searchwithKeyword;
				}
				if (nextKeyButton.isClicked(window, event) && orderKey < words.size() - 1) orderKey++;
				if (backKeyButton.isClicked(window, event) && orderKey > 0) orderKey--;
			}

		}

	}
	if (!searchingType.empty()) {
		if (searchingType.top() == 0) { //search by keyword
			searchKeyBox.draw(window);
			if (searchKeyBox.text.getString() == "") {
				heartButton.texture.loadFromFile("Image/heart.png");
				heartButton.sprite.setTexture(heartButton.texture);
				searchFlag = 0;
			}
			if (searchFlag == 1) {
				if (orderDef < searchDef.size() - 1) {
					nextDefButton.draw(window);
					nextDefButton.isHover(window, "Image/nextDefHover.png");
				}
				if (orderDef > 0) {
					backDefButton.draw(window);
					backDefButton.isHover(window, "Image/backDefHover.png");
				}

				displayDef(650, 150, searchDef[orderDef], 50);
				if (nodeEE != nullptr && nodeEE->isLiked == 0) {
					heartKeyButton.texture.loadFromFile("Image/heart.png");
					heartKeyButton.sprite.setTexture(heartKeyButton.texture);
				}
				else if (nodeEE != nullptr && nodeEE->isLiked == 1) {
					heartKeyButton.texture.loadFromFile("Image/heartHover.png");
					heartKeyButton.sprite.setTexture(heartKeyButton.texture);
				}

				editSearchButton.draw(window);
				editSearchButton.isHover(window, "Image/editButtonHover.png");

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
				searchDefRes.content = converter.from_bytes(words[orderKey]);

				searchDefRes.draw(window);
				if (orderKey < words.size() - 1) {
					nextKeyButton.draw(window);
					nextKeyButton.isHover(window, "Image/nextDefHover.png");
				}
				if (orderKey > 0) {
					backKeyButton.draw(window);
					backKeyButton.isHover(window, "Image/backDefHover.png");
				}
			}

			if (searchDefBox.text.getString() == "") searchFlag = 0;

			searchDefButton.texture.loadFromFile("Image/searchDefHover.png");
			searchKeyButton.texture.loadFromFile(searchKeyButton.orgImage);
			submitSearchDef.draw(window);
			submitSearchDef.isHover(window, "Image/searchSubmitHover.png");
		}
	}


	searchKeyButton.draw(window);
	searchDefButton.draw(window);
	backButton.draw(window);
	backButton.isHover(window, "Image/backHover.png");
}

void adding() {
	std::string word;
	std::wstring wword;
	std::string def;
	std::wstring wdef;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();
		if (backButton.isClicked(window, event)) {
			page.pop();
			inputWord.reset();
			inputDef.reset();
		}
		


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
	backButton.isHover(window, "Image/backHover.png");
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

std::wstring wWrongWord1;
std::wstring wWrongWord2;
std::wstring wWrongWord3;


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
	qSprite.setPosition(105, 115);
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
		

		//change type of question
		if (qnaType == 0) {//VE
			if (VNtoEnButton.isClicked(window, event)) {
				qnaType = 1;
				answerFlag = 2;
			}
			if (nextQuestion.isClicked(window, event)) {

				answerFlag = 2;

				if (guessType == 0) {//guess by definition
					ansWord.clear();

					wrightDef = L"";
					wrightWord = L"";
					wWrongWord1 = L"";
					wWrongWord2 = L"";
					wWrongWord3 = L"";

					VE::randomADefinitionAnd4Words(rootVtoE, wrightDef, wrightWord, wWrongWord1, wWrongWord2, wWrongWord3);
					handleWString(wrightDef, 80, 6);
					ansWord.push_back(wrightWord);
					ansWord.push_back(wWrongWord1);
					ansWord.push_back(wWrongWord2);
					ansWord.push_back(wWrongWord3);

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
		else if (qnaType == 1) {//EV

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
		else {//EE

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
	text.setPosition(120, 115);

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

	backButton.draw(window);
	backButton.isHover(window, "Image/backHover.png");
	nextQuestion.draw(window);
	nextQuestion.isHover(window, "Image/nextDefHover.png");
}

ChoiceButton h1(180, 110, "Image/displayBox.png");
ChoiceButton h2(180, 280, "Image/displayBox.png");
ChoiceButton h3(180, 450, "Image/displayBox.png");
ChoiceButton h4(180, 620, "Image/displayBox.png");
ChoiceButton h5(780, 110, "Image/displayBox.png");
ChoiceButton h6(780, 280, "Image/displayBox.png");
ChoiceButton h7(780, 450, "Image/displayBox.png");
ChoiceButton h8(780, 620, "Image/displayBox.png");

Button nextHisButton(1325, 700, "Image/nextButton.png");
Button backHisButton(20, 700, "Image/backDefButton.png");

int orderHis = 0;

void setHisContent() {
	if (orderHis < searchHistory.size()) {
		h1.content = searchHistory[orderHis];
		handleWString(h1.content,35,2);
		h1.content += L"\n" + std::wstring(30, L' ') + searchRealTime[orderHis];
	}
	if (orderHis + 1 < searchHistory.size()) {
		h2.content = searchHistory[orderHis + 1];
		handleWString(h2.content, 35, 2);
		h2.content += L"\n" + std::wstring(30, L' ') + searchRealTime[orderHis + 1];
	}
	if (orderHis + 2 < searchHistory.size()) {
		h3.content = searchHistory[orderHis + 2];
		handleWString(h3.content, 35, 2);
		h3.content += L"\n" + std::wstring(30, L' ')  + searchRealTime[orderHis + 2];
	}
	if (orderHis + 3 < searchHistory.size()) {
		h4.content = searchHistory[orderHis + 3];
		handleWString(h4.content, 35, 2);
		h4.content += L"\n" + std::wstring(30, L' ') + searchRealTime[orderHis + 3];
	}
	if (orderHis + 4 < searchHistory.size()) {
		h5.content = searchHistory[orderHis + 4];
		handleWString(h5.content, 35, 2);
		h5.content += L"\n" + std::wstring(30, L' ') + searchRealTime[orderHis + 4];
	}
	if (orderHis + 5 < searchHistory.size()) {
		h6.content = searchHistory[orderHis + 5];
		handleWString(h6.content, 35, 2);
		h6.content += L"\n" + std::wstring(30, L' ') + searchRealTime[orderHis + 5];
	}
	if (orderHis + 6 < searchHistory.size()) {
		h7.content = searchHistory[orderHis + 6];
		handleWString(h7.content, 35, 2);
		h7.content += L"\n" + std::wstring(30, L' ') + searchRealTime[orderHis + 6];
	}
	if (orderHis + 7 < searchHistory.size()) {
		h8.content = searchHistory[orderHis + 7];
		handleWString(h8.content, 35, 2);
		h8.content += L"\n" + std::wstring(30, L' ') + searchRealTime[orderHis + 7];
	}
}

void history() {

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event)) page.pop();
		backButton.isHover(window, "Image/backHover.png");

		if (nextHisButton.isClicked(window, event)) {
			if (orderHis + 8 < searchHistory.size()) {
				orderHis += 8;
				setHisContent();
			}
		}
		if (backHisButton.isClicked(window, event)) {
			if (orderHis-8 >= 0) {
				orderHis -= 8;
				setHisContent();
			}
		}
	}

	if (h1.content == L"") setHisContent();

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

int favType = 0;
int orderFav = 0;

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
	if (h1.isClicked(window, event) == 1) {
		page.push(2);
		transType = 1;
		if (orderFav < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav]);
	}
	if (h2.isClicked(window, event) == 1) {
		page.push(2);
		transType = 1;
		if (orderFav+1 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+1]);
	}
	if (h3.isClicked(window, event) == 1) {
		page.push(2);
		transType = 1;
		if (orderFav+2 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+2]);
	}
	if (h4.isClicked(window, event) == 1) {
		page.push(2);
		transType = 1;
		if (orderFav+3 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+3]);
	}
	if (h5.isClicked(window, event) == 1) {
		page.push(2);
		transType = 1;
		if (orderFav+4 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+4]);
	}
	if (h6.isClicked(window, event) == 1) {
		page.push(2);
		transType = 1;
		if (orderFav+5 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+5]);
	}
	if (h7.isClicked(window, event) == 1) {
		page.push(2);
		transType = 1;
		if (orderFav+6< favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+6]);
	}
	if (h8.isClicked(window, event) == 1) {
		page.push(2);
		transType = 1;
		if (orderFav+7 < favWordsEV.size())
			inputENBox.text.setString(favWordsEV[orderFav+7]);
	}
}

void handleChangePageVE() {
	if (h1.isClicked(window, event) == 1) {
		page.push(2);
		transType = 0;
		if (orderFav < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav]);
	}
	if (h2.isClicked(window, event) == 1) {
		page.push(2);
		transType = 0;
		if (orderFav+1 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+1]);
	}
	if (h3.isClicked(window, event) == 1) {
		page.push(2);
		transType = 0;
		if (orderFav+2 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+2]);
	}
	if (h4.isClicked(window, event) == 1) {
		page.push(2);
		transType = 0;
		if (orderFav+3 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+3]);
	}
	if (h5.isClicked(window, event) == 1) {
		page.push(2);
		transType = 0;
		if (orderFav+4 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+4]);
	}	
	if (h6.isClicked(window, event) == 1) {
		page.push(2);
		transType = 0;
		if (orderFav+5 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+5]);
	}
	if (h7.isClicked(window, event) == 1) {
		page.push(2);
		transType = 0;
		if (orderFav+6 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+6]);
	}
	if (h8.isClicked(window, event) == 1) {
		page.push(2);
		transType = 0;
		if (orderFav+7 < favWordsVE.size())
			inputVNBox.text.setString(favWordsVE[orderFav+7]);
	}

}

void handleChangePageEE() {
	if (h1.isClicked(window, event) == 1) {
		page.push(1);
		searchingType.push(0);
		if (orderFav < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav]);
	}
	if (h2.isClicked(window, event) == 1) {
		page.push(1);
		searchingType.push(0);
		if (orderFav+1 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+1]);
	}
	if (h3.isClicked(window, event) == 1) {
		page.push(1);
		searchingType.push(0);
		if (orderFav+2 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+2]);
	}
	if (h4.isClicked(window, event) == 1) {
		page.push(1);
		searchingType.push(0);
		if (orderFav+3 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+3]);
	}
	if (h5.isClicked(window, event) == 1) {
		page.push(1);
		searchingType.push(0);
		if (orderFav+4 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+4]);
	}
	if (h6.isClicked(window, event) == 1) {
		page.push(1);
		searchingType.push(0);
		if (orderFav+5 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+5]);
	}
	if (h7.isClicked(window, event) == 1) {
		page.push(1);
		searchingType.push(0);
		if (orderFav+6 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+6]);
	}
	if (h8.isClicked(window, event) == 1) {
		page.push(1);
		searchingType.push(0);
		if (orderFav+7 < favWordsEE.size())
			searchKeyBox.text.setString(favWordsEE[orderFav+7]);
	}

}

void isLiked() {
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event)) {
			page.pop();
			favType = 0;
			orderFav = 0;
		}
		backButton.isHover(window, "Image/backHover.png");

		if (favType == 0) {
			if (h1.content == L"") setContentEV;
			if (ENtoVnButton.isClicked(window, event)) {
				orderFav = 0;
				favType = 1;
			}

			handleChangePageEV();

			if (nextHisButton.isClicked(window, event))
				if (orderFav + 8 < favWordsEV.size()) {
					orderFav += 8;
				}
			if (backHisButton.isClicked(window, event))
				if (orderFav >= 8) {
					orderFav -= 8;
				}
		}
		else if (favType == 1) {
			if (VNtoEnButton.isClicked(window, event)) {
				orderFav = 0;
				favType = 2;
			}

			handleChangePageVE();

			if (nextHisButton.isClicked(window, event))
				if (orderFav + 8 < favWordsVE.size()) {
					orderFav += 8;
				}
			if (backHisButton.isClicked(window, event))
				if (orderFav >= 8) {
					orderFav -= 8;
				}
		}
		else {
			if (ENtoENButton.isClicked(window, event)) {
				orderFav = 0;
				favType = 0;
			}

			handleChangePageEE();

			if (nextHisButton.isClicked(window, event))
				if (orderFav + 8 < favWordsEE.size()) {
					orderFav += 8;
				}
			if (backHisButton.isClicked(window, event))
				if (orderFav >= 8) {
					orderFav -= 8;
				}
		}
		
	}


	if (favType == 0) {
		ENtoVnButton.draw(window);
		ENtoVnButton.isHover(window, "Image/ENtoVNHover.png");
		setContentEV();
		if (orderFav > 0) {
			backHisButton.draw(window);
			backHisButton.isHover(window, "Image/backDefHover.png");
		}
		if (orderFav < favDefsEV.size() - 1) {
			nextHisButton.draw(window);
			nextHisButton.isHover(window, "Image/nextDefHover.png");
		}
	}
	else if (favType == 1) {
		VNtoEnButton.draw(window);
		VNtoEnButton.isHover(window, "Image/VNtoENHover.png");
		setContentVE();
		if (orderFav > 0) {
			backHisButton.draw(window);
			backHisButton.isHover(window, "Image/backDefHover.png");
		}
		if (orderFav < favDefsVE.size() - 1) {
			nextHisButton.draw(window);
			nextHisButton.isHover(window, "Image/nextDefHover.png");
		}
	}
	else if (favType == 2) {
		ENtoENButton.draw(window);
		ENtoENButton.isHover(window, "Image/ENtoENHover.png");
		setContentEE();
		if (orderFav > 0) {
			backHisButton.draw(window);
			backHisButton.isHover(window, "Image/backDefHover.png");
		}
		if (orderFav < favDefsEE.size() - 1) {
			nextHisButton.draw(window);
			nextHisButton.isHover(window, "Image/nextDefHover.png");
		}
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

Button findbyCodeButton(1045, 35, "Image/findByCode.png");
Button findbyWordButton(1045, 35, "Image/findByWord.png");
bool emojiType = 0;

Button desTag(140, 145, "Image/Destag.png");
Button emojiTag(820, 145, "Image/Emojitag.png");
InputDef emojiInput(170, 325, "Image/InputBox.png",L"Text Here", 3, 20);
SubmitENButton searchEmoji(650, 490, "Image/searchImage.png");
Button nextEmo(1400, 700, "Image/nextButton.png");
Button prevEmo(800, 700, "Image/backDefButton.png");

string emo;
vector<pair<string, string>> listEmoji;
pair<string, string> emoCode;
int orderEmo = 0;
bool isFound = 0;
string path;

void emoji() {
	sf::Texture displayEmo;
	

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event)) {
			page.pop();
			emojiInput.reset();
		}

		emojiInput.isClicked(window,event);
		if (searchEmoji.isClicked(window, event,emo,emojiInput.text)) {
			isFound = 1;
			orderEmo = 0;
			if (!listEmoji.empty()) listEmoji.clear();
			removeEndline(emo);
			if (emojiType == 0) {
				listEmoji = Emoji::findbyNameUntil(emojiTable, emo);
				if (listEmoji.empty()) isFound = 0;
			}
			else {
				emoCode = Emoji::findBycode(emojiTable, emo);
				if (emoCode.first == "") isFound = 0;
			}
	

		}
		if (emojiType == 0) {
			if (findbyWordButton.isClicked(window, event)) {
				emojiType = 1;
				isFound = 0;
			}
			if (nextEmo.isClicked(window, event) && orderEmo < listEmoji.size() - 1)
				orderEmo++;
			if (prevEmo.isClicked(window, event) && orderEmo > 0)
				orderEmo--;
		}
		else {
			if (findbyCodeButton.isClicked(window, event)) {
				emojiType = 0;
				isFound = 0;
			}
		}

	}
	emojiInput.draw(window);

	desTag.draw(window);
	emojiTag.draw(window);


	emojiInput.draw(window);
	searchEmoji.draw(window);
	searchEmoji.isHover(window, "Image/searchImageHover.png");
	
	backButton.draw(window);
	backButton.isHover(window, "Image/backHover.png");
	
	if (emojiInput.text.getString() == "") isFound = 0;
	

	if (emojiType == 0) {
		findbyWordButton.draw(window);
		findbyWordButton.isHover(window, "Image/findByWordHover.png");
	}
	else {

		findbyCodeButton.draw(window);
		findbyCodeButton.isHover(window, "Image/findByCodeHover.png");
		
	}

	if (isFound == 1) {

		string content;
		if (emojiType == 0) {
			content = listEmoji[orderEmo].first;
			if (!listEmoji.empty())
				path = "Emoji-PNG/" + listEmoji[orderEmo].second + "-" + listEmoji[orderEmo].first + ".png";

		}
		else {
			content = emoCode.first;
			if (emoCode.first != "")
				path = "Emoji-PNG/" + emoCode.second + "-" + emoCode.first + ".png";
		}
		if (displayEmo.loadFromFile(path)) {
			sf::Sprite sprite;
			sprite.setTexture(displayEmo);
			sprite.setPosition(900, 300);
			window.draw(sprite);
		}

		sf::Text text;
		text.setFont(font);
		text.setFillColor(sf::Color::Black);
		text.setCharacterSize(40);
		text.setPosition(900, 400);

		handleString(content, 20, 2);
		text.setString(content);
		window.draw(text);

		if (orderEmo < listEmoji.size() - 1) {
			nextEmo.draw(window);
			nextEmo.isHover(window, "Image/nextDefHover.png");
		}
		if (orderEmo > 0) {
			prevEmo.draw(window);
			prevEmo.isHover(window, "Image/backDefHover.png");
		}
	}
}

bool ee = 0, ev = 0, ve = 0;
void reset()
{

	// hint
	sf::Text guideUser;
	sf::Text warning;
	guideUser.setFont(font);
	guideUser.setCharacterSize(30);
	guideUser.setFillColor(sf::Color::Black);
	guideUser.setPosition(181, 59);
	guideUser.setString("Please tick the box to choose the type of dictionary you want to reset");

	warning.setFont(font);
	warning.setCharacterSize(30);
	warning.setFillColor(sf::Color::Red);
	warning.setPosition(70, 700);
	warning.setString("This will reset your dictionary to original state,\n including added, edited, deleted words !");
	window.draw(guideUser);
	window.draw(warning);
	tickEEButton.draw(window);
	tickEVButton.draw(window);
	tickVEButton.draw(window);
	submitResetButton.draw(window);
	backButton.draw(window);
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event))
		{
			page.pop();
		}
		if (tickEEButton.isClicked(window, event))
		{
			if (ee) ee = 0;
			else ee = 1;
		}
		if (tickEVButton.isClicked(window, event))
		{
			if (ev) ev = 0;
			else ev = 1;
		}
		if (tickVEButton.isClicked(window, event))
		{
			if (ve) ve = 0;
			else ve = 1;
		}
		if (submitResetButton.isClicked(window, event))
		{
			resetToOriginal(ee, ev, ve, rootEtoE, rootEtoV, rootVtoE);
			ee = 0;
			ev = 0;
			ve = 0;
			page.pop();
		}

	}
	if (ee)
	{
		sf::Texture isChosen;
		if (!isChosen.loadFromFile("Image/tickedboxEE.png"))
			return;
		isChosen.setSmooth(1);
		tickEEButton.sprite.setTexture(isChosen);
		tickEEButton.draw(window);
	}
	else
	{
		sf::Texture isChosen;
		if (!isChosen.loadFromFile("Image/untickedboxEE.png"))
			return;
		isChosen.setSmooth(1);
		tickEEButton.sprite.setTexture(isChosen);
		tickEEButton.draw(window);
	}
	if (ev)
	{
		sf::Texture isChosen;
		if (!isChosen.loadFromFile("Image/tickedboxEV.png"))
			return;
		isChosen.setSmooth(1);
		tickEVButton.sprite.setTexture(isChosen);
		tickEVButton.draw(window);
	}
	else
	{
		sf::Texture isChosen;
		if (!isChosen.loadFromFile("Image/untickedboxEV.png"))
			return;
		isChosen.setSmooth(1);
		tickEVButton.sprite.setTexture(isChosen);
		tickEVButton.draw(window);
	}
	if (ve)
	{
		sf::Texture isChosen;
		if (!isChosen.loadFromFile("Image/tickedboxVE.png"))
			return;
		isChosen.setSmooth(1);
		tickVEButton.sprite.setTexture(isChosen);
		tickVEButton.draw(window);
	}
	else
	{
		sf::Texture isChosen;
		if (!isChosen.loadFromFile("Image/untickedboxVE.png"))
			return;
		isChosen.setSmooth(1);
		tickVEButton.sprite.setTexture(isChosen);
		tickVEButton.draw(window);
	}
	backButton.isHover(window, "Image/backHover.png");

	tickEEButton.isHover(window, "Image/tickedboxEE.png");
	tickEVButton.isHover(window, "Image/tickedboxEV.png");
	tickVEButton.isHover(window, "Image/tickedboxVE.png");
	submitResetButton.isHover(window, "Image/submitResetButtonHover.png");
}



void edit() {
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) window.close();

		if (backButton.isClicked(window, event))
		{
			page.pop();
			inputEditedDef.reset();
		}
		inputEditedDef.isClicked(window,event);
		if (submitEdit.isClicked(window, event)) {
			wstring newDef = inputEditedDef.text.getString();
			removeWEndline(newDef);
			if (editType == 0) {
				
				VE::changeWordDefinition(nodeV,newDef, orderDef);
			}
			else if (editType == 1) {
				EV::changeWordDefinition(nodeE, newDef, orderDef);
			}
			else {
				EE::changeWordDefinition(nodeEE, converter.to_bytes(newDef), orderDef);
			}
			searchFlag = 0;
			translateFlag=0;
			inputEditedDef.reset();
			page.pop();
		}
	}
	if (editType == 0) {//Viet - Eng

	}
	else if (editType == 1) { //Eng - Viet
	}
	else { //Eng - Eng

	}
	
	submitEdit.draw(window);
	submitEdit.isHover(window, "Image/submitResetButtonHover.png");

	editedWord.draw(window);
	inputEditedDef.draw(window);

	backButton.draw(window);
	backButton.isHover(window, "Image/backHover.png");
}

ChoiceButton wordsTag(150, 170, "Image/wordTag.png");
ChoiceButton defTag(800, 170, "Image/Deftag.png");
Button shuffleButton(600, 580, "Image/randomButton.png");
int randType = 0;
EVTrie* evNode;
VTrie* veNode;
EETrie* eeNode;
string resWord;
wstring resWword;
int orderRanDef = 0;
bool isShuffle = 0;

void randomWords() {
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) window.close();
		if (backButton.isClicked(window, event))
		{
			page.pop();
			isShuffle = 0;
		}
		if (randType == 0) {//Eng to Vn
			if (ENtoVnButton.isClicked(window, event)) {
				randType = 1;
				isShuffle = 0;
			}
			if (shuffleButton.isClicked(window, event)) {
				evNode = nullptr;
				orderRanDef = 0;
				isShuffle = 1;
				EV::randomAWordNode(rootEtoV, resWord, evNode);
				addToHistory(converter.from_bytes(resWord), evNode->definition[0], "Dataset/History.txt");
			}
			if (isShuffle == 1) {
				if (nextMeanButton.isClicked(window, event) && orderRanDef < evNode->definition.size() - 1) orderRanDef++;
				if (backMeanButton.isClicked(window, event) && orderRanDef > 0) orderRanDef--;
			
			}
			if (heartButton.isClicked(window, event)) {
				if (evNode != nullptr) {
					evNode->isLiked = 1 - evNode->isLiked;
					if (evNode->isLiked == 1) {
						favWordsEV.push_back(resWord);
						favDefsEV.push_back(evNode->definition[0]);
					}
					else {
						EV::unLikeAWord(favWordsEV, favDefsEV, resWord, evNode->definition[0]);
					}
				}
			}
			if (deleteButton.isClicked(window, event)) {
				EV::deleteAWord(rootEtoV, resWord);
				isShuffle = 0;
			}
		}
		else if (randType == 1) {
			if (VNtoEnButton.isClicked(window, event)) {
				randType = 2;
				isShuffle = 0;
			}
			if (shuffleButton.isClicked(window, event)) {
				veNode = nullptr;
				orderRanDef = 0;
				isShuffle = 1;
				VE::randomAWordNode(rootVtoE, resWword, veNode);
				addToHistory(resWword, veNode->definition[0], "Dataset/History.txt");

			}
			if (isShuffle == 1) {
				if (nextMeanButton.isClicked(window, event) && orderRanDef < veNode->definition.size() - 1) orderRanDef++;
				if (backMeanButton.isClicked(window, event) && orderRanDef > 0) orderRanDef--;
			

			}
			if (heartButton.isClicked(window, event)) {
				if (veNode != nullptr) {
					veNode->isLiked = 1 - veNode->isLiked;
					if (veNode->isLiked == 1) {
						favWordsVE.push_back(resWword);
						favDefsVE.push_back(veNode->definition[0]);
					}
					else {
						VE::unLikeAWord(favWordsVE, favDefsEV, resWword, veNode->definition[0]);
					}
				}
			}
			if (deleteButton.isClicked(window, event)) {
				VE::deleteAWord(rootVtoE, resWword);
				isShuffle = 0;
			}
		} 
		else {
			if (ENtoENButton.isClicked(window, event)) {
				randType = 0;
				isShuffle = 0;
			}
			if (shuffleButton.isClicked(window, event)) {
				eeNode = nullptr;
				orderRanDef = 0;
				isShuffle = 1;
				EE::randomAWordNode(rootEtoE, resWord, eeNode);
				addToHistory(converter.from_bytes(resWord), converter.from_bytes(eeNode->definition[0]), "Dataset/History.txt");

			}
			if (isShuffle == 1) {
				if (nextMeanButton.isClicked(window, event) && orderRanDef < eeNode->definition.size() - 1) orderRanDef++;
				if (backMeanButton.isClicked(window, event) && orderRanDef > 0) orderRanDef--;
			}

			if (heartButton.isClicked(window, event)) {
				if (eeNode != nullptr) {
					eeNode->isLiked = 1 - eeNode->isLiked;
					if (eeNode->isLiked == 1) {
						favWordsEE.push_back(resWord);
						favDefsEE.push_back(eeNode->definition[0]);
					}
					else {
						EE::unLikeAWord(favWordsEE, favDefsEE, resWord, eeNode->definition[0]);
					}
				}
			}
			if (deleteButton.isClicked(window, event)) {
				EE::deleteAWord(rootEtoE, resWord);
				isShuffle = 0;
			}
		}

	}
	wordsTag.draw(window);
	defTag.draw(window);

	shuffleButton.draw(window);
	shuffleButton.isHover(window, "Image/randomHover.png");

	heartButton.draw(window);
	deleteButton.draw(window);
	deleteButton.isHover(window, "Image/deleteHover.png");

	backButton.draw(window);
	backButton.isHover(window, "Image/backHover.png");

	if (randType == 0) {
		ENtoVnButton.draw(window);
		ENtoVnButton.isHover(window, "Image/ENtoVNHover.png");
		if (isShuffle == 1) {
			displayDef(200, 300, resWord, 30);
			displayWDef(900, 300, evNode->definition[orderRanDef], 30);

			if (orderRanDef > 0) {
				backMeanButton.draw(window);
				backMeanButton.isHover(window, "Image/backDefHover.png");
			}
			if (orderRanDef < evNode->definition.size() - 1) {
				nextMeanButton.draw(window);
				nextMeanButton.isHover(window, "Image/nextDefHover.png");
			}
		}
		if (evNode != nullptr && evNode->isLiked == 0) {
			heartButton.texture.loadFromFile("Image/heart.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
		else if (evNode != nullptr && evNode->isLiked == 1) {
			heartButton.texture.loadFromFile("Image/heartHover.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
	}
	else if (randType == 1) {
		VNtoEnButton.draw(window);
		VNtoEnButton.isHover(window, "Image/VNtoENHover.png");
		if (isShuffle == 1) {
			displayWDef(200, 300, resWword, 30);
			displayWDef(900, 300, veNode->definition[orderRanDef], 30);

			if (orderRanDef > 0) {
				backMeanButton.draw(window);
				backMeanButton.isHover(window, "Image/backDefHover.png");
			}
			if (orderRanDef < veNode->definition.size() - 1) {
				nextMeanButton.draw(window);
				nextMeanButton.isHover(window, "Image/nextDefHover.png");
			}
		}
		if (veNode != nullptr && veNode->isLiked == 0) {
			heartButton.texture.loadFromFile("Image/heart.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
		else if (veNode != nullptr && veNode->isLiked == 1) {
			heartButton.texture.loadFromFile("Image/heartHover.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
	}
	else {
		ENtoENButton.draw(window);
		ENtoENButton.isHover(window, "Image/ENtoENHover.png");
		if (isShuffle == 1) {
			displayDef(200, 300, resWord, 30);
			displayDef(900, 300, eeNode->definition[orderRanDef], 30);

			if (orderRanDef > 0) {
				backMeanButton.draw(window);
				backMeanButton.isHover(window, "Image/backDefHover.png");
			}
			if (orderRanDef < eeNode->definition.size() - 1) {
				nextMeanButton.draw(window);
				nextMeanButton.isHover(window, "Image/nextDefHover.png");
			}
		}
		if (eeNode != nullptr && eeNode->isLiked == 0) {
			heartButton.texture.loadFromFile("Image/heart.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
		else if (eeNode != nullptr && eeNode->isLiked == 1) {
			heartButton.texture.loadFromFile("Image/heartHover.png");
			heartButton.sprite.setTexture(heartButton.texture);
		}
	}
	
	
}

bool isExtended = 0;

void homePage() {
	orderDef = 0;
	translateFlag = 0;
	searchFlag = 0;
	transType = 0;
	addingType = 0;

	emojiType = 0;

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

	extendButton.draw(window);

	historyButton.draw(window);
	isLikedButton.draw(window);
	resetButton.draw(window);

	if (isExtended == 1) {

		extendLayout.draw(window);

		emojiSearchButton.draw(window);
		qnaButton.draw(window);
		randomWordButton.draw(window);

		//extendButton.isHover(window, "Image/collapseButtonHover.png");
		
		randomWordButton.isHover(window, "Image/randomHover.png");
		qnaButton.isHover(window, "Image/qnaHover.png");
		emojiSearchButton.isHover(window, "Image/emojiHover.png");
	}
	else {
		//extendButton.isHover(window, "Image/extendButtonHover.png");
	}
	

	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) window.close();

		if (searchButton.isClicked(window, event)) {
			page.push(1);
			while (!searchingType.empty()) {
				searchingType.pop();
			}
			searchingType.push(0);

			
		}
		if (translatingButton.isClicked(window, event)) {
			page.push(2);
			
		}
		if (addNewWordButton.isClicked(window, event)) {
			page.push(3);
		}
	
		if (historyButton.isClicked(window, event)) {
			page.push(5);
		}
		if (isLikedButton.isClicked(window, event)) {
			page.push(6);
		}

		if (resetButton.isClicked(window, event)) {
			page.push(8);
		}

		if (isExtended == 0) {
			if (extendButton.isClicked(window, event)) {
				isExtended = 1;
				extendButton.texture.loadFromFile("Image/collapseButton.png");
				extendButton.sprite.setTexture(extendButton.texture);
			}
			
		}
		else {
			if (extendButton.isClicked(window, event)) {
				isExtended = 0;
				extendButton.texture.loadFromFile("Image/extendButton.png");
				extendButton.sprite.setTexture(extendButton.texture);
			}

			if (qnaButton.isClicked(window, event)) {
				page.push(4);
			}
			if (emojiSearchButton.isClicked(window, event)) {
				page.push(7);
			}
			if (randomWordButton.isClicked(window, event)) {
				page.push(10);
			}
		}
		

	}
	addNewWordButton.isHover(window, "Image/addnewwordHover.png");
	searchButton.isHover(window, "Image/searchHover.png");
	translatingButton.isHover(window, "Image/translateHover.png");
	

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
		loading.setFillColor(sf::Color::Black);
		loading.setString("0%");
		window.draw(loading);

		sf::RectangleShape trainingProgressIndicator;
		trainingProgressIndicator.setSize(sf::Vector2f(0, 20));
		trainingProgressIndicator.setPosition(700, 300);
		trainingProgressIndicator.setFillColor(sf::Color::Black);
		window.draw(trainingProgressIndicator);	

		window.display();


		auto start = chrono::high_resolution_clock::now();

		//use threads to load data faster
		thread t1, t2, t3;
		thread t4(Def::loadDataset, std::ref(table), "Dataset/englishDictionary.csv");
		wifstream file;
		file.open("Dataset/UserTrieENVN.bin");
		// if file is open, load trie from file; if not, load raw data
		if (file.is_open()) {
			file.close();
			t1 = thread(EV::loadTriefromFile, std::ref(rootEtoV), "Dataset/UserTrieENVN.bin");
			window.clear();
			setBackground();
			trainingProgressIndicator.setSize(sf::Vector2f(10, 20));
			window.draw(trainingProgressIndicator);
			loading.setString("10%");
			window.draw(loading);
			window.display();

		}
		else {
			t1 = thread(EV::loadRawData, std::ref(rootEtoV), "Dataset/ENVN.txt");
			window.clear();
			setBackground();
			trainingProgressIndicator.setSize(sf::Vector2f(10, 20));
			window.draw(trainingProgressIndicator);
			loading.setString("10%");
			window.draw(loading);
			window.display();
		}
		file.open("Dataset/UserTrieVNEN.bin");
		if (file.is_open()) {
			file.close();
			t2 = thread(VE::loadTrieFromFile, std::ref(rootVtoE), "Dataset/UserTrieVNEN.bin");
			window.clear();
			setBackground();
			trainingProgressIndicator.setSize(sf::Vector2f(20, 20));
			window.draw(trainingProgressIndicator);
			loading.setString("20%");
			window.draw(loading);
			window.display();
		}
		else {
			t2 = thread(VE::loadRawData, std::ref(rootVtoE), "Dataset/VE.csv");
			window.clear();
			setBackground();
			trainingProgressIndicator.setSize(sf::Vector2f(20, 20));
			window.draw(trainingProgressIndicator);
			loading.setString("20%");
			window.draw(loading);
			window.display();
		}
		ifstream file2;
		file2.open("Dataset/UserTrieEN.bin");
		if (file2.is_open()) {
			file2.close();
			t3 = thread(EE::loadTrieFromFile, std::ref(rootEtoE), "Dataset/UserTrieEN.bin");
			window.clear();
			setBackground();
			trainingProgressIndicator.setSize(sf::Vector2f(30, 20));
			window.draw(trainingProgressIndicator);
			loading.setString("30%");
			window.draw(loading);
			window.display();
		}
		else {
			t3 = thread(EE::loadRawData, std::ref(rootEtoE), "Dataset/englishDictionary.csv");
			window.clear();
			setBackground();
			trainingProgressIndicator.setSize(sf::Vector2f(30, 20));
			window.draw(trainingProgressIndicator);
			loading.setString("30%");
			window.draw(loading);
			window.display();
		}

		// Modify Emoji::loadDataset to accept a reference
		thread t5([](Emo& ht, const string& filename, size_t tableSize) {
			ht = Emoji::loadDataset(filename, tableSize);
			}, std::ref(emojiTable), "Dataset\\Emoji_Filter.csv", 101);
		thread t6(loadSearchHistory, std::ref(searchHistory), std::ref(searchRealTime), "Dataset/History.txt");
		window.clear();
		setBackground();
		trainingProgressIndicator.setSize(sf::Vector2f(50, 20));
		window.draw(trainingProgressIndicator);
		loading.setString("50%");
		window.draw(loading);
		window.display();

	
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		// after loading root, load favorite words
		thread t7(EV::loadFavWord, std::ref(rootEtoV), std::ref(favWordsEV), std::ref(favDefsEV), "Dataset/favWordsEV.txt");
		window.clear();
		setBackground();
		trainingProgressIndicator.setSize(sf::Vector2f(60, 20));
		window.draw(trainingProgressIndicator);
		loading.setString("60%");
		window.draw(loading);
		window.display();
		thread t8(VE::loadFavWord, std::ref(rootVtoE), std::ref(favWordsVE), std::ref(favDefsVE), "Dataset/favWordsVE.txt");
		window.clear();
		setBackground();
		trainingProgressIndicator.setSize(sf::Vector2f(70, 20));
		window.draw(trainingProgressIndicator);
		loading.setString("70%");
		window.draw(loading);
		window.display();
		thread t9(EE::loadFavWord, std::ref(rootEtoE), std::ref(favWordsEE), std::ref(favDefsEE), "Dataset/favWordsEE.txt");
		window.clear();
		setBackground();
		trainingProgressIndicator.setSize(sf::Vector2f(100, 20));
		window.draw(trainingProgressIndicator);
		loading.setString("100%");
		window.draw(loading);
		window.display();
		t7.join();
		t8.join();
		t9.join();

		auto end = chrono::high_resolution_clock::now();
		wcout << L"Time to load data: " << chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000 << L"ms" << endl;
	

		return 1;
	}


