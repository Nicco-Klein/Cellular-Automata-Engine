#pragma once
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
class BoardIterator {
public:
	void start();
	int checkUp();
	int checkDown();
	int checkLeft();
	int checkRight();
	int checkUpLeft();
	int checkUpRight();
	int checkDownLeft();
	int checkDownRight();
	int checkBoard(int, int);
	int checkBoard();
	int surroundCount();
	int adjacentCount(int color);
	int surroundId();
	void setTile(int tileId);
	void run();
	void updateChangeList();
	int** board;
	int** bufferBoard;
	BoardIterator();
	int rows, cols;
	bool notEdge;
private:
	int xValue, yValue;
};

void BoardIterator::start() {
	xValue = 0, yValue = 0;
	for (xValue = 0; xValue < rows; xValue++) {
		for (yValue = 0; yValue < cols; yValue++) {
			run();
		}
	}
}

void BoardIterator::run() {
	try {
		notEdge = ((yValue > 0 && yValue < cols - 1) && (xValue > 0 && xValue < rows - 1));
		if(!notEdge)
			setTile(-1);
		
		if (checkBoard() == 0 && adjacentCount(1)>0 && ((float)rand() / RAND_MAX)<(.04 * surroundCount())) {
			setTile(1);
			return;
		}else
		if(checkBoard()==0&&adjacentCount(2)>0&&((float)rand()/RAND_MAX)<(.04*surroundCount())) {
			setTile(2);
			return;
		}else
		if(checkBoard()==0&&adjacentCount(3)>0&&((float)rand()/RAND_MAX)<(.04*surroundCount())) {
			setTile(3);
			return;
		}else
		//Green when touching red
		if(checkBoard()==2&&adjacentCount(1)>1&&((float)rand()/RAND_MAX)<(.03*adjacentCount(1))) {
			setTile(1);
			return;
		}else
		//Green when touching yellow
		if(checkBoard()==2&&adjacentCount(3)>1&&((float)rand()/RAND_MAX)<(.01*adjacentCount(3))) {
			setTile(3);
			return;
		}else
		//Red when touching Green
		if(checkBoard()==1&&adjacentCount(2)>1&&((float)rand()/RAND_MAX)<(.01*adjacentCount(2))) {
			setTile(2);
			return;
		}else
		//REd when touching yellow
		if(checkBoard()==1&&adjacentCount(3)>1&&((float)rand()/RAND_MAX)<(.03*adjacentCount(3))) {
			setTile(3);
			return;
		}else
		//Yellow when touching green
		if(checkBoard()==3&&adjacentCount(2)>1&&((float)rand()/RAND_MAX)<(.03*adjacentCount(2))) {
			setTile(2); 
			return;
		}else
		//Yellow when touching red
		if(checkBoard()==3&&adjacentCount(1)>1&&((float)rand()/RAND_MAX)<(.01*adjacentCount(1))) {
			setTile(1);
			return;
		}

		/*if((surroundCount()==2||surroundCount()==3)){
				if(checkBoard()==0 && surroundCount()==3)
					setTile(1);
		}else
			setTile(0);
		*/	
		//if (checkBoard() != 0 && adjacentCount(checkBoard()) < 3 && adjacentCount(checkBoard())>1 && adjacentCount(0)<1 && (float)rand() / RAND_MAX<.02) {
	//		setTile(4);
	//		return;
		//}

	
	}
	catch (const std::exception &ex) { }
}


int BoardIterator::checkUp()
{
	if (notEdge)
		return board[xValue][yValue - 1];
	else
		return -1;
}

int BoardIterator::checkDown()
{
	if (notEdge)
		return board[xValue][yValue + 1];
	else
		return -1;
}

int BoardIterator::checkLeft()
{
	if (notEdge)
		return board[xValue - 1][yValue];
	else
		return -1;
}
int BoardIterator::checkRight()
{
	if (notEdge)
		return board[xValue + 1][yValue];
	else
		return -1;
}

int BoardIterator::checkUpLeft()
{
	if (notEdge)
		return board[xValue - 1][yValue - 1];
	else
		return -1;
}

int BoardIterator::checkUpRight()
{
	if (notEdge)
		return board[xValue + 1][yValue - 1];
	else
		return -1;
}

int BoardIterator::checkDownLeft()
{
	if (notEdge)
		return board[xValue - 1][yValue + 1];
	else
		return -1;
}

int BoardIterator::checkDownRight()
{
	if (notEdge)
		return board[xValue + 1][yValue + 1];
	else
		return -1;
}

int BoardIterator::checkBoard(int x, int y) {
	if (x > 0 && y < cols -1)
		return board[x][y];
	else
		return -1;
}

int BoardIterator::checkBoard() {
	return board[xValue][yValue];
}

void BoardIterator::setTile(int tileId) {
	bufferBoard[xValue][yValue] = tileId;
}

int BoardIterator::surroundCount() {
	int count = 0;
	int array[] = { checkUp(),checkDown(),checkLeft(),checkRight(),checkUpLeft(),checkUpRight(),checkDownLeft(),checkDownRight() };
	for (int i = 0; i < 8; i++) {
		if (array[i] > 0) {
			count++;
		}
	}
	return count;
}

int BoardIterator::adjacentCount(int color) {
	int count = 0;
	int array[] = { checkUp(),checkDown(),checkLeft(),checkRight(),checkUpLeft(),checkUpRight(),checkDownLeft(),checkDownRight() };
	for (int i = 0; i < 8; i++) {
		if (array[i] == color) {
			count++;
		}
	}
	return count;
}


BoardIterator::BoardIterator() {
	srand((unsigned)time(NULL));

}
