/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#include <iostream> // ostream
#include <string> // to_string
#include <stdexcept> // throw, out_of_range, invalid_argument

#include "Hand.h" // inherits vector: back, size

// constructor
Hand::Hand()
	: score{ 0 },
	aceFlag{ false },
	hand{ }
{
}

// return score
unsigned int Hand::getScore() const
{
	return score;
}

// return aceFlag
bool Hand::getAceFlag() const
{
	return aceFlag;
}

// return card from hand
unsigned int Hand::getCard(size_t c) const
{
	if (c < 0 || c >= hand.size())
		throw std::out_of_range("invalid card index");
	else
		return hand[c];
}

// add card to hand by value and update score
void Hand::addCard(unsigned int c)
{
	// validate card value
	if (c < 1 || c > 13)
		throw std::invalid_argument("card value must be between 1 and 13");
	hand.push_back(c);
	updateScore();
}

// remove and return last card in hand
unsigned int Hand::removeCard()
{
	// temporary last card value
	unsigned int temp{ hand.back() };
	// remove last card from hand
	hand.pop_back();
	// return saved card value
	return temp;
}

// updates score, for use after hand has changed
void Hand::updateScore()
{
	// reset score and aceFlag
	aceFlag = false;
	score = 0;
	// loop through all of hand
	for (auto c : hand)
	{
		if (c >= 10)
			score += 10;
		else
			score += c;
		// update aceFlag
		if (c == 1)
			aceFlag = true;
	}
}

// return score string with multi Ace values
// eg if there is an Ace and total doesn't bust, "7 or 17"
std::string Hand::handScore() const
{
	// handle multi Ace values and Blackjack condition
	if (aceFlag && score <= 11)
	{
		if (score == 11 && hand.size() == 2)
			return "Blackjack!";
		return std::to_string(score) + " or " + std::to_string(score + 10);
	}
	else
		return std::to_string(score);
}

// return hand string with first card hidden
std::string Hand::dealerHand() const
{
	std::string output;
	output += "( * ";
	for (size_t c{ 1 }; c < hand.size(); c++)
	{
		if (c > 1)
			output += " ";
		switch (hand[c])
		{
			case 1: output += "A"; break;
			case 11: output += "J"; break;
			case 12: output += "Q"; break;
			case 13: output += "K"; break;
			default: output += std::to_string(hand[c]);
		}
	}
	output += " )";
	return output;
}

// overload output
std::ostream &operator<<(std::ostream &out, const Hand &h)
{
	out << "( ";
	for (size_t c{ 0 }; c < h.hand.size(); c++)
	{
		if (c > 0)
			out << " ";
		switch (h.hand[c])
		{
			case 1: out << "A"; break;
			case 11: out << "J"; break;
			case 12: out << "Q"; break;
			case 13: out << "K"; break;
			default: out << h.hand[c];
		}
	}
	out << " )";
	return out;
}