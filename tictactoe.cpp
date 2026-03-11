#include<iostream>
#include<string>
using namespace std;
enum class GameStatus {
    IN_PROGRESS,
    DRAW,
    ENDED
};

enum class PlayerType {
    HUMAN,
    BOT
};

enum class BotDifficultyLevel {
    EASY
};

class Symbol {
private:
    char symbol;

public:
    Symbol(char symbol) : symbol(symbol) {}
    char getSymbol() const { return symbol; }
};

class Cell {
public:
    int row;
    int col;
    Symbol* symbol;

    Cell(int r, int c) : row(r), col(c), symbol(nullptr) {}

    bool isEmpty() const {
        return symbol == nullptr;
    }
};

#include <vector>
using namespace std;

class Board {
private:
    int size;

public:
    vector<vector<Cell>> grid;

    Board(int n) : size(n) {
        for(int i=0;i<n;i++){
            vector<Cell> row;
            for(int j=0;j<n;j++){
                row.push_back(Cell(i,j));
            }
            grid.push_back(row);
        }
    }

    int getSize() const { return size; }

    void display() {
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                if(grid[i][j].symbol)
                    cout << grid[i][j].symbol->getSymbol() << " ";
                else
                    cout << "- ";
            }
            cout << endl;
        }
    }
};

class Player;

class Move {
public:
    Player* player;
    Cell* cell;

    Move(Player* p, Cell* c) : player(p), cell(c) {}
};

class WinningStrategy {
public:
    virtual bool checkWinner(Board& board, Move& move) = 0;
};

class OrderOneWinningStrategy : public WinningStrategy {
public:
    bool checkWinner(Board& board, Move& move) override {

        int n = board.getSize();
        int r = move.cell->row;
        int c = move.cell->col;
        char sym = move.player->getSymbol()->getSymbol();

        bool rowWin = true, colWin = true, diagWin = true, antiDiagWin = true;

        for(int i=0;i<n;i++){
            if(board.grid[r][i].symbol == nullptr ||
               board.grid[r][i].symbol->getSymbol() != sym)
                rowWin = false;

            if(board.grid[i][c].symbol == nullptr ||
               board.grid[i][c].symbol->getSymbol() != sym)
                colWin = false;

            if(board.grid[i][i].symbol == nullptr ||
               board.grid[i][i].symbol->getSymbol() != sym)
                diagWin = false;

            if(board.grid[i][n-i-1].symbol == nullptr ||
               board.grid[i][n-i-1].symbol->getSymbol() != sym)
                antiDiagWin = false;
        }

        return rowWin || colWin || diagWin || antiDiagWin;
    }
};

class Player {
protected:
    string name;
    Symbol* symbol;
    PlayerType type;

public:
    Player(string name, Symbol* symbol, PlayerType type)
        : name(name), symbol(symbol), type(type) {}

    virtual Move makeMove(Board& board) = 0;

    Symbol* getSymbol() { return symbol; }

    string getName() { return name; }
};

class HumanPlayer : public Player {
public:
    HumanPlayer(string name, Symbol* symbol)
        : Player(name, symbol, PlayerType::HUMAN) {}

    Move makeMove(Board& board) override {
        int r, c;
        cout << name << " enter row and column: ";
        cin >> r >> c;

        return Move(this, &board.grid[r][c]);
    }
};

class BotPlayingStrategy {
public:
    virtual Move makeBotMove(Board& board, Player* bot) = 0;
};

#include <cstdlib>

class RandomBotStrategy : public BotPlayingStrategy {
public:
    Move makeBotMove(Board& board, Player* bot) override {

        int n = board.getSize();

        while(true){
            int r = rand() % n;
            int c = rand() % n;

            if(board.grid[r][c].isEmpty()){
                return Move(bot, &board.grid[r][c]);
            }
        }
    }
};

class Bot : public Player {
private:
    BotPlayingStrategy* strategy;

public:
    Bot(string name, Symbol* symbol, BotPlayingStrategy* strat)
        : Player(name, symbol, PlayerType::BOT),
          strategy(strat) {}

    Move makeMove(Board& board) override {
        return strategy->makeBotMove(board, this);
    }
};

class Game {
private:
    Board board;
    vector<Player*> players;
    vector<Move> moves;
    WinningStrategy* winningStrategy;
    GameStatus status;
    int currentPlayerIndex;

    Game(int size,
         vector<Player*> players,
         WinningStrategy* strategy)
        : board(size),
          players(players),
          winningStrategy(strategy),
          status(GameStatus::IN_PROGRESS),
          currentPlayerIndex(0) {}

public:

    class Builder {
    private:
        int size;
        vector<Player*> players;
        WinningStrategy* strategy;

    public:
        Builder& setSize(int s){
            size = s;
            return *this;
        }

        Builder& setPlayers(vector<Player*> p){
            players = p;
            return *this;
        }

        Builder& setWinningStrategy(WinningStrategy* ws){
            strategy = ws;
            return *this;
        }

        Game* build(){
            return new Game(size, players, strategy);
        }
    };

    void play(){
        while(status == GameStatus::IN_PROGRESS){

            board.display();

            Player* player = players[currentPlayerIndex];

            Move move = player->makeMove(board);

            if(!move.cell->isEmpty()){
                cout << "Invalid move!\n";
                continue;
            }

            move.cell->symbol = player->getSymbol();
            moves.push_back(move);

            if(winningStrategy->checkWinner(board, move)){
                board.display();
                cout << player->getName() << " Wins!\n";
                status = GameStatus::ENDED;
                break;
            }

            if(moves.size() == board.getSize() * board.getSize()){
                board.display();
                cout << "Game Draw!\n";
                status = GameStatus::DRAW;
                break;
            }

            currentPlayerIndex =
                (currentPlayerIndex + 1) % players.size();
        }
    }
};

int main() {

    Symbol* x = new Symbol('X');
    Symbol* o = new Symbol('O');

    Player* p1 = new HumanPlayer("Player1", x);
    Player* p2 = new Bot("Bot",
                         o,
                         new RandomBotStrategy());

    vector<Player*> players = {p1, p2};

    Game* game = Game::Builder()
                    .setSize(3)
                    .setPlayers(players)
                    .setWinningStrategy(new OrderOneWinningStrategy())
                    .build();

    game->play();

    return 0;
}