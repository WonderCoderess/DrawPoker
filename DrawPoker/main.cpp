#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <string>
#include <sstream>
#define RUN_TESTS

using namespace std;

bool GetNumber(int minValue, int maxValue, int& number);

// class Card represents a single playing card with a rank and suit.
class Card {
public:
	Card(int r, int s) : rank(r), suit(s) {}

	// Display the card's rank and suit in a human-readable format.
	void display() const {

		//cout << "Number Rank: " << rank << ", suit: " << suit << endl;

		switch (rank) {
		case (11):
			cout << "Jack";
			break;
		case(12):
			cout << "Queen";
			break;
		case(13):
			cout << "King";
			break;
		case(14):
			cout << "Ace";
			break;
		default:
			cout << rank;
		}
		cout << " ";
		switch (suit) {
		case 0:
			cout << "Clubs";
			break;
		case 1:
			cout << "Diamonds";
			break;
		case 2:
			cout << "Hearts";
			break;
		case 3:
			cout << "Spades";
			break;
		default:
			cout << "Unknown suit";
		}
	}

	// Getters for rank and suit
	int getRank() const {
		return rank;
	}
	// Getters for rank and suit
	int getSuit() const {
		return suit;
	}
private:
	int rank;
	int suit;
};

// class Deck represents a standard deck of 52 playing cards.
class Deck {

public:
	// Constructor initializes the deck with 52 cards (13 ranks x 4 suits).
	Deck() {
		for (int s = 0; s < 4; ++s) { // Loop through suits
			for (int r = 2; r <= 14; ++r) { // Loop through ranks
				cards.emplace_back(r, s);
			}
		}

	}

	// Display all cards in the deck.
	void display() const {
		for (const Card& card : cards) {
			card.display();
			cout << endl;
		}
	}

	// Shuffle the deck using a random number generator.
	void shuffleCards() {
		random_device rd;
		mt19937 generator(rd());
		shuffle(cards.begin(), cards.end(), generator);
	}

	Card drawCard() {
		if (cards.empty()) {
			throw runtime_error("Deck is empty. Cannot draw a card.");
		}

		Card drawnCard = cards.back();
		cards.pop_back();
		return drawnCard;
	}

	size_t remainingCards() const { // size_t is the appropriate type for sizes and counts
		return (cards.size());
	}

private:
	vector<Card> cards;
};

enum class HandRank { // on najsilniejszego
	HighCard,
	OnePair,
	TwoPairs,
	ThreeOfAKind,
	Straight,
	Flush,
	FullHouse,
	FourOfAKind,
	StraightFlush
};

class Player {
public:
	void receiveCard(const Card& card) {
		hand.push_back(card);
	}
	void displayHand() const {
		int number = 1;
		cout << "Player's hand:" << endl;
		for (const Card& card : hand) {
			cout << "Card " << number++ << ": ";
			card.display();
			cout << endl;
		}
	}
	void replaceCard(int index, Deck& deck) {
		if (index <1 || index > hand.size()) {
			throw out_of_range("Invalid card index");
		}
		Card chosenToReplace = hand[index - 1]; // Adjust for 0-based index
		chosenToReplace.display();
		cout << " is being replaced with a new card from the deck." << endl;

		hand[index - 1] = deck.drawCard();
	}

	void exchangeCards(Player& player, Deck& deck) {
		cout << "Your hand before exchange:" << endl;
		player.displayHand();

		int count = 0;
		cout << "How many cards would you like to exchange? (0-5)" << endl;

		if (!GetNumber(0, 5, count)) {
			return;
		}

		if (count == 0) {
			cout << "You keep all your cards." << endl;
			return;
		}

		vector<int> selectedCardsToReplace;

		//Collect unique card members before replacing cards
		for (int i = 0; i < count; ++i) {
			int cardNumber = 0;
			bool alreadySelected = false;

			cout << "Enter the number of the card to exchange (1-5):" << endl;
			if (!GetNumber(1, 5, cardNumber)) {
				return;
			}

			for (int cardIndex : selectedCardsToReplace) {
				if (cardIndex == cardNumber) {
					alreadySelected = true;
					break;
				}
			}

			if (alreadySelected) {
				cout << "You have already selected this card. Try again." << endl;
				--i;
				continue;
			}

			selectedCardsToReplace.push_back(cardNumber);
		}
		//Replace cards only after the selection is complete to avoid index issues
		for (int cardIndex : selectedCardsToReplace) {
			player.replaceCard(cardIndex, deck);

		}
		cout << "Player's hand after exchange:" << endl;
		player.displayHand();
	}

	bool isFlush() const {
		validateHandSize();

		int firstSuit = hand[0].getSuit();

		for (const Card& card : hand) {
			if (card.getSuit() != firstSuit) {
				return false;
			}
		}
		return true;
	}

	bool isStraight() const {
		vector<int> rankList;
		validateHandSize();

		for (const Card& card : hand) {
			int cardRank = card.getRank();
			rankList.push_back(cardRank);

		}
		sort(rankList.begin(), rankList.end());
		int lastRank = rankList.back();

		// Special case: Ace acts as 1
		if (rankList[0] == 2 &&
			rankList[1] == 3 &&
			rankList[2] == 4 &&
			rankList[3] == 5 &&
			rankList[4] == 14) {
			return true;
		}
		// Check consecutive ranks
		for (int i = 1; i < 5; ++i) {
			int actualRank = rankList[i];
			int previousRank = rankList[i - 1];

			if (actualRank - previousRank != 1) {
				return false;
			}

		}
		return true;
	}

	int getFourOfAKindRank() const {
		vector<int> counts = countRanks();

		for (int rank = 2; rank <= 14; ++rank) {
			if (counts[rank] == 4) {
				return rank;
			}
		}

		return 0;
	}

	int getThreeOfAKindRank() const {
		vector<int> counts = countRanks();

		for (int rank = 2; rank <= 14; ++rank) {
			if (counts[rank] == 3) {
				return rank;
			}
		}

		return 0;
	}

	vector<int> getPairRanks() const { // it can be two pairs, so we return a vector of ranks
		vector<int> counts = countRanks();
		vector<int> pairRanks;

		for (int rank = 14; rank >= 2; --rank) { // Start from the highest rank to get the highest pair first
			if (counts[rank] == 2) {
				pairRanks.push_back(rank);
			}
		}
		return pairRanks;
	}

	bool isFullHouse() const {
		vector<int> listOfPairs = getPairRanks();
		int rankOfThrees = getThreeOfAKindRank();

		return listOfPairs.size() == 1 && rankOfThrees != 0;
	}

	bool isStraightFlush() const {
		// result of combining isStraight() i isFlush()
		return isStraight() && isFlush();
	}

	HandRank evaluateHand() const {
		if (isStraightFlush()) {
			return HandRank::StraightFlush;
		}
		if (getFourOfAKindRank() != 0) {
			return HandRank::FourOfAKind;
		}
		if (isFullHouse()) {
			return HandRank::FullHouse;
		}
		if (isFlush()) {
			return HandRank::Flush;
		}
		if (isStraight()) {
			return HandRank::Straight;
		}
		if (getThreeOfAKindRank() != 0) {
			return HandRank::ThreeOfAKind;
		}

		vector<int> pairRanks = getPairRanks();

		if (pairRanks.size() == 2) {
			return HandRank::TwoPairs;
		}
		if (pairRanks.size() == 1) {
			return HandRank::OnePair;
		}

		return HandRank::HighCard;
	}



private:
	vector<Card> hand;
	vector<int> countRanks() const {
		validateHandSize();
		vector<int> counts(15, 0);

		for (int i = 0; i < 5; i++)
		{
			int actualRank = hand[i].getRank();
			++counts[actualRank];
		}

		return counts;
	}

	void validateHandSize() const {
		if (hand.size() != 5) {
			throw runtime_error("Hand must contain exactly 5 cards.");
		}
	}



};




/*
###### MAIN #######
*/

int main() {

#ifdef RUN_TESTS
	Player testPlayer;

	testPlayer.receiveCard(Card(5, 0));
	testPlayer.receiveCard(Card(5, 1));
	testPlayer.receiveCard(Card(5, 2));
	testPlayer.receiveCard(Card(10, 0));
	testPlayer.receiveCard(Card(10, 1));

	if (testPlayer.evaluateHand() == HandRank::FullHouse) {
		cout << "Full house detected correctly." << endl;
	}
	else {
		cout << "Incorrect hand evaluation." << endl;
	}
#else

	Deck deck;
	deck.shuffleCards();


	Player player;
	Player computer;

	for (int i = 0; i < 5; ++i) {
		player.receiveCard(deck.drawCard());
		computer.receiveCard(deck.drawCard());
	}

	player.exchangeCards(player, deck);

#endif

	return 0;

}

bool GetNumber(int minValue, int maxValue, int& number)
{
	while (true)
	{
		cout << "Enter a number (" << minValue
			<< "-" << maxValue << "): ";

		string line;

		if (!getline(cin, line))
		{
			cout << endl << "Input ended." << endl;
			return false;
		}

		istringstream input(line);
		int value = 0;
		char extra;

		if (!(input >> value) ||
			(input >> extra) ||
			value < minValue || value > maxValue)
		{
			cout << "Invalid value. Try again." << endl;
		}
		else
		{
			number = value;
			return true;
		}
	}
}