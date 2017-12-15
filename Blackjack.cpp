/*
Final Project Fall 2017
Blackjack Game
shchmue
*/

#include <iostream> // cout, cin, fixed
#include <limits> // numeric_limits, max
#include <iomanip> // setprecision
#include <vector> // begin
#include <string> // for string comparison overloads
#include <chrono> // chrono::system_clock, now, time_since_epoch, count


#include "Actor.h" // inherits Hand
#include "Blackjack.h" // inherits random: default_random_engine, uniform_int_distribution
#include "Deck.h"
#include "Player.h"

// constructor
Blackjack::Blackjack()
	: gameDeck{ Deck() },
	dealer{ Actor() },
	players{ },
	demo{ },
	// set seed to current time
	seed{ static_cast<unsigned int>(
		std::chrono::system_clock::now().time_since_epoch().count()) },
	random_engine{ seed }, // seed the random engine
	random(0, 1) // random int distribution of 0s and 1s
{
	// set 2 digit fixed precision for currency display
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "\n\n***** Welcome to Blackjack! *****\n\n";
}

// destructor
Blackjack::~Blackjack()
{
	// announce game ending
	std::cout << "\n\n****** Thanks for playing! ******\n\n";
}

// play the game or a non-interactive demo
// game is interactive by default
void Blackjack::play(bool demoMode)
{
	// set demo flag for object so other member functions know
	demo = demoMode;
	unsigned int numPlayers; // number of players

	std::cout << " > Enter number of players (1 - 5): ";
	if (demo) // 5 players for demo
	{
		numPlayers = 5;
		std::cout << "5\n In this demo, players 1-3 use strategy while 4 and 5 play randomly.\n";
	}
	else
		std::cin >> numPlayers;

	// input validation loop. fail() handles wrong input types
	while (std::cin.fail() || numPlayers < 1 || numPlayers > 5)
	{
		// clear failbit
		std::cin.clear();
		// empty istream buffer
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		// repeat prompt
		std::cout << " >> Please enter a valid number of players between 1 and 5: ";
		std::cin >> numPlayers;
	}
	// resize Player vector to initialize all players
	players.resize(numPlayers);

	// game loop to handle replay
	while (true)
	{
		std::cout << "\n--- Beginning of round ---";
		takeBets();

		std::cout << "\nDealing initial 2 cards:\n";
		dealCards();

		// if dealer draws Blackjack, players needn't play
		if (dealer.getHand(0).handScore() == "Blackjack!")
			std::cout << "\nDealer drew Blackjack. Round has concluded.\n";
		else
		{
			// show initial state of table with first dealer card hidden
			printTable(false);

			// play each player's hands and then the dealer's
			playHands();
		}

		// print table with all hand-play final and all dealer cards revealed
		std::cout << "\n Final table state:\n";
		printTable(true);

		// pay and collect wins and losses
		winsAndLosses();

		// offer to replay, stays interactive even for demo
		char choice;
		// but alter prompt for demo
		if (demo)
			std::cout << "\n > Would you like to run the demo again with the same players? [y/n]: ";
		else
			std::cout << "\n > Would you like to play again with the same players? [y/n]: ";
		std::cin >> choice;

		// input validation loop
		while (std::cin.fail() || (choice != 'y' && choice != 'n' && choice != 'Y' && choice != 'N'))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << " >> Please enter y or n: ";
			std::cin >> choice;
		}
		if (choice == 'y' || choice == 'Y')
		{
			// reset dealer and player variables keeping balances
			dealer.reset();
			for (size_t p{ 0 }; p < Player::getPlayerCount(); p++)
			{
				// disqualify player if not enough money to play again
				if (players[p].getBalance() < 5.0f)
				{
					std::cout << " Player ";
					// only print Player number if more than one Player
					if (players[p].getPlayerCount() > 1)
						std::cout << p + 1 << " ";
					std::cout << "doesn't have enough money and is kicked from the table.\n";
					// if this was the only player, end game
					if (Player::getPlayerCount() == 1)
						return;
					// remove player index p from player vector
					// and dec p index since number of players has changed
					players.erase(players.begin() + p--);
					continue;
				}
				players[p].reset();
			}
		}
		// otherwise exit function
		else
			return;
	}
}

// collect bets for each Player in players
// make sure bets are between $5-100 and <= player balance
void Blackjack::takeBets()
{
	// set 2 digit fixed precision for currency display
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "\n\nPlace bet between $5 and $100:\n";

	for (size_t p{ 0 }; p < Player::getPlayerCount(); p++)
	{
		float bet;
		std::cout << " > Player ";
		// only print Player number if more than one Player
		if (Player::getPlayerCount() > 1)
			std::cout << p + 1 << " ";
		std::cout << "has $" << players[p].getBalance() << ". Enter bet: ";
		if (demo)
		{ // max bet for demo
			if (players[p].getBalance() < 100.0f) // unless player doesn't have enough money
				bet = players[p].getBalance();
			else
				bet = 100.0f;
			std::cout << bet << "\n";
		}
		else
			std::cin >> bet;

		// input validation loop
		while (std::cin.fail() || bet < 5.0f || bet > 100.0f || bet > players[p].getBalance())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << " >> Please enter a valid bet <= current balance and between $5 and $100: ";
			std::cin >> bet;
		}
		players[p].setBet(0, bet);
	}
}

// deal one card to each Player + dealer twice
void Blackjack::dealCards()
{
	for (auto times{ 0 }; times < 2; times++)
	{
		// deal clockwise from dealer with dealer last
		for (size_t p{ 0 }; p < Player::getPlayerCount(); p++)
		{
			players[p].addCard(0, gameDeck.drawCard());
		}
		dealer.addCard(0, gameDeck.drawCard());
	}
}

// go around the table playing each Hand
// assumes dealCards and takeBets have been run
void Blackjack::playHands()
{
	// set 2 digit fixed precision for currency display
	std::cout << std::fixed << std::setprecision(2);

	char choice; // used for menus

	// store face-up dealer card for demo decisions
	unsigned int dealerCard{ dealer.getHand(0).getCard(1) }; 

	// play each Player's Hands
	for (size_t p{ 0 }; p < Player::getPlayerCount(); p++)
	{
		std::cout << "\nPlayer";
		// only print Player number if more than one Player
		if (Player::getPlayerCount() > 1)
			std::cout << " " << p + 1;
		std::cout << "'s turn.\n";
		// Blackjack handling: hand consists of one Ace and one 10/face card------------------
		if (players[p].getHand(0).handScore() == "Blackjack!")
		{
			std::cout << "Congratulations on Blackjack!\n";
			// proceed to next player
			continue;
		}
		// print hand, score and bet
		std::cout << "  Hand: " << players[p].getHand(0)
			<< " | Score: " << players[p].getHand(0).handScore()
			<< " | Bet: $" << players[p].getBet(0) << "\n";

		// surrender handling-----------------------------------------------------------------
		std::cout << " > Would you like to surrender half your bet and fold? [y/n]: ";
		if (demo)
		{
			if (p == 4 || p == 5) // players 4 and 5 play randomly
				choice = demoDecide();
			else // others play with strategy
			{
				if (dealerCard >= 10 && players[p].getHand(0).getScore() == 16)
					choice = 'y';
				else
					choice = 'n';
			}
			std::cout << choice << "\n";
		}
		else
			std::cin >> choice;

		// input validation loop
		while (std::cin.fail() || (choice != 'y' && choice != 'n' && choice != 'Y' && choice != 'N'))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << " >> Please enter y or n: ";
			std::cin >> choice;
		}
		if (choice == 'y' || choice == 'Y')
		{
			// set player's surrender flag
			players[p].setSurrender();
			// proceed to next player
			continue;
		}

		// split handling: first two cards must be of equal value-----------------------------
		// use loop since this can happen multiple times
		// h can't index past handCount and player can't split past maxHands - 1
		size_t h{ 0 };
		while (h < players[p].getHandCount() && players[p].getHandCount() < Player::maxHands)
		{
			if (players[p].getHand(h).getCard(0) == players[p].getHand(h).getCard(1)
					|| (players[p].getHand(h).getCard(0) >= 10 && players[p].getHand(h).getCard(1) >= 10))
			{
				// player must have enough money for all splits
				if (players[p].getBet(0) * (players[p].getHandCount() + 1) > players[p].getBalance())
				{
					std::cout << "Not enough money to split.\n";
					break;
				}
				std::cout << " > Hand " << h + 1 << " is eligible to split. Would you like to? [y/n]: ";
				if (demo)
				{
					if (p == 4 || p == 5) // players 4 and 5 play randomly
						choice = demoDecide();
					else // others play with strategy
					{
						// store the splittable card value for tests
						unsigned int playerCard{ players[p].getHand(h).getCard(0) };

						if (playerCard == 8 || playerCard == 1)
							choice = 'y';
						else if (playerCard == 4 || playerCard == 5 || playerCard >= 10
							|| dealerCard == 1 || dealerCard >= 7)
							choice = 'n';
						else
							choice = 'y';
					}
					std::cout << choice << "\n";
				}
				else
					std::cin >> choice;

				// input validation loop
				while (std::cin.fail() || (choice != 'y' && choice != 'n' && choice != 'Y' && choice != 'N'))
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << " >> Please enter y or n: ";
					std::cin >> choice;
				}
				if (choice == 'y' || choice == 'Y')
				{
					// split hands and duplicate bet for new hand
					// new hand index is one less than player p's hand count
					players[p].split(h);
					players[p].addBet(players[p].getBet(h));
					// draw second card for each hand
					players[p].addCard(h, gameDeck.drawCard());
					players[p].addCard(players[p].getHandCount() - 1, gameDeck.drawCard());
					// reprint all hands due to split
					std::cout << " Reprinting all player's hands due to split: \n";
					for (size_t i{ 0 }; i < players[p].getHandCount(); i++)
					{
						std::cout << "  Hand " << i + 1 << ": " << players[p].getHand(i)
							<< " | Score: " << players[p].getHand(i).handScore()
							<< " | Bet: $" << players[p].getBet(i) << "\n";
					}
					// reset hand index to rescan all hands for new splits
					h = 0;
					continue;
				}
			}
			// inc h to check next hand for split
			h++;
		}

		// double down handling: all hands contain 2 cards------------------------------------
		for (h = 0; h < players[p].getHandCount(); h++)
		{
			// skip double downs if not enough money among all hands to double bet
			if (players[p].getBet(0) * (players[p].getHandCount() + 1) > players[p].getBalance())
			{
				std::cout << "Not enough money to double down.\n";
				break;
			}
			std::cout << " > Would you like to double down Hand";
			if (players[p].getHandCount() > 1)
				std::cout << " " << h + 1;
			std::cout << " (double bet and draw one more card)? [y/n]: ";
			if (demo)
			{
				if (p == 4 || p == 5) // players 4 and 5 play randomly
					choice = demoDecide();
				else // others play with strategy
				{
					// store the player score for tests
					unsigned int playerScore{ players[p].getHand(h).getScore() };

					if (playerScore == 11)
						choice = 'y';
					else if (playerScore == 10)
					{
						if (dealerCard == 1 || dealerCard >= 10)
							choice = 'n';
						else
							choice = 'y';
					}
					else if (dealerCard > 1 && dealerCard < 7
						&& (playerScore == 9 
						|| (players[p].getHand(h).getAceFlag() && playerScore >= 6 && playerScore <= 8)))
						choice = 'y';
					else
						choice = 'n';
				}
				std::cout << choice << "\n";
			}
			else
				std::cin >> choice;

			// input validation loop
			while (std::cin.fail() || (choice != 'y' && choice != 'n' && choice != 'Y' && choice != 'N'))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << " >> Please enter y or n: ";
				std::cin >> choice;
			}
			if (choice == 'y' || choice == 'Y')
			{
				// double bet, draw card, set double down flag
				players[p].setBet(h, players[p].getBet(h) * 2);
				players[p].addCard(h, gameDeck.drawCard());
				players[p].setDoubleDown(h);
				// announce results
				std::cout << "  Hand";
				if (players[p].getHandCount() > 1)
					std::cout << " " << h + 1;
				std::cout << ": " << players[p].getHand(h) << " | Score: ";
				// if Ace=11 doesn't bust, count higher score
				if (players[p].getHand(h).getAceFlag() && players[p].getHand(h).getScore() <= 11)
					std::cout << players[p].getHand(h).getScore() + 10 << "\n";
				else
					std::cout << players[p].getHand(h).getScore() << "\n";
				// check for bust
				if (players[p].getHand(h).getScore() > 21)
					std::cout << " Player busts.\n";
				else
					std::cout << " Player stands.\n";
			}
		}

		// track reprinting for split hands
		bool reprint{ true };

		// finally, give option to hit or stand-----------------------------------------------
		for (h = 0; h < players[p].getHandCount(); h++)
		{
			// if this hand already doubled down, skip
			if (players[p].getDoubleDown(h))
				continue;
			// finish if hand already 21
			if (players[p].getHand(h).getScore() == 21
				|| (players[p].getHand(h).getScore() == 11 && players[p].getHand(h).getAceFlag()))
			{
				std::cout << " Player stands.\n";
				continue;
			}
			// if player split any hands, reprint the hand in question once
			if (players[p].getHandCount() > 1 && reprint)
			{
				std::cout << "  Hand " << h + 1 << ": " << players[p].getHand(h)
					<< " | Score: " << players[p].getHand(h).handScore()
					<< " | Bet: $" << players[p].getBet(h) << "\n";
				reprint = false;
			}
			std::cout << " > Hit (deal another card)? [y/n]: ";
			if (demo)
			{
				if (p == 4 || p == 5) // players 4 and 5 play randomly
					choice = demoDecide();
				else // others play with strategy
				{
					// store the player score for tests
					unsigned int playerScore{ players[p].getHand(h).getScore() };
					// handle soft hands
					if (players[p].getHand(h).getAceFlag())
					{
						if (playerScore <= 8)
							choice = 'y';
						else
							choice = 'n';
					}
					// handle hard hands
					else
					{
						if (playerScore <= 11)
							choice = 'y';
						else if (playerScore == 12)
						{
							if (dealerCard == 4 || dealerCard == 5 || dealerCard == 6)
								choice = 'n';
							else
								choice = 'y';
						}
						else if (playerScore <= 16)
						{
							if (dealerCard == 1 || dealerCard >= 7)
								choice = 'y';
							else
								choice = 'n';
						}
						else
							choice = 'n';
					}
				}
				std::cout << choice << "\n";
			}
			else
				std::cin >> choice;

			// input validation loop
			while (std::cin.fail() || (choice != 'y' && choice != 'n' && choice != 'Y' && choice != 'N'))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << " >> Please enter y or n: ";
				std::cin >> choice;
			}
			if (choice == 'y' || choice == 'Y')
			{
				// draw card
				players[p].addCard(h, gameDeck.drawCard());
				// reprint hand after draw
				std::cout << "  Hand";
				if (players[p].getHandCount() > 1)
					std::cout << " " << h + 1;
				std::cout << ": " << players[p].getHand(h) << " | Score: "
					<< players[p].getHand(h).handScore() << "\n";
				// prevent from reprinting all hands
				reprint = false;
			}
			else
			{
				std::cout << " Player stands.\n";
				reprint = true;
				continue;
			}
			// if possible, proceed back through loop on same hand
			if (players[p].getHand(h).getScore() <= 21)
			{
				h--;
				reprint = false;
				continue;
			}
			else
				std::cout << " Player busts.\n";
		}
	}

	// dealer handling: hit until score >= 17-------------------------------------------------
	std::cout << "\nPlayer hand(s) final.\n --------------------\n"
		<< "Dealer's turn.\n\nDealer reveals hidden card.\n Hand: " << dealer.getHand(0)
		<< " | Score: " << dealer.getHand(0).handScore() << "\n";

	// if ALL player hands have blackjack, skip dealer play
	bool skip{ true };

	for (size_t p{ 0 }; p < Player::getPlayerCount(); p++)
		for (size_t h{ 0 }; h < players[p].getHandCount(); h++)
			if (players[p].getHand(h).handScore() != "Blackjack!")
				skip = false;

	while (!skip)
	{
		unsigned int dealerScore{ dealer.getHand(0).getScore() }; // used many times, easier to read
		// handle loop-terminating score conditions
		if (dealerScore > 21)
			std::cout << "\nDealer busts with " << dealerScore << ".\n";
		else if (dealerScore >= 17)
			std::cout << "\nDealer stands with " << dealerScore << ".\n";
		// handle soft 17 and 2-ace hands
		else if (dealer.getHand(0).getAceFlag() && dealerScore >= 7 && dealerScore <= 11)
			std::cout << "\nDealer stands with " << dealerScore + 10 << ".\n";
		else // otherwise draw and announce a card
		{
			dealer.addCard(0, gameDeck.drawCard());
			std::cout << "Dealer draws.\n Hand: " << dealer.getHand(0)
				<< " | Score: " << dealer.getHand(0).handScore() << "\n";
			// loop again to re-run loop termination checks
			continue;
		}
		// if card not drawn, break
		break;
	}
}

// pay and collect wins and losses
void Blackjack::winsAndLosses()
{
	// set 2 digit fixed precision for currency display
	std::cout << std::fixed << std::setprecision(2);

	// if dealer busts, reset dealer variable to set hand score to 0
	if (dealer.getHand(0).getScore() > 21)
		dealer = Actor();

	for (size_t p{ 0 }; p < Player::getPlayerCount(); p++)
	{
		std::cout << "\n Player";
		// only print Player number if more than one player
		if (Player::getPlayerCount() > 1)
			std::cout << " " << p + 1;
		// handle surrender
		if (players[p].getSurrender())
		{
			std::cout << " surrendered $" << players[p].getBet(0) / 2;
			// collect half original bet from player
			players[p].payOrCollect(-players[p].getBet(0) / 2);
			// announce new balance
			std::cout << "\n Player";
			if (Player::getPlayerCount() > 1)
				std::cout << " " << p + 1;
			std::cout << " has $" << players[p].getBalance() << "\n";
			// move on to next player
			continue;
		}
		// handle all other scoring scenarios
		for (size_t h{ 0 }; h < players[p].getHandCount(); h++)
		{
			// only print hand number if more than one hand
			if (players[p].getHandCount() > 1)
				std::cout << "\n Hand " << h + 1;
			// handle player bust
			if (players[p].getHand(h).getScore() > 21)
			{
				std::cout << " busts and loses $" << players[p].getBet(h);
				// collect player bet
				players[p].payOrCollect(-players[p].getBet(h));
				// proceed to next hand
				continue;
			}

			unsigned int playerScore{ players[p].getHand(h).getScore() };
			unsigned int dealerScore{ dealer.getHand(0).getScore() };

			// after bust handling, store Blackjack as 22
			// and temporary scores with Aces at max value for easier comparisons
			if (dealer.getHand(0).getAceFlag() && dealerScore <= 11)
				dealerScore += 10;
			if (players[p].getHand(h).getAceFlag() && playerScore <= 11)
				playerScore += 10;
			if (dealer.getHand(0).handScore() == "Blackjack!")
				dealerScore = 22;
			if (players[p].getHand(h).handScore() == "Blackjack!")
				playerScore = 22;

			// handle player win
			if (playerScore > dealerScore)
			{
				// handle player Blackjack
				if (players[p].getHand(h).handScore() == "Blackjack!")
				{
					std::cout << " wins $" << players[p].getBet(h) * 1.5f << " as Blackjack pays 3:2 odds.";
					players[p].payOrCollect(players[p].getBet(h) * 1.5f);
					continue;
				}
				// handle all other types of player win
				else
				{
					std::cout << " wins $" << players[p].getBet(h);
					players[p].payOrCollect(players[p].getBet(h));
					continue;
				}
			}
			// handle player tie
			else if (playerScore == dealerScore)
			{
				std::cout << " ties dealer.";
			}
			// handle player loss
			else
			{
				std::cout << " loses $" << players[p].getBet(h);
				players[p].payOrCollect(-players[p].getBet(h));
			}
		}
		// once player is finished, announce new balance
		std::cout << "\n Player";
		if (Player::getPlayerCount() > 1)
			std::cout << " " << p + 1;
		std::cout << " has $" << players[p].getBalance() << "\n";
	}
}

// print the current table with first dealer card either shown or not
void Blackjack::printTable(bool showDealer) const
{
	// set 2 digit fixed precision for currency display
	std::cout << std::fixed << std::setprecision(2);

	// store dealer score temporarily for repeated use
	unsigned int dealerScore{ dealer.getHand(0).getScore() };

	// print dealer Hand
	std::cout << "\n *Table*\n --------------------\n| Dealer\n" << "|  Hand: ";
	// show dealer score in addition to whole Hand
	if (showDealer)
	{
		std::cout << dealer.getHand(0) << " | Score: ";
		if (dealer.getHand(0).handScore() == "Blackjack!")
			std::cout << "Blackjack" << "\n";
		// assume round finished
		else if (dealerScore >= 17)
			std::cout << dealerScore << "\n";
		else if (dealer.getHand(0).getAceFlag())
			std::cout << dealerScore + 10 << "\n";
		else
			std::cout << dealerScore << "\n";
	}
	else
		std::cout << dealer.getHand(0).dealerHand() << " | (Dealer's first card is face-down.)\n";

	// print player Hands
	for (size_t p{ 0 }; p < Player::getPlayerCount(); p++)
	{
		// players numbered internally starting from 0, but print starting from 1
		std::cout << "|\n| Player " << p + 1 << "\n";
		// print message if player surrendered and continue to next player
		if (players[p].getSurrender())
		{
			std::cout << "|  Surrendered.\n";
			continue;
		}
		for (size_t h{ 0 }; h < players[p].getHandCount(); h++)
		{
			std::cout << "|  Hand";
			if (players[p].getHandCount() > 1)
				std::cout << " " << h + 1;
			std::cout << ": " << players[p].getHand(h) << " | Score: ";
			// if game is over, show max ace score
			if (showDealer && players[p].getHand(h).getAceFlag()
				&& players[p].getHand(h).getScore() <= 11
				&& players[p].getHand(h).handScore() != "Blackjack!")
				std::cout << players[p].getHand(h).getScore() + 10;
			else
				std::cout << players[p].getHand(h).handScore();
			std::cout << " | Bet: $" << players[p].getBet(h) << "\n";
		}
	}
	std::cout << " --------------------\n";
}

// return random yes/no decisions for demo mode
char Blackjack::demoDecide()
{
	if (random(random_engine))
		return 'y';
	else
		return 'n';
}