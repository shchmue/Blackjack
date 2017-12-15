/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#include <algorithm> // shuffle
#include <random> // default_random_engine
#include <chrono> // chrono::system_clock, now, time_since_epoch, count
#include <iostream> // cout
#include <iterator> // begin, end

#include "Deck.h"

// constructor
Deck::Deck()
	: deck{
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
	}, // deck contains 52 cards with 13 ranks, suits not important, 1=A 11=J 12=Q 13=K
	currentCard{ 0 },
	// set seed to current time
	seed{ static_cast<unsigned int>(
		std::chrono::system_clock::now().time_since_epoch().count()) }
{
}

// shuffle deck (randomize order)
void Deck::shuffleDeck()
{
	// shuffle deck order using seed
	std::shuffle(std::begin(deck), std::end(deck), std::default_random_engine(seed));
	// reset current card to beginning of deck
	currentCard = 0;
	std::cout << "\nDeck shuffled.\n";
}

// return next card in deck
int Deck::drawCard()
{
	// shuffle if deck exhausted or begun
	if (currentCard % deckSize == 0)
		shuffleDeck();
	// return currentCard and increment counter
	// currentCard can be incremented indefinitely, index modulo deckSize
	return deck[currentCard++ % deckSize];
}