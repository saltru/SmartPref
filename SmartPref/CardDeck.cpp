#include "pch.h"
#include "CardDeck.h"


CardDeck::CardDeck(mt19937 rng)
{
	this->rng = rng;

	for (int i = VAL_7; i <= VAL_A; ++i)
	{
		for (int j = SUIT_SPADES; j <= SUIT_HEARTS; ++j)
		{
			Card card((CardValue) i, (CardSuit) j);
			cards.push_back(card);
		}
	}
}


CardDeck::~CardDeck()
{

}

vector<Card> CardDeck::getCards()
{
	return cards;
}

//Fisher-Yetts shuffle
void CardDeck::shuffle()
{
	int j;

	for (int i = PREF_CARD_COUNT - 1; i >= 0; --i)
	{
		uniform_int_distribution<int> uni(0, i);
		j = uni(rng);
		swap(cards[i], cards[j]);
	}
}

void CardDeck::print()
{
	cout << "DECK:  ";
	for (auto &card : cards)
	{
		card.print();
	}

	cout << endl;
}