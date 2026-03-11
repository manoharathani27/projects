#include<iostream>
#include<string>
#include <cstdlib>
using namespace std;
class Snake {
public:
    int head;
    int tail;

    Snake(int h, int t) : head(h), tail(t) {}
};

class Ladder {
public:
    int start;
    int end;

    Ladder(int s, int e) : start(s), end(e) {}
};

class DiceStrategy {
public:
    virtual int roll() = 0;
};


class NormalDice : public DiceStrategy {
public:
    int roll() override {
        return (rand() % 6) + 1;
    }
};

class Dice {
private:
    DiceStrategy* strategy;

public:
    Dice(DiceStrategy* strat) : strategy(strat) {}

    int roll() {
        return strategy->roll();
    }
};

#include <string>
using namespace std;

class Player {
private:
    string name;
    int position;

public:
    Player(string name) : name(name), position(0) {}

    string getName() { return name; }
    int getPosition() { return position; }

    void setPosition(int pos) {
        position = pos;
    }
};

#include <vector>

class Board {
private:
    int size;
    vector<Snake> snakes;
    vector<Ladder> ladders;

public:
    Board(int size,
          vector<Snake> snakes,
          vector<Ladder> ladders)
        : size(size), snakes(snakes), ladders(ladders) {}

    int getSize() { return size; }

    int checkSnakeOrLadder(int position) {

        for (auto& snake : snakes) {
            if (snake.head == position) {
                cout << "Bitten by snake! Go to "
                     << snake.tail << endl;
                return snake.tail;
            }
        }

        for (auto& ladder : ladders) {
            if (ladder.start == position) {
                cout << "Climbed ladder! Go to "
                     << ladder.end << endl;
                return ladder.end;
            }
        }

        return position;
    }
};

class Game {
private:
    Board board;
    vector<Player*> players;
    Dice dice;
    int currentPlayerIndex;

    Game(Board board,
         vector<Player*> players,
         Dice dice)
        : board(board),
          players(players),
          dice(dice),
          currentPlayerIndex(0) {}

public:

    class Builder {
    private:
        int size;
        vector<Snake> snakes;
        vector<Ladder> ladders;
        vector<Player*> players;
        DiceStrategy* diceStrategy;

    public:
        Builder& setBoardSize(int s) {
            size = s;
            return *this;
        }

        Builder& setSnakes(vector<Snake> s) {
            snakes = s;
            return *this;
        }

        Builder& setLadders(vector<Ladder> l) {
            ladders = l;
            return *this;
        }

        Builder& setPlayers(vector<Player*> p) {
            players = p;
            return *this;
        }

        Builder& setDiceStrategy(DiceStrategy* ds) {
            diceStrategy = ds;
            return *this;
        }

        Game* build() {
            Board board(size, snakes, ladders);
            Dice dice(diceStrategy);
            return new Game(board, players, dice);
        }
    };

    void play() {

        while (true) {

            Player* player = players[currentPlayerIndex];

            cout << player->getName()
                 << "'s turn.\n";

            int diceValue = dice.roll();
            cout << "Rolled: " << diceValue << endl;

            int newPosition =
                player->getPosition() + diceValue;

            if (newPosition > board.getSize()) {
                cout << "Move exceeds board. Skipping.\n";
            } else {
                newPosition =
                    board.checkSnakeOrLadder(newPosition);

                player->setPosition(newPosition);
                cout << "New Position: "
                     << newPosition << endl;

                if (newPosition == board.getSize()) {
                    cout << player->getName()
                         << " Wins!\n";
                    break;
                }
            }

            currentPlayerIndex =
                (currentPlayerIndex + 1)
                % players.size();

            cout << "-----------------\n";
        }
    }
};

int main() {

    vector<Snake> snakes = {
        Snake(99, 54),
        Snake(70, 55),
        Snake(52, 42)
    };

    vector<Ladder> ladders = {
        Ladder(25, 95),
        Ladder(6, 40),
        Ladder(11, 60)
    };

    Player* p1 = new Player("Alice");
    Player* p2 = new Player("Bob");

    vector<Player*> players = {p1, p2};

    Game* game = Game::Builder()
                    .setBoardSize(100)
                    .setSnakes(snakes)
                    .setLadders(ladders)
                    .setPlayers(players)
                    .setDiceStrategy(new NormalDice())
                    .build();

    game->play();

    return 0;
}
