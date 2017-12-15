/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#ifndef ACTOR_H
#define ACTOR_H

#include <vector> // vector

#include "Hand.h"

// Actor base class represents any card-holder
class Actor
{
public:
	Actor(); // constructor
	~Actor() { } // destructor

	// get functions
	Hand getHand(size_t) const; // return particular Hand
	size_t getHandCount() const; // return handCount

	// hand modification functions
	void addCard(size_t, unsigned int); // add card value to particular Hand
	void split(size_t); // split particular Hand

	// specialized function
	virtual void reset(); // resets member variables to play again with same Actor

private:
	// member variables
	size_t handCount; // number of hands/bets
	std::vector<Hand> hand; // vector of Hands
};

#endif // !ACTOR_H