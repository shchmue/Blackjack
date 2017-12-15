/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#ifndef HAND_H
#define HAND_H

#include <string> // string
#include <vector> // vector

// Hand class represents a hand of cards
class Hand
{
	// overload output
	friend std::ostream &operator<<(std::ostream &, const Hand &);

public:
	Hand(); // constructor
	~Hand() { } // destructor

	// get functions
	unsigned int getScore() const; // return score
	bool getAceFlag() const; // return aceFlag
	unsigned int getCard(size_t) const; // return card from hand

	// card manipulation functions
	void addCard(unsigned int); // add card to hand by value and update score
	unsigned int removeCard(); // remove and return last card in hand

	// specialized member function
	void updateScore(); // corrects score for use after hand has changed

	// string representation functions
	std::string handScore() const; // return score string with multi Ace values
	std::string dealerHand() const; // return hand string with first card hidden

private:
	// member variables
	unsigned int score; // score of the Hand
	bool aceFlag; // whether hand contains an Ace
	std::vector<unsigned int> hand; // vector of card values
};

#endif // !HAND_H