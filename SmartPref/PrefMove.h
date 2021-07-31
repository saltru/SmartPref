#pragma once
#include <vector>
#include "Card.h"

class PrefMove;
typedef PrefMove* pPrefMove;

class PrefMove
{
public:
	Card card;
	vector<int> nextMoves; //Indices in whole moves tree vector

	PrefMove(Card card) : card(card), nextMoves() {}
	PrefMove(Card card, vector<int> nextMoves) : card(card), nextMoves(nextMoves) {}
	~PrefMove() {}

	void print();
};

