#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <string>
#include <sstream>

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
		if (hand.size() != 5) {
			throw runtime_error("Hand must contain exactly 5 cards to check for a flush.");
		}

		int firstSuit = hand[0].getSuit();

		for (const Card& card : hand) {
			if (card.getSuit() != firstSuit) {
				return false;
			}
		}
		return true;
	}

private:
	vector<Card> hand;
};




/*
###### MAIN #######
*/

int main() {

	Deck deck;
	// cout << "Initial deck:" << endl;
	//deck.display();

	deck.shuffleCards();
	// cout << "Shuffled deck:" << endl;
	// deck.display();
	// cout << "Cards before drawing: " << deck.remainingCards() << endl;

	Player player;
	Player computer;

	for (int i = 0; i < 5; ++i) {
		player.receiveCard(deck.drawCard());
		computer.receiveCard(deck.drawCard());
	}

	player.exchangeCards(player, deck);
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