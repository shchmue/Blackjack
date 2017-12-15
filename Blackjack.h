/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <random> // default_random_engine, uniform_int_distribution
#include <vector> // vector

#include "Actor.h"
#include "Player.h"
#include "Deck.h"
#include "Hand.h"

class Blackjack
{
public:
	Blackjack(); // constructor
	~Blackjack(); // destructor

	// public member function
	void play(bool = false); // play the game or a non-interactive demo

private:
	// specialized private member functions
	void takeBets(); // collect bets for each Player in players
	void dealCards(); // deal one card to each Player + dealer twice
	void playHands(); // go around the table playing each Hand
	void winsAndLosses(); // pay and collect wins and losses
	char demoDecide(); // return random yes/no decisions for demo mode

	// const member function
	void printTable(bool = false) const; // print the whole table

	// member variables
	Deck gameDeck; // deck of cards for the game
	Actor dealer; // the dealer
	std::vector<Player> players; // vector of players
	bool demo; // demo flag
	unsigned int seed; // seed for random number generator
	std::default_random_engine random_engine;
	std::uniform_int_distribution<unsigned int> random;
};

#endif // !BLACKJACK_H