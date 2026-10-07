# DrawPoker

A text-based draw poker project written in C++, developed step by step to practise object-oriented programming, standard library containers, input validation and poker hand evaluation.

## Project status

Work in progress. The game runs in a loop of rounds: dealing, player card exchange, a showdown with a winner announcement, and a prompt to play another round. It does not support betting yet.

The source currently defines `RUN_TESTS`, so the program runs hand comparison checks instead of the interactive game.

## Implemented features

- A standard 52-card deck without jokers.
- Deck shuffling with `std::shuffle` and `std::mt19937`.
- Card drawing and a remaining-card count.
- Five cards dealt to the player and computer.
- A numbered display of the player's hand.
- Exchange of zero to five cards, with unique selections collected before replacement.
- Integer input validation, range checks and rejection of duplicate selections.
- Five-card hand validation and detection of all nine hand categories.
- Support for both ace-low (`A-2-3-4-5`) and ace-high (`10-J-Q-K-A`) straights.
- Comparison of two hands, including tie-breakers and draws.
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

Leave the following definition enabled near the top of `main.cpp`:

```cpp
#define RUN_TESTS
```

The current test creates three fives and two tens and checks that the hand is classified as a full house. Expected output:

```text
Full house detected correctly.
```

This is a single manual check, not a complete test suite.

### Interactive mode

Comment out the definition and rebuild:

```cpp
// #define RUN_TESTS
```

Each round, the program shuffles a fresh deck and deals five cards to each participant. Enter how many cards to exchange (`0-5`), then enter each selected card number (`1-5`) on a separate line. Repeated selections are rejected. After all selections are collected, the program replaces those cards and displays the updated hand.

The computer currently keeps all its cards. In the showdown both hands and their categories are shown and the winner is announced. Enter `1` to play another round or `0` to quit.

## Planned features

- A simple computer card-exchange strategy.
- Betting with chips, a pot, checking, calling, raising and folding.
- Additional checks covering all hand categories and edge cases.
