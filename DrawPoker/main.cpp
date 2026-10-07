#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>     // pair
#include <functional>  // greater
#define RUN_TESTS

using namespace std;

bool GetNumber(int minValue, int maxValue, int& number);

// class Card represents a single playing card with a rank and suit.
class Card {
public:
	Card(int r, int s) : rank(r), suit(s) {}

	// Display the card's rank and suit in a human-readable format.
	void display() const {

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
		for (int s = 0; s < 4; ++s) { 
			for (int r = 2; r <= 14; ++r) { 
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

	// Draw a card from the top of the deck. Throws an exception if the deck is empty.
	Card drawCard() {
		if (cards.empty()) {
			throw runtime_error("Deck is empty. Cannot draw a card.");
		}

		Card drawnCard = cards.back();
		cards.pop_back();
		return drawnCard;
	}

private:
	vector<Card> cards;
};

// Enum representing the rank of a poker hand, ordered from weakest to strongest.
enum class HandRank {
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

// class Player represents a player in the poker game, holding a hand of cards and providing methods to evaluate the hand.
class Player {
public:
	explicit Player(const string& playerName = "Player") : name(playerName) {}

	// Receive a card and add it to the player's hand.
	void receiveCard(const Card& card) {
		hand.push_back(card);
	}

	// Remove all cards from the player's hand before a new round.
	void clearHand() {
		hand.clear();
	}

	// Display the player's hand in a human-readable format.
	void displayHand() const {
		int number = 1;
		cout << name << "'s hand:" << endl;
		for (const Card& card : hand) {
			cout << "Card " << number++ << ": ";
			card.display();
			cout << endl;
		}
	}

	// Replace a card in the player's hand at the specified index with a new card drawn from the deck.
	void replaceCard(int index, Deck& deck) {
		if (index <1 || index > hand.size()) {
			throw out_of_range("Invalid card index");
		}
		Card chosenToReplace = hand[index - 1]; // Adjust for 0-based index
		chosenToReplace.display();
		cout << " is being replaced with a new card from the deck." << endl;

		hand[index - 1] = deck.drawCard();
	}

	// Allow the player to exchange cards in their hand with new cards from the deck.
	// Returns false if input ended before the exchange was finished.
	bool exchangeCards(Player& player, Deck& deck) {
		cout << "Your hand before exchange:" << endl;
		player.displayHand();

		int count = 0;
		cout << "How many cards would you like to exchange? (0-5)" << endl;

		if (!GetNumber(0, 5, count)) {
			return false;
		}

		if (count == 0) {
			cout << "You keep all your cards." << endl;
			return true;
		}

		vector<int> selectedCardsToReplace;

		//Collect unique card members before replacing cards
		for (int i = 0; i < count; ++i) {
			int cardNumber = 0;
			bool alreadySelected = false;

			cout << "Enter the number of the card to exchange (1-5):" << endl;
			if (!GetNumber(1, 5, cardNumber)) {
				return false;
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

		// Replace the selected cards with new cards from the deck
		for (int cardIndex : selectedCardsToReplace) {
			player.replaceCard(cardIndex, deck);

		}
		cout << "Hand after exchange:" << endl;
		player.displayHand();
		return true;
	}

	// Check if the player's hand is a flush (all cards of the same suit).
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

	// Check if the player's hand is a straight (five consecutive ranks).
	bool isStraight() const {
		vector<int> rankList;
		validateHandSize();

		for (const Card& card : hand) {
			int cardRank = card.getRank();
			rankList.push_back(cardRank);

		}
		sort(rankList.begin(), rankList.end());

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

	// Get the rank of the four of a kind in the player's hand, or return 0 if there is none.
	int getFourOfAKindRank() const {
		vector<int> counts = countRanks();

		for (int rank = 2; rank <= 14; ++rank) {
			if (counts[rank] == 4) {
				return rank;
			}
		}

		return 0;
	}

	//	Get the rank of the three of a kind in the player's hand, or return 0 if there is none.
	int getThreeOfAKindRank() const {
		vector<int> counts = countRanks();

		for (int rank = 2; rank <= 14; ++rank) {
			if (counts[rank] == 3) {
				return rank;
			}
		}

		return 0;
	}

	// Get the ranks of all pairs in the player's hand, returning a vector of ranks (can be more than one pair).
	vector<int> getPairRanks() const { 
		vector<int> counts = countRanks();
		vector<int> pairRanks;

		for (int rank = 14; rank >= 2; --rank) { 
			if (counts[rank] == 2) {
				pairRanks.push_back(rank);
			}
		}
		return pairRanks;
	}

	// Check if the player's hand is a full house (three of a kind and a pair).
	bool isFullHouse() const {
		vector<int> listOfPairs = getPairRanks();
		int rankOfThrees = getThreeOfAKindRank();

		return listOfPairs.size() == 1 && rankOfThrees != 0;
	}

	// Check if the player's hand is a straight flush (both a straight and a flush).
	bool isStraightFlush() const {
		// result of combining isStraight() i isFlush()
		return isStraight() && isFlush();
	}

	// Evaluate the player's hand and return its rank as a HandRank enum value.
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

	// Get the ranks of the cards in the player's hand for tie-breaking purposes, sorted by frequency and rank.
	vector<int> getTieBreakRanks() const {
		vector<int> counts = countRanks();
		vector<pair<int, int>> groups; // (how many times, rank)

		for (int rank = 14; rank >= 2; --rank) {
			if (counts[rank] > 0) {
				groups.push_back({ counts[rank], rank });
			}
		}
		// Sort by count first, then by rank, both highest first
		sort(groups.begin(), groups.end(), greater<pair<int, int>>());

		vector<int> ranks;
		for (const auto& group : groups) {
			ranks.push_back(group.second);
		}

		// Special case: A, 2, 3, 4, 5 straight, Ace acts as 1
		if (isStraight() && ranks[0] == 14 && ranks[1] == 5) {
			ranks = { 5, 4, 3, 2, 1 };
		}
		return ranks;
	}

private:
	string name;
	vector<Card> hand;

	// Count the occurrences of each rank in the player's hand, returning a vector where the index represents the rank and the value represents the count.
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

	// Validate that the player's hand contains exactly 5 cards, throwing an exception if not.
	void validateHandSize() const {
		if (hand.size() != 5) {
			throw runtime_error("Hand must contain exactly 5 cards.");
		}
	}
};

// Compare the hands of two players and return 1 if the first player wins, -1 if the second player wins, or 0 for a tie.
int compareHands(const Player& player, const Player& computer) {
	HandRank playerRank = player.evaluateHand();
	HandRank computerRank = computer.evaluateHand();

	if (playerRank > computerRank) {
		return 1;
	}
	if (playerRank < computerRank) {
		return -1;
	}

	vector<int> playerRanks = player.getTieBreakRanks();
	vector<int> computerRanks = computer.getTieBreakRanks();

	if (playerRanks > computerRanks) return 1;
	if (playerRanks < computerRanks) return -1;
	return 0;
}

// Return a human-readable name of a hand rank.
string handRankName(HandRank rank) {
	switch (rank) {
	case HandRank::HighCard:
		return "High card";
	case HandRank::OnePair:
		return "One pair";
	case HandRank::TwoPairs:
		return "Two pairs";
	case HandRank::ThreeOfAKind:
		return "Three of a kind";
	case HandRank::Straight:
		return "Straight";
	case HandRank::Flush:
		return "Flush";
	case HandRank::FullHouse:
		return "Full house";
	case HandRank::FourOfAKind:
		return "Four of a kind";
	case HandRank::StraightFlush:
		return "Straight flush";
	default:
		return "Unknown hand";
	}
}

// class Game controls the flow of the game: dealing, exchanging cards, showdown and starting the next round.
class Game {
public:
	Game() : player("Player"), computer("Computer") {}

	// Main game loop: play rounds until the player stops or input ends.
	void run() {
		cout << "Welcome to Draw Poker!" << endl;

		int roundNumber = 1;
		while (true) {
			cout << endl << "===== Round " << roundNumber++ << " =====" << endl;

			if (!playRound()) {
				break;
			}
			if (!askPlayAgain()) {
				break;
			}
		}

		cout << "Thanks for playing!" << endl;
	}

private:
	Deck deck;
	Player player;
	Player computer;

	// Play a single round. Returns false if input ended during the round.
	bool playRound() {
		startRound();

		if (!playerTurn()) {
			return false;
		}
		computerTurn();
		showdown();

		return true;
	}

	// Prepare a new shuffled deck and deal five cards to each participant.
	void startRound() {
		deck = Deck();
		deck.shuffleCards();

		player.clearHand();
		computer.clearHand();

		for (int i = 0; i < 5; ++i) {
			player.receiveCard(deck.drawCard());
			computer.receiveCard(deck.drawCard());
		}
	}

	// Let the player exchange cards.
	bool playerTurn() {
		return player.exchangeCards(player, deck);
	}

	// The computer has no exchange strategy yet, so it keeps its cards.
	void computerTurn() {
		cout << endl << "Computer keeps all its cards." << endl;
	}

	// Reveal both hands, compare them and announce the result.
	void showdown() {
		cout << endl << "===== Showdown =====" << endl;

		player.displayHand();
		cout << "-> " << handRankName(player.evaluateHand()) << endl << endl;

		computer.displayHand();
		cout << "-> " << handRankName(computer.evaluateHand()) << endl << endl;

		int result = compareHands(player, computer);
		if (result > 0) {
			cout << "You win!" << endl;
		}
		else if (result < 0) {
			cout << "Computer wins!" << endl;
		}
		else {
			cout << "It's a draw!" << endl;
		}
	}

	// Ask whether to play another round. Returns false if the player says no or input ends.
	bool askPlayAgain() {
		int answer = 0;
		cout << endl << "Play another round? (1 = yes, 0 = no)" << endl;

		if (!GetNumber(0, 1, answer)) {
			return false;
		}
		return answer == 1;
	}
};

// ######## MAIN FUNCTION ########

int main() {

#ifdef RUN_TESTS

	Player testPlayer;
	
	testPlayer.receiveCard(Card(13, 0));
	testPlayer.receiveCard(Card(13, 1));
	testPlayer.receiveCard(Card(14, 2));
	testPlayer.receiveCard(Card(8, 0));
	testPlayer.receiveCard(Card(2, 1));
	testPlayer.displayHand();


	Player testComputer;

	testComputer.receiveCard(Card(13, 0));
	testComputer.receiveCard(Card(13, 1));
	testComputer.receiveCard(Card(14, 2));
	testComputer.receiveCard(Card(8, 3));
	testComputer.receiveCard(Card(2, 0));
	testComputer.displayHand();

	cout << compareHands(testPlayer, testComputer) << endl;
	cout << compareHands(testComputer, testPlayer) << endl;

	Player testPlayer1;
	testPlayer1.receiveCard(Card(13, 0));
	testPlayer1.receiveCard(Card(13, 1));
	testPlayer1.receiveCard(Card(14, 2));
	testPlayer1.receiveCard(Card(8, 3));
	testPlayer1.receiveCard(Card(2, 0));

	Player testComputer1;
	testComputer1.receiveCard(Card(13, 2));
	testComputer1.receiveCard(Card(13, 3));
	testComputer1.receiveCard(Card(12, 0));
	testComputer1.receiveCard(Card(9, 1));
	testComputer1.receiveCard(Card(7, 2));

	cout << compareHands(testPlayer1, testComputer1) << endl;
	cout << compareHands(testComputer1, testPlayer1) << endl;

#else

	Game game;
	game.run();

#endif

	return 0;

}

// Get a number from the user within a specified range, returning true if successful and false if input ends.
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