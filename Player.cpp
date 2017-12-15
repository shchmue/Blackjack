/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#include <iostream> // cout
#include <stdexcept> // throw, out_of_range, invalid_argument

#include "Player.h" // inherits Actor, Deck, Hand; vector: push_back

// init static player counting variable
size_t Player::playerCount = 0;

// constructor
Player::Player()
	: balance{ 1000.0f },
	bet{ 0 },
	surrender{ false },
	doubleDown{ false }
{
	// increment static count of players
	playerCount++;
}

// destructor
Player::~Player()
{
	// decrement static count of players
	playerCount--;
}

// return balance
float Player::getBalance() const
{
	return balance;
}

// return particular bet
float Player::getBet(size_t h) const
{
	// check bounds
	if (0 <= h && h < Actor::getHandCount())
		return bet[h];
	else
		throw std::out_of_range("invalid bet index");
}

bool Player::getSurrender() const
{
	return surrender;
}

bool Player::getDoubleDown(size_t h) const
{
	// check bounds
	if (0 <= h && h < Actor::getHandCount())
		return doubleDown[h];
	else
		throw std::out_of_range("invalid hand index");
}

// set particular bet
void Player::setBet(size_t h, float b)
{
	// check bounds
	if (h < 0 || h >= Actor::getHandCount())
		throw std::out_of_range("invalid bet index");
	// check bet doesn't exceed balance
	if (b > balance)
		throw std::invalid_argument("not enough money");
	bet[h] = b;
}

void Player::setSurrender()
{
	surrender = true;
}

void Player::setDoubleDown(size_t h)
{
	// check bounds
	if (h < 0 || h >= Actor::getHandCount())
		throw std::out_of_range("invalid hand index");
	else
		doubleDown[h] = true;
}

// add bet to bet vector
void Player::addBet(float b)
{
	// check if number of hands/bets already max
	if (Actor::getHandCount() > maxHands)
		throw std::out_of_range("max hands exceeded");
	// check bet doesn't exceed balance
	if (b > balance)
		throw std::invalid_argument("not enough money");
	bet.push_back(b);
	doubleDown.push_back(false);
}

// pay winnings or collect losses
void Player::payOrCollect(float amount)
{
	if (amount < 0 && -amount > balance)
		throw std::invalid_argument("not enough money");
	balance += amount;
}

// reset member variables to play again with same Player
// leave balance alone
void Player::reset()
{
	bet = { 0 };
	surrender = false;
	doubleDown = { false };
	// call base class reset function to clear other private variables
	Actor::reset();
}

// return playerCount
size_t Player::getPlayerCount()
{
	return playerCount;
}