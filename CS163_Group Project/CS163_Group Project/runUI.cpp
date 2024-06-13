// #include <SFML/Graphics.hpp>
// #include "UI.h"
// #include <iostream>

// sf::RenderWindow window(sf::VideoMode(1500, 800), "Dictionary");
// sf::Event event;
// sf::View view = window.getView();
// float scrollSpeed = 50.f;

// Button VNtoEnButton(520, 218, "Image/VNtoENButton.png");
// Button ENtoVnButton(520, 340, "Image/ENtoVNButton.png");
// Button searchButton(520, 452, "Image/searchButton.png");
// Button backButton(20, 18, "Image/backButton.png");


// bool searchingType = 0;
// Button searchKeyButton(98, 138, "Image/searchKey.png");
// Button searchDefButton(98, 237, "Image/searchDef.png");

// InputBox inputVNBox(218, 236, "Image/InputBox.png", "Nhap tai day");
// InputBox inputENBox(218, 236, "Image/InputBox.png", "Text here");

// InputBox searchKeyBox(28, 412, "Image/InputBox.png", "Text here");
// InputDef searchDefBox(28, 349, "Image/InputDef.png", "Text here");

// SubmitButton translate(548, 368, "Image/translateButton.png");
// SubmitButton submitSearchButton(383, 729, "Image/search.png");

// int page = 0;
// void setBackground() {
// 	sf::Texture texture;
// 	if (!texture.loadFromFile("Image/background.png")) {

// 	}
// 	sf::Sprite sprite;
// 	texture.setSmooth(1);
// 	sprite.setTexture(texture);
// 	sprite.setPosition(0, 0);
// 	window.draw(sprite);
// }

// void VNtoEnglish() {
// 	sf::Texture vnmese;
// 	if (!vnmese.loadFromFile("Image/Vntag.png")) {

// 	}
// 	sf::Sprite VNtag;
// 	vnmese.setSmooth(1);
// 	VNtag.setTexture(vnmese);
// 	VNtag.setPosition(189, 63);
// 	window.draw(VNtag);

// 	sf::Texture eng;
// 	if (!eng.loadFromFile("Image/ENtag.png")) {

// 	}
// 	eng.setSmooth(1);
// 	sf::Sprite ENtag;
// 	ENtag.setTexture(eng);
// 	ENtag.setPosition(860, 63);
// 	window.draw(ENtag);

// 	std::string word;

// 	while (window.pollEvent(event))
// 	{
// 		if (backButton.isClicked(window, event)) page = 0;
// 		if (event.type == sf::Event::Closed) window.close();
// 		inputVNBox.isClicked(window, event);
// 		if (translate.isClicked(window, event, word, inputVNBox.text)) std::cout << word;

// 	}
// 	backButton.draw(window);
// 	translate.draw(window);
// 	inputVNBox.draw(window);

// }

// void ENtoVietnames() {
// 	sf::Texture vnmese;
// 	if (!vnmese.loadFromFile("Image/Vntag.png")) {

// 	}
// 	sf::Sprite VNtag;
// 	vnmese.setSmooth(1);
// 	VNtag.setTexture(vnmese);
// 	VNtag.setPosition(860, 63);
// 	window.draw(VNtag);

// 	sf::Texture eng;
// 	if (!eng.loadFromFile("Image/ENtag.png")) {

// 	}
// 	eng.setSmooth(1);
// 	sf::Sprite ENtag;
// 	ENtag.setTexture(eng);
// 	ENtag.setPosition(189, 63);
// 	window.draw(ENtag);

// 	std::string word;

// 	while (window.pollEvent(event))
// 	{
// 		if (backButton.isClicked(window, event)) page = 0;
// 		if (event.type == sf::Event::Closed) window.close();
// 		inputENBox.isClicked(window, event);
// 		if (translate.isClicked(window, event, word, inputENBox.text)) std::cout << word;
// 	}
// 	translate.draw(window);
// 	inputENBox.draw(window);
// 	backButton.draw(window);
// }

// void search() {
// 	sf::Texture layout2;
// 	if (!layout2.loadFromFile("Image/layout2.png")) {

// 	}
// 	layout2.setSmooth(1);
// 	sf::Sprite sprite;
// 	sprite.setTexture(layout2);
// 	sprite.setPosition(0, 0);
// 	window.draw(sprite);
// 	std::string words;
// 	while (window.pollEvent(event)) {
// 		if (event.type == sf::Event::Closed) window.close();
// 		if (searchKeyButton.isClicked(window, event)) searchingType = 0;
// 		if (searchDefButton.isClicked(window, event)) searchingType = 1;
// 		if (backButton.isClicked(window, event)) page = 0;
// 		if (searchingType == 0) {
// 			searchKeyBox.isClicked(window, event);
// 			if (submitSearchButton.isClicked(window, event, words, searchKeyBox.text)) std::cout << words;
// 		}
// 		else {
// 			searchDefBox.isClicked(window, event);
// 			if (submitSearchButton.isClicked(window, event, words, searchDefBox.text)) std::cout << words;
// 		}
// 	}
// 	if (searchingType == 0) {
// 		searchKeyBox.draw(window);
// 	}
// 	else {
// 		searchDefBox.draw(window);
// 	}
// 	submitSearchButton.draw(window);
// 	searchKeyButton.draw(window);
// 	searchDefButton.draw(window);
// 	backButton.draw(window);

// }

// void homePage() {
// 	VNtoEnButton.draw(window);
// 	ENtoVnButton.draw(window);
// 	searchButton.draw(window);
// 	while (window.pollEvent(event)) {

// 		if (event.type == sf::Event::Closed) window.close();

// 		if (VNtoEnButton.isClicked(window, event)) page = 1;
// 		if (ENtoVnButton.isClicked(window, event)) page = 2;
// 		if (searchButton.isClicked(window, event)) page = 3;
// 	}
// }
// int run() {
// 	while (window.isOpen()) {
// 		setBackground();
// 		switch (page)
// 		{
// 		case 0: {
// 			homePage();
// 			break;
// 		}
// 		case 1: {
// 			VNtoEnglish();
// 			break;
// 		}
// 		case 2: {
// 			ENtoVietnames();
// 			break;
// 		}
// 		case 3: {
// 			search();
// 			break;
// 		}
// 		default:
// 			break;
// 		}
// 		window.display();
// 		window.clear(sf::Color::White);
// 	}
// }