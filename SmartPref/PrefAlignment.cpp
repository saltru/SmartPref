#include "pch.h"
#include "PrefAlignment.h"

PrefAlignment::PrefAlignment(vector<Card> cards)
{
	vector<Card> playersCards[3];

	int n = 0;
	for (int i = 0; i < PREF_CARDS_PER_PLAYER; ++i)
	{
		for (int j = 0; j < 3; ++j)
			playersCards[j].push_back(cards[n++]);
	}

	for (int i = 0; i < 2; ++i)
		secret.push_back(cards[n++]);

	PrefPlayer east((string)"EAST", playersCards[0]);
	players.push_back(east);

	PrefPlayer south((string)"SOUTH", playersCards[1]);
	players.push_back(south);

	PrefPlayer west((string)"WEST", playersCards[2]);
	players.push_back(west);

	turn = PREF_TURN_EAST;
	turnCount = 0;
}

PrefAlignment::~PrefAlignment()
{
}

void PrefAlignment::print()
{
	cout << "TURN " << turnCount << ": ";
	switch (turn)
	{
	case PREF_TURN_EAST:
		cout << "EAST";
		break;
	case PREF_TURN_SOUTH:
		cout << "SOUTH";
		break;
	case PREF_TURN_WEST:
		cout << "WEST";
		break;
	default:
		break;
	}
	cout << endl;

	for (auto &player : players)
	{
		player.print();
		cout << endl;
	}

	cout << "SECRET:  ";
	for (auto &card : secret)
	{
		card.print();
	}

	cout << endl;
}

void PrefAlignment::makeTurn()
{
	CardSuit initialTurnSuit = SUIT_UNKNOWN;
	if (turnCount < 2)
	{
		initialTurnSuit = secret[turnCount].getSuit();
		playedCards.push_back(secret[turnCount]);
	}

	vector<PrefMove> currentTurnMoves;
	vector<PrefMove> movesTree;

	int firstHand = turn;
	int secondHand = (turn + 1) % 3;
	int thirdHand = (turn + 2) % 3;

	int n = 0; //number of new PrefAligments generated

	currentTurnMoves.empty();

	vector<PrefMove> firstHandMoves = players[firstHand].generateMoves(currentTurnMoves, n, playedCards, initialTurnSuit);
	for (auto &firstMove : firstHandMoves)
	{
		currentTurnMoves.push_back(firstMove);
		playedCards.push_back(firstMove.card);

		//Not a first/second turn
		if (turnCount >= 2)
			initialTurnSuit = firstMove.card.getSuit();

		vector<PrefMove> secondHandMoves = players[secondHand].generateMoves(currentTurnMoves, n, playedCards, initialTurnSuit);
		for (auto &secondMove : secondHandMoves)
		{
			currentTurnMoves.push_back(secondMove);
			playedCards.push_back(secondMove.card);

			vector<PrefMove> thirdHandMoves = players[thirdHand].generateMoves(currentTurnMoves, n, playedCards, initialTurnSuit);
			for (auto &thirdMove : thirdHandMoves)
			{
				cout << "MOVE " << n++ << ": ";

				if (turnCount < 2)
				{
					cout << "[";
					secret[turnCount].print();
					cout << "] ";
				}

				firstMove.card.print();
				cout << " ";
				secondMove.card.print();
				cout << " ";
				thirdMove.card.print();
				cout << endl;
			}

			playedCards.pop_back();
			currentTurnMoves.pop_back();
		}

		playedCards.pop_back();
		currentTurnMoves.pop_back();
	}


	turn = (turn + 1) % 3;
	turnCount++;
}