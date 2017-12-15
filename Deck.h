/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#ifndef DECK_H
#define DECK_H

// Deck class represents a deck of cards
class Deck
{
public:
	// static const member variable
	static const size_t deckSize = 52; // number of cards in a deck

	Deck(); // constructor
	~Deck() { } // destructor

	// specialized member functions
	void shuffleDeck(); // shuffle deck (randomize order)
	int drawCard(); // return next card in deck

private:
	// member variables
	unsigned int deck[deckSize]; // built-in array of card values
	size_t currentCard; // track card index
	unsigned int seed; // seed for random number generator
};

#endif // !DECK_H