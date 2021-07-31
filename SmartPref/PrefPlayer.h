#pragma once
#include <algorithm>
#include <array>
#include <vector>
#include <string>
#include "Card.h"
#include "PrefMove.h"

#define PREF_CARDS_PER_PLAYER (10)

class PrefPlayer
{
private:
	string name;
	vector<Card> cards;

	array<int, PREF_CARD_SUITS> numSuits; //Number of card of each suit
	array<int, PREF_CARD_SUITS> minSuits; //Index of weakest card in each suit
	array<int, PREF_CARD_SUITS> maxSuits; //Index of strongest card in each suit

	void updateSuiteNumbers();

	Card getMaxCardForSuit(CardSuit suit, vector<PrefMove> currentTurnMoves);

	void generateMinMaxMovesOnFirstHand(vector<PrefMove> &moves, CardSuit suit);
	void generateMinMaxMoves(vector<PrefMove> &moves, CardSuit suit, vector<PrefMove> &currentTurnMoves);
	void generateDropCardMoves(vector<PrefMove> &moves);
	void generateBlockMovesForSuit(vector<PrefMove> &moves, CardSuit suit, vector<PrefMove> &currentTurnMoves);

public:
	PrefPlayer(string name, vector<Card> cards);
	~PrefPlayer() {}

	void print();

	vector<PrefMove> generateMoves(vector<PrefMove> currentTurnMoves, int currentIndex, vector<Card> playedCards, CardSuit initialTurnSuit = SUIT_UNKNOWN);
};

