#pragma once
#include <random>
#include <vector>
#include "Card.h"

#define PREF_CARD_COUNT (32)

class CardDeck
{
private:
	vector<Card> cards;
	mt19937 rng;

public:
	CardDeck(mt19937 rng);
	~CardDeck();

	vector<Card> getCards();

	void shuffle();

	void print();
};

