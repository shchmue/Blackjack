# Blackjack

A console blackjack game. My final project for an object-oriented programming course in C++.

## How to use

### Play using test.cpp

test.cpp first plays an unattended 5-player round of Blackjack with basic strategy. Then it prompts to either run another demo round or end. Upon ending, it allows user to play interactively with 1-5 players.

### Play by calling library independently

Create a Blackjack object and call its only public member function
```
void Blackjack::play(bool demoMode)
```
with argument deciding whether to play an interactive game (false, default) or run unattended demo (true).

## Requirements

Compiler must support C++11. Tested on VS2015, VS2017, and g++ 6.3.0 under MinGW.
