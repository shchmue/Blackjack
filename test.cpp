/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#include <iostream> // cout, cin
#include <limits> // numeric_limits, max

#include "Blackjack.h"

// test program for blackjack game
int main()
{
	char choice;

	// prompt to see a demo before regular play
	std::cout << "\n > Would you like to see a 5-player demo game before playing? [y/n]: ";
	std::cin >> choice;
	// input validation loop
	while (std::cin.fail() || (choice != 'y' && choice != 'n' && choice != 'Y' && choice != 'N'))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << " >> Please enter y or n: ";
		std::cin >> choice;
	}
	// either run a demo or proceed to interactive game
	if (choice == 'y' || choice == 'Y')
	{
		Blackjack demoGame;
		demoGame.play(true);
	}
	
	// play interactive game regardless
	Blackjack game;
	game.play();

	return 0;
}