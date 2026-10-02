#include <iostream>
#include <vector>

using namespace std;

/*
- wartości od 2 do 14, gdzie 11 to walet, 12 dama, 13 król, 14 as;
- kolory: 0 — Clubs ♣
1 — Diamonds ♦
2 — Hearts ♥
3 — Spades ♠ */

class Card {
public:
	Card(int r, int s) : rank(r), suit(s) {}

	void display() const {
		cout << "Rank: " << rank << ", suit: " << suit << endl;
		switch (suit) {
		case 0:
			cout << "Clubs" << endl;
			break;
		case 1:
			cout << "Diamonds" << endl;
			break;
		case 2:
			cout << "Hearts" << endl;
			break;
		case 3:
			cout << "Spades" << endl;
			break;
		default:
			cout << "Unknown suit" << endl;
		}
		
		switch (rank) {
			case (11):
				cout << "Jack" << endl;
				break;
			case(12):
				cout << "Queen" << endl;
				break;
			case(13):
				cout << "King" << endl;
				break;
			case(14):
				cout << "Ace" << endl;
				break;
			default:
				cout << rank << endl;
		} // End switch (rank)
	} // End display()

	int getRank() const {
		return rank;
	}
	int getSuit() const {
		return suit;
	}

private:
	int rank;
	int suit;
};

class Deck {

public: 
	Deck() {
		for (int s = 0; s < 4; ++s) { // Loop through suits
			for (int r = 2; r <= 14; ++r) { // Loop through ranks
				cards.emplace_back(r, s);
			}
		}

	} // end Deck()
	void display() const {
		for (const Card& card : cards) {
			card.display();
		}
	}
private:
	vector<Card> cards;
};

int main() {
	Card card(14, 3);
	card.display();
	cout << "Rank: " << card.getRank() << ", Suit: " << card.getSuit() << endl;
	Deck deck;
	deck.display();
	return 0;
}