#pragma once
#include <utility>
#include <vector>
#include <unordered_map>
#include "BoardIterator.h" 
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "math.h"
#include <iostream>
//TO DO LIST
//Use your own data structure??
class KeyTask {
public:
	virtual void run();
};

class Game {
public:
	void createGame(int width, int height, int size);
	void mainLoop();
	void setColor(int tileId, sf::Color color);
	void addMouseButton(int button, int tileId);

private:
	int rows;
	int cols;
	int** board;
	int** bufferBoard;
	int tickLength;
	bool paused = false;
	int tileSize = 1; 
	std::unordered_map<int, sf::Color> colorMap;
	std::unordered_map<int, int> clickMap;
	std::unordered_map<int, KeyTask> keyMap;
	sf::RenderWindow *window;
	sf::Clock clock;
	BoardIterator iter;
	sf::RectangleShape gridLineX;
	sf::RectangleShape gridLineY;
	sf::RectangleShape tile;
	std::vector<std::pair<int, int>>* changeList;
	std::vector<sf::Vector3f> changeL;
	sf::RenderTexture *renTex = new sf::RenderTexture();

	void fillBoard(int tileId);
	void setTile(int width, int height, int tileId);
	int getTile(int width, int height);
	void drawGame();
	void loadBuffer();
	void initializeShapes();
	void updateChangeList();
	void emptyChangelist();
	void setTileSize(int);
	void clearGame();
};


void Game::createGame(int width, int height, int size) {
	window = new sf::RenderWindow(sf::VideoMode(width,height),"Title");
	renTex->create(width, height);
	setTileSize(size);//MARKED
	rows = 1920/tileSize, cols = 1080/tileSize;
	//setTileSize((width/rows)); //Alternate way of setting size(in terms of pixel width)
	tickLength = 20;
	initializeShapes();
	board = new int* [rows], iter.board = new int* [rows];
	bufferBoard = new int* [rows], iter.bufferBoard = new int* [rows];
	changeList = new std::vector<std::pair<int, int>>[rows];
	for (int i = 0; i < rows; i++){
		board[i] = new int[cols]; bufferBoard[i] = new int[cols];
		iter.board[i] = board[i];
		iter.bufferBoard[i] = bufferBoard[i];
	}
	fillBoard(0);
	iter.rows = rows, iter.cols = cols;
	window->setSize(sf::Vector2u(width, height));
	window->setTitle("Nicco Klein - Pixel Project");
	window->setFramerateLimit(1000/tickLength);//When the number is 10000, microseconds are being used
}

void Game::mainLoop() {
	while (window->isOpen()) {
		window->clear(sf::Color::White);
		sf::Event event;
		while (window->pollEvent(event)) {
			//INPUT CHECKS
			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == sf::Event::KeyReleased) {
				sf::Keyboard::Key key = event.key.code;
//				if(event.key.code == sf::Keyboard::Key::C)
//					clearGame();
				if(key == sf::Keyboard::Key::C)
					clearGame();
				else
					paused = !paused;
			}

			if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::getPosition(*window).y>5) {
				bufferBoard[(int)floor((sf::Mouse::getPosition(*window).x / tileSize))][(int)floor((sf::Mouse::getPosition(*window).y / tileSize))] = clickMap[event.mouseButton.button];
				loadBuffer();
				drawGame();
        std::cout<<"\nClick happened";//MARKED
			}


		}
		if (clock.getElapsedTime() >= sf::milliseconds(tickLength) && paused == false) {
			changeL.clear();
			for (int i = 0; i < 1; i++) {
				iter.start();
				loadBuffer();
				//updateChangeList();
			}
				clock.restart();
			
		}
		drawGame();
		renTex->display();
		window->draw(sf::Sprite(renTex->getTexture()));
		window->display();
	}
};

void Game::fillBoard(int tileId) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			setTile(i, j, tileId);
			bufferBoard[i][j] = tileId;
		}
	}
}

void Game::clearGame(){
		changeL.clear();
		iter.start();
		fillBoard(0);
		renTex->clear(sf::Color::White);
		loadBuffer();
		drawGame();
}

void Game::setTile(int x, int y, int tileId) {
	board[x][y] = tileId;
}

int Game::getTile(int x, int y) {
	return board[x][y];
}

void Game::drawGame() {
	//Below code is a deprecated method of drawing the tiles to the screen. It is an improvement over drawing every tile every frame but not as fast as the new method
	/*for (int j = 0; j < cols; j++) {
		int count = 1;
		for (int i = 0; i < changeList[j].size(); i++) {
			count += changeList[j][i].second;
			tile.setFillColor(colorMap[changeList[j][i].first]);
			tile.setSize(sf::Vector2f(changeList[j][i].second * tileSize, tileSize));
			tile.setPosition(sf::Vector2f((count * tileSize) - (tileSize*2) - ((changeList[j][i].second - 1) * tileSize), (j * tileSize) + 50));
			renTex->draw(tile);
		}
	}*/
	tile.setSize(sf::Vector2f(tileSize,tileSize));
	for (int i = 0; i < changeL.size(); i++) {
		tile.setFillColor(colorMap[changeL[i].z]);
		tile.setPosition(sf::Vector2f((changeL[i].x * tileSize), (changeL[i].y * tileSize)));
		renTex->draw(tile);
	}
	//Below code can be used to draw gridlines(make this static later if you ever use it)
	//for (int i = 0; i < rows; i++) {
	//	for (int j = 0; j < cols-5; j++) {
	//		tile.setFillColor(colorMap[getTile(i,j)]);
	//		tile.setPosition(sf::Vector2f(i*10, (j*10)+50));
	//		window->draw(tile);
	//	}
//		gridLineX.setPosition(sf::Vector2f(i * 10, 50));
	//	window->draw(gridLineX);
//	}
	//for (int i = 0; i < cols; i++) {
	//	gridLineY.setPosition(sf::Vector2f(0, (i*10)+50));
	//	window->draw(gridLineY);
	//}


}

void Game::setColor(int tileId, sf::Color color) {
	colorMap[tileId] = color;
}

void Game::addMouseButton(int button, int tileId) {
	clickMap[button] = tileId;
}

void Game::loadBuffer() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (bufferBoard[i][j] != board[i][j]) {
				changeL.push_back( sf::Vector3f(i, j,bufferBoard[i][j]));
	 		}
			setTile(i, j, bufferBoard[i][j]);
		}
	}
}



void Game::initializeShapes()
{
	gridLineX = sf::RectangleShape(sf::Vector2f(tileSize, cols* tileSize));
	gridLineY = sf::RectangleShape(sf::Vector2f(rows* tileSize, tileSize));
	tile = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
  tile = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
	gridLineX.setFillColor(sf::Color::Transparent);
	gridLineX.setOutlineColor(sf::Color::Black);
	gridLineX.setOutlineThickness(.5);
	gridLineY.setFillColor(sf::Color::Transparent);
	gridLineY.setOutlineColor(sf::Color::Black);
	gridLineY.setOutlineThickness(.5);
	tile.setFillColor(sf::Color::White);
}

void Game::updateChangeList() {
	int count = 1;
	int color = 0;
	for (int j = 0; j < cols; j++) {
		for (int i = 0; i < rows - 1; i++) {

			if (bufferBoard[i + 1][j] == bufferBoard[i][j]) {
				count++;
			}
			else {
				color = bufferBoard[i][j];
				changeList[j].push_back(std::make_pair(color, count));
				count = 1;
			}

		}
		if (bufferBoard[rows - 1][j] == bufferBoard[rows - 2][j]) {
			count++;
			color = bufferBoard[rows - 1][j];
		}
		else {
			changeList[j].push_back(std::make_pair(bufferBoard[rows - 1][j], 1));
		}
		count = 1;
	}
}

void Game::emptyChangelist() {
	for (int j = 0; j < cols; j++) {
		changeList[j].clear();
	}
}

void Game::setTileSize(int size) {
	tileSize = size;
}
