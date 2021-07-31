#pragma once
#include <iostream>
#include <windows.h>

using namespace std;

#define PREF_CARD_VALUES (8)
enum CardValue {
	VAL_7 = 0,
	VAL_8,
	VAL_9,
	VAL_10,
	VAL_J,
	VAL_Q,
	VAL_K,
	VAL_A,
	VAL_UNKNOWN
};

#define PREF_CARD_SUITS (4)
enum CardSuit {
	SUIT_SPADES = 0,
	SUIT_CLUBS,
	SUIT_DIAMOND,
	SUIT_HEARTS,
	SUIT_UNKNOWN
};

class Card
{
private:
	CardValue value;
	CardSuit suit;

public:
	Card() : value(VAL_UNKNOWN), suit(SUIT_UNKNOWN) {}
	Card(CardValue value, CardSuit suit) : value(value), suit(suit) {}

	~Card();

	CardValue getValue();
	CardSuit getSuit();

	void setValue(CardValue value);
	void setSuit(CardSuit suit);

	void print();

	Card& operator=(const Card& rvalue);

	friend bool operator== (const Card& lvalue, const Card& rvalue);
	friend inline bool operator!= (const Card& lvalue, const Card& rvalue) { return !(lvalue == rvalue); }

	friend bool operator< (const Card& lvalue, const Card& rvalue);
	friend inline bool operator> (const Card& lvalue, const Card& rvalue) { return rvalue > lvalue; }
	friend inline bool operator<= (const Card& lvalue, const Card& rvalue) { return !(lvalue > rvalue); }
	friend inline bool operator>= (const Card& lvalue, const Card& rvalue) { return !(lvalue < rvalue); }

};

