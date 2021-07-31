#include "pch.h"
#include "PrefPlayer.h"

using namespace std;

PrefPlayer::PrefPlayer(string name, vector<Card> cards)
{
	this->name = name;
	this->cards = cards;

	sort(this->cards.begin(), this->cards.end());
	updateSuiteNumbers();
}

void PrefPlayer::updateSuiteNumbers()
{
	int n = 0;
	for (int suit = 0; suit < PREF_CARD_SUITS; ++suit)
	{
		numSuits[suit] = 0;
		bool isSuit = false;
		while ((n < cards.size()) && (cards[n].getSuit() == suit))
		{
			if (!isSuit)
				minSuits[suit] = n;
			isSuit = true;
			maxSuits[suit] = n;

			numSuits[suit]++;
			n++;
		}
	}
}

void PrefPlayer::print()
{
	cout << name << ": ";
	for (auto &card : cards)
	{
		card.print();
	}
	cout << endl;

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < PREF_CARD_SUITS; ++i)
	{
		switch (i)
		{
		case SUIT_SPADES:
			SetConsoleTextAttribute(hStdout,
				FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << "SPADES:\t\t";
			break;
		case SUIT_CLUBS:
			SetConsoleTextAttribute(hStdout,
				FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << "CLUBS:\t\t";
			break;
		case SUIT_DIAMOND:
			SetConsoleTextAttribute(hStdout,
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << "DIAMONDS:\t";
			break;
		case SUIT_HEARTS:
			SetConsoleTextAttribute(hStdout,
				FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "HEARTS:\t\t";
			break;

		default:
			break;
		}

		cout << numSuits[i] << "\t";

		if (numSuits[i] > 0)
		{
			cards[minSuits[i]].print();
			cout << " - ";
			cards[maxSuits[i]].print();
		}

		cout << endl;
	}

	SetConsoleTextAttribute(hStdout,
		FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void PrefPlayer::generateMinMaxMovesOnFirstHand(vector<PrefMove> &moves, CardSuit suit)
{
	//Single-card suit turns
	if (1 == numSuits[suit])
	{
		moves.push_back(PrefMove(cards[minSuits[suit]]));
	}
	//Long suit turns
	else if (numSuits[suit] > 1)
	{
		//Weakest card in suit
		moves.push_back(PrefMove(cards[minSuits[suit]]));

		//Strongest card in suit
		moves.push_back(PrefMove(cards[maxSuits[suit]]));
	}
}

void PrefPlayer::generateMinMaxMoves(vector<PrefMove> &moves, CardSuit suit, vector<PrefMove> &currentTurnMoves)
{
	//Single-card suit turns
	if (1 == numSuits[suit])
	{
		moves.push_back(PrefMove(cards[minSuits[suit]]));
	}
	//Long suit turns
	else if (numSuits[suit] > 1)
	{
		//Pass the trick with strongest possible card
		Card card(VAL_UNKNOWN, SUIT_UNKNOWN);
		for (int i = minSuits[suit]; i <= maxSuits[suit]; ++i)
		{
			bool isStrongest = true;
			for (int j = 0; j < currentTurnMoves.size(); ++j)
			{
				if ((cards[i].getSuit() == currentTurnMoves[j].card.getSuit()) &&
					(cards[i].getValue() < currentTurnMoves[j].card.getValue()) ) //Pass trick by the strongest card from weaker than first hand
				{
					isStrongest = false;
					break;
				}
			}

			if (isStrongest)
				break;

			card = cards[i];
		}

		if (card.getSuit() != SUIT_UNKNOWN)
		{
			moves.push_back(card);			
		}

		//Overtake the trick with strongest card
		if (cards[maxSuits[suit]] != card)
			moves.push_back(PrefMove(cards[maxSuits[suit]]));
	}
}

Card PrefPlayer::getMaxCardForSuit(CardSuit suit, vector<PrefMove> currentTurnMoves)
{
	if (currentTurnMoves.size() > 0)
	{
		Card maxCard(VAL_7, SUIT_UNKNOWN);
		for (auto &m : currentTurnMoves)
		{
			if ((m.card.getSuit() == suit) &&
				(m.card.getValue() >= maxCard.getValue()))
			{
				maxCard = m.card;
			}
		}
		return maxCard;
	}

	//There are no cards in the list
	return Card(VAL_UNKNOWN, SUIT_UNKNOWN);
}

void PrefPlayer::generateBlockMovesForSuit(vector<PrefMove> &moves, CardSuit suit, vector<PrefMove> &currentTurnMoves)
{
	if (1 == numSuits[suit])
	{
		moves.push_back(PrefMove(cards[minSuits[suit]]));
	}
	else
	{
		Card maxCardOnTable = getMaxCardForSuit(suit, currentTurnMoves);
		
		//Simple case: if your maximum card weaken than leading card on the table, 
		//there is only 1 option for your turn - put the maximum card 
		if ((maxCardOnTable.getValue() != VAL_UNKNOWN) &&
			(cards[maxSuits[suit]].getValue() < maxCardOnTable.getValue()))
		{
			moves.push_back(PrefMove(cards[maxSuits[suit]]));
			return;
		}

		Card weakenMaxCard(VAL_UNKNOWN, SUIT_UNKNOWN);		
		Card blockMaxCard = cards[minSuits[suit]];

		for (int i = minSuits[suit]; i <= maxSuits[suit]; ++i)
		{
			//we need only 1 card to pass the trick (maximum from cards weaken than maximum card on the table)
			if ((maxCardOnTable.getValue() != VAL_UNKNOWN) && 
				(cards[i].getValue() < maxCardOnTable.getValue()))
			{
				weakenMaxCard = cards[i];
			}
			else
			{
				//Block finished (we found "void" between card blocks)
				//Or this is the last card in the last block
				if (blockMaxCard.getValue() + 1 < cards[i].getValue())
				{
					//Add max card in this block to the turn list, only if:
					//1. There is no lead card on the table, yours will be first - so adding all options, but no single weakenMaxCard at the end
					//2. There is lead card and current card to add is weaker. Then it's not need to adding these options - just add single weakenMaxCard at the end
					if ((maxCardOnTable.getValue() == VAL_UNKNOWN) ||
						((maxCardOnTable.getValue() != VAL_UNKNOWN) && (blockMaxCard.getValue() > maxCardOnTable.getValue())) )
					{
						moves.push_back(PrefMove(blockMaxCard));
					}
				}

				blockMaxCard = cards[i];

			}
		}

		//Last block can't be added by previous "for", so adding it
		moves.push_back(PrefMove(blockMaxCard));

		if (weakenMaxCard.getValue() != VAL_UNKNOWN)
			moves.push_back(PrefMove(weakenMaxCard));
	}
}

//Drop one maximum card in each suit
void PrefPlayer::generateDropCardMoves(vector<PrefMove> &moves)
{
	for (int i = 0; i < PREF_CARD_SUITS; ++i)
	{
		if (numSuits[i] > 0)
		{
			moves.push_back(cards[maxSuits[i]]);
		}
	}
}

vector<PrefMove> PrefPlayer::generateMoves(vector<PrefMove> currentTurnMoves, int currentIndex, vector<Card> playedCards, CardSuit initialTurnSuit)
{
	int i;
	int index = currentIndex;
	vector<PrefMove> moves;
	Card maxCardOnTable;

	switch (currentTurnMoves.size())
	{
	case 0: //It's first hand turn

		if (SUIT_UNKNOWN == initialTurnSuit)
		{
			for (i = 0; i < PREF_CARD_SUITS; ++i)
			{
				if (numSuits[i] > 0)
					generateBlockMovesForSuit(moves, (CardSuit)i, currentTurnMoves);
			}
		}
		//First and second turn with an "initial" suit provided by two secret cards
		else
		{			
			if (numSuits[initialTurnSuit] > 0)
			{
				generateBlockMovesForSuit(moves, initialTurnSuit, currentTurnMoves);
			}
			//Drop a card (reasonable to drop top in each suit)
			else
			{
				generateDropCardMoves(moves);
			}
		}

		break;

	case 1: //It's second hand turn

		//initialTurnSuit can't be SUIT_UNKNOWN in second and third hands

		if (numSuits[initialTurnSuit] > 0) // We have that suit
		{
			generateBlockMovesForSuit(moves, initialTurnSuit, currentTurnMoves);

		}
		//Drop a card (reasonable to drop top in each suit)
		else
		{
			generateDropCardMoves(moves);
		}

		break;

	case 2: //It's third hand turn

		maxCardOnTable = getMaxCardForSuit(initialTurnSuit, currentTurnMoves);

		//initialTurnSuit can't be SUIT_UNKNOWN in second and third hands
		if (numSuits[initialTurnSuit] > 0) // We have that suit
		{
			//At least 1 card on the table is in "right" initial suit, not dropped card
			if (maxCardOnTable.getValue() != VAL_UNKNOWN)
			{
				//If all our cards stronger than max card on the table, 
				//then take the trick with the maximum card
				if (maxCardOnTable.getValue() < cards[minSuits[initialTurnSuit]].getValue())
				{					
					moves.push_back(PrefMove(cards[maxSuits[initialTurnSuit]]));
				}
				else
				{
					//In this case, add 2 options: maximum card to pass the trick and 
					Card weakenMaxCard(VAL_UNKNOWN, SUIT_UNKNOWN);

					for (int i = minSuits[initialTurnSuit]; i <= maxSuits[initialTurnSuit]; ++i)
					{
						//we need only 1 card to pass the trick (maximum from cards weaken than maximum card on the table)
						if (cards[i].getValue() < maxCardOnTable.getValue())
							weakenMaxCard = cards[i];
					}

					//Add maximum card weaken than max on the table - to pass the trick
					if (weakenMaxCard.getValue() != VAL_UNKNOWN)
						moves.push_back(PrefMove(weakenMaxCard));

					//Add maximum card in suit to take the trick
					//prevent doubling
					if (weakenMaxCard != cards[maxSuits[initialTurnSuit]])
						moves.push_back(PrefMove(cards[maxSuits[initialTurnSuit]]));
				}
			}
			else
			{
				//We have no choice: just take the trick with strongest card
				moves.push_back(PrefMove(cards[maxSuits[initialTurnSuit]]));
			}
		}
		//Drop a card (reasonable to drop top in each suit)
		else
		{
			generateDropCardMoves(moves);
		}

		break;

	default:
		break;
	}

	return moves;
}

/*
vector<PrefMove> PrefPlayer::generateMoves(vector<PrefMove> currentTurnMoves, int currentIndex, vector<Card> playedCards, CardSuit initialTurnSuit)
{
	int i, j;
	int index = currentIndex;
	vector<PrefMove> moves;

	switch (currentTurnMoves.size())
	{
	case 0: //It's first hand turn

		if (SUIT_UNKNOWN == initialTurnSuit)
		{
			for (i = 0; i < PREF_CARD_SUITS; ++i)
				generateMinMaxMovesOnFirstHand(moves, (CardSuit) i);
		}
		//First and second turn with an "initial" suit provided by two secret cards
		else
		{
			//It's reasonable to start only from min or from max card in suit
			if (numSuits[initialTurnSuit] > 0)
			{
				generateMinMaxMovesOnFirstHand(moves, initialTurnSuit);
			}
			//Drop a card (reasonable to drop top in each suit)
			else
			{
				generateDropCardMoves(moves);
			}
		}

		break;

	case 1: //It's second hand turn

		//initialTurnSuit can't be SUIT_UNKNOWN in second and third hands
		
		if (numSuits[initialTurnSuit] > 0) // We have that suit
		{	
			if (currentTurnMoves[0].card.getSuit() == initialTurnSuit)
			{
				Card weakenMaxCard(VAL_UNKNOWN, SUIT_UNKNOWN);
				for (i = minSuits[initialTurnSuit]; i <= maxSuits[initialTurnSuit]; ++i)
				{
					if (cards[i].getValue() < currentTurnMoves[0].card.getValue())
					{
						weakenMaxCard = cards[i];
					}
					else
					{
						moves.push_back(PrefMove(cards[i]));
					}
				}
				if (weakenMaxCard.getSuit() != SUIT_UNKNOWN)
					moves.push_back(weakenMaxCard);
				//generateMinMaxMoves(moves, initialTurnSuit, currentTurnMoves);
			}
			else //First hand doesn't have this suit and dropped a card
			{
				generateMinMaxMovesOnFirstHand(moves, initialTurnSuit);
			}
			
		}
		//Drop a card (reasonable to drop top in each suit)
		else
		{
			generateDropCardMoves(moves);
		}

		break;

	case 2: //It's third hand turn

		//initialTurnSuit can't be SUIT_UNKNOWN in second and third hands
		if (numSuits[initialTurnSuit] > 0) // We have that suit
		{
			//First hand doesn't have this suit and dropped a card
			if ((currentTurnMoves[0].card.getSuit() == initialTurnSuit) ||
				(currentTurnMoves[1].card.getSuit() == initialTurnSuit) )
			{
				generateMinMaxMoves(moves, initialTurnSuit, currentTurnMoves);
			}
			else
			{
				//We have no choice: just Take with strongest card
				moves.push_back(PrefMove(cards[maxSuits[initialTurnSuit]]));
			}
		}
		//Drop a card (reasonable to drop top in each suit)
		else
		{
			generateDropCardMoves(moves);
		}
		
		break;

	default:
		break;
	}

	return moves;
}
*/