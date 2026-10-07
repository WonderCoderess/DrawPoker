# DrawPoker

A text-based draw poker project written in C++, developed step by step to practise object-oriented programming, standard library containers, input validation and poker hand evaluation.

## Project status

Work in progress. The game runs in a loop of rounds: dealing, player and computer card exchange, a showdown with a winner announcement and result explanation, and a prompt to play another round. Betting is outside the current project scope.

Interactive mode is enabled by default. Define `RUN_TESTS` to run hand comparison checks instead of the interactive game.

## Implemented features

- A standard 52-card deck without jokers.
- Deck shuffling with `std::shuffle` and `std::mt19937`.
- Card drawing with an empty-deck check.
- Five cards dealt to the player and computer.
- A numbered display of the player's hand.
- Exchange of zero to five cards, with unique selections collected before replacement.
- Integer input validation, range checks and rejection of duplicate selections.
- Five-card hand validation and detection of all nine hand categories.
- Support for both ace-low (`A-2-3-4-5`) and ace-high (`10-J-Q-K-A`) straights.
- Comparison of two hands, including tie-breakers and draws.
- A simple computer card-exchange strategy that preserves made hands and repeated ranks, or keeps the highest card when the hand has no combination.
- Showdown explanations identifying the deciding category, pair, three of a kind, kicker or card value, with readable face-card names.
- A game loop with a showdown, winner announcement and multiple rounds.
- A compile-time test mode containing hand comparison checks.

## Hand categories

`HandRank` lists categories from weakest to strongest:

1. High card
2. One pair
3. Two pairs
4. Three of a kind
5. Straight
6. Flush
7. Full house
8. Four of a kind
9. Straight flush

A royal flush belongs to the straight flush category. `evaluateHand()` checks stronger categories first. Hands of the same category are compared with `getTieBreakRanks()`.

## Code structure

The implementation is currently contained in `DrawPoker/main.cpp`.

| Component | Responsibility |
| --- | --- |
| `Card` | Stores and displays a card's rank and suit. |
| `Deck` | Creates, shuffles and manages the deck. |
| `Player` | Stores a named hand, handles card exchange and evaluates hand categories. |
| `compareHands()` | Compares two hands and returns `1`, `-1` or `0`. |
| `rankName()` | Converts card rank values to readable names. |
| `handRankName()` | Returns a readable hand category name. |
| `explainResult()` | Explains which category or tie-break value determines the result. |
| `Game` | Runs the game loop: starting a round, the player's and computer's turns, the showdown and asking for another round. |
| `HandRank` | Represents the supported hand categories. |
| `GetNumber()` | Reads a whole-line integer input and validates its range. |

Card ranks use values `2` through `14`, where `11` is Jack, `12` is Queen, `13` is King and `14` is Ace. Suits are represented by `0` for Clubs, `1` for Diamonds, `2` for Hearts and `3` for Spades.

## Build and run

The code uses the C++ standard library and requires C++11 or later. No third-party libraries are needed.

### Visual Studio

1. Open `DrawPoker.slnx` in a Visual Studio version that supports this solution format.
2. Choose test mode or interactive mode as described below.
3. Build and run the project.

### Command-line alternative

With a C++ compiler such as GCC installed, run this command from the repository root:

```sh
g++ -std=c++11 -Wall -Wextra DrawPoker/main.cpp -o DrawPoker.exe
```

On Windows PowerShell, start the resulting program with:

```powershell
.\DrawPoker.exe
```

### Test mode

Uncomment the following definition near the top of `main.cpp` and rebuild:

```cpp
#define RUN_TESTS
```

The current checks compare equal hands with a pair of kings, then two hands with a pair of kings where an Ace kicker beats a Queen kicker. The comparison results, printed alongside the displayed hands, should be:

```text
0
0
1
-1
```

These are manual comparison checks, not a complete test suite. They do not yet test the computer's exchange strategy or result explanations.

### Interactive mode

Comment out the definition and rebuild:

```cpp
// #define RUN_TESTS
```

Each round, the program shuffles a fresh deck and deals five cards to each participant. Enter how many cards to exchange (`0-5`), then enter each selected card number (`1-5`) on a separate line. Repeated selections are rejected. After all selections are collected, the program replaces those cards and displays the updated hand.

The computer keeps all cards for a straight, flush, full house or straight flush. With a pair, two pairs, three of a kind or four of a kind, it keeps repeated ranks and exchanges single cards. With a high-card hand, it keeps the highest card and exchanges the other four.

In the showdown both hands and their categories are shown. The program announces the winner or a draw and explains the deciding category or tie-break value. Enter `1` to play another round or `0` to quit.

## Planned features

- Additional checks covering all hand categories and edge cases.
- Checks for the computer's card-exchange strategy and showdown explanations.

## Future improvements

- Unify `replaceCard()` and `replaceComputerCard()` into one method using zero-based indices and a `bool announce` parameter to control whether the replaced card is displayed. Convert user-facing card numbers (1-5) to indices (0-4) at the call site to avoid mixing indexing conventions.
