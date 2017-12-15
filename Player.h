/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <vector> // vector

#include "Actor.h"

// Player class derived from Actor represents Blackjack players
class Player : public Actor
{
public:
	// static const member variable
	static const size_t maxHands = 4; // most hands a player can have

	Player(); // constructor
	~Player(); // destructor

	// get functions
	float getBalance() const; // return balance
	float getBet(size_t) const; // return specified bet
	bool getSurrender() const; // return surrender flag
	bool getDoubleDown(size_t) const; // return specified double down flag

	// set functions
	void setBet(size_t, float); // set bet for specified hand
	void setSurrender(); // set surrender flag
	void setDoubleDown(size_t); // set specified double down flag

	// specialized member functions
	void addBet(float); // add bet to bet vector
	void payOrCollect(float); // pay winnings or collect losses
	virtual void reset() override; // reset member variables to play again with same Player

	// static member function
	static size_t getPlayerCount(); // return playerCount

private:
	// member variables
	float balance; // player's money
	std::vector<float> bet; // player's bets
	bool surrender; // surrender flag
	std::vector<bool> doubleDown; // double down flag for each hand
	
	// static member variable tracking number of Player objects
	static size_t playerCount; // current number of players
};

#endif // !PLAYER_H