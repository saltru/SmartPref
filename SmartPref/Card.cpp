#include "pch.h"
#include "Card.h"


Card::~Card()
{
}

CardValue Card::getValue()
{
	return value;
}

CardSuit Card::getSuit()
{
	return suit;
}

void Card::setValue(CardValue value)
{
	this->value = value;
}

void Card::setSuit(CardSuit suit)
{
	this->suit = suit;
}

void Card::print()
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (suit)
	{
	case SUIT_UNKNOWN:
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		break;
	case SUIT_SPADES:
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case SUIT_CLUBS:
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case SUIT_DIAMOND:
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case SUIT_HEARTS:
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;

	default:
		break;
	}

	switch (value)
	{
	case VAL_UNKNOWN:
		cout << "?";
		break;
	case VAL_7:
		cout << "7";
		break;
	case VAL_8:
		cout << "8";
		break;
	case VAL_9:
		cout << "9";
		break;
	case VAL_10:
		cout << "10";
		break;
	case VAL_J:
		cout << "J";
		break;
	case VAL_Q:
		cout << "Q";
		break;
	case VAL_K:
		cout << "K";
		break;
	case VAL_A:
		cout << "A";
		break;

	default:
		break;
	}

	SetConsoleTextAttribute(hStdout,
		FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
}

Card& Card::operator=(const Card& rvalue)
{
	if (this != &rvalue)
	{
		this->suit = rvalue.suit;
		this->value = rvalue.value;
	}
	
	return *this;
}

bool operator==(const Card & lvalue, const Card & rvalue)
{
	if ((lvalue.suit == rvalue.suit) &&
		(lvalue.value == rvalue.value))
	{
		return true;
	}

	return false;
}

bool operator<(const Card & lvalue, const Card & rvalue)
{
	if (lvalue.suit < rvalue.suit)
	{
		return true;
	}
	else if (lvalue.suit == rvalue.suit)
	{
		if (lvalue.value < rvalue.value)
			return true;
	}

	return false;
}
