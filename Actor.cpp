/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#include <iostream> // cerr, endl
#include <stdexcept> // throw, out_of_range, invalid_argument, logic_error

#include "Actor.h" // inherits Hand

// constructor
Actor::Actor()
	: handCount{ 1 },
	hand{ Hand() } // start with one empty hand
{
}

// return particular Hand
Hand Actor::getHand(size_t i) const
{
	if (0 <= i && i < handCount)
		return hand[i];
	else
		throw std::out_of_range("invalid hand index");
}

// return handCount
size_t Actor::getHandCount() const
{
	return handCount;
}

// add card value to particular Hand
// passthrough function with bounds checking
void Actor::addCard(size_t h, unsigned int c)
{
	if (h < 0 || h >= getHandCount())
		throw std::out_of_range("invalid hand index");
	if (c < 1 || c > 13)
		throw std::invalid_argument("card value must be between 1 and 13");
	try
	{
		hand[h].addCard(c);
	}
	catch (std::invalid_argument &e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

// split particular Hand
void Actor::split(size_t h)
{
	if (h < 0 || h >= handCount)
		throw std::out_of_range("invalid hand index");
	// make sure hand at least contains 2 cards
	try
	{
		hand[h].getCard(0);
		hand[h].getCard(1);
	}
	catch (std::out_of_range &e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	// make sure 2 cards are of equal value (except values >=10)
	if (hand[h].getCard(0) != hand[h].getCard(1)
		&& hand[h].getCard(0) < 10 && hand[h].getCard(1) < 10)
		throw std::logic_error("invalid split");
	// add a new Hand to hand vector
	hand.push_back(Hand());
	// add second card from specified hand to new hand and inc handCount
	try
	{
		hand[handCount++].addCard(hand[h].removeCard());
	}
	catch (std::invalid_argument &e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	// update hand scores
	hand[h].updateScore();
	hand[handCount - 1].updateScore();
}

// resets member variables to play again with same Actor
void Actor::reset()
{
	hand = { Hand() };
	handCount = 1;
}