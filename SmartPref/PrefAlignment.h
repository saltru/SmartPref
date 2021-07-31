#pragma once
#include <string>
#include "Card.h"
#include "PrefPlayer.h"
#include "PrefMove.h"

enum PrefTurn
{
	PREF_TURN_EAST = 0,
	PREF_TURN_SOUTH,
	PREF_TURN_WEST
};

class PrefAlignment
{
private:
	vector<PrefPlayer> players;
	vector<Card> secret; //"prikup" on Russian - 2 secret cards from 32
	vector<Card> playedCards; //Cards out of the game

	int turn;
	int turnCount;

public:
	PrefAlignment(vector<Card> cards);
	~PrefAlignment();

	void print();

	void makeTurn();
};

