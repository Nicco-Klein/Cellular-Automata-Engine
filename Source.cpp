#include <iostream>
#include "Game.h"

int main()
{   
   Game game;
	int tileSize = 10;
	std::cout<<"Enter tile size from following values ";
	std::cout<<"\n1,2,3,4,5,6,8,10,12,15,20,24,30,40,60,120";
	std::cout<<"\n>>";
	std::cin>>tileSize;
	game.createGame(1920, 1080, tileSize); 
    game.setColor(0, sf::Color::Black);
	game.setColor(-1, sf::Color::Black);
	game.setColor(1, sf::Color::Red);
	game.setColor(2, sf::Color::Green);
	game.setColor(3, sf::Color::Blue);
  game.addMouseButton(0, 1);
  game.addMouseButton(1, 2);
  game.addMouseButton(2, 3);
  std::cout << sf::Mouse::Right;
  game.mainLoop();
    return 0;
}
