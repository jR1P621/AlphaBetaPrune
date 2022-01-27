/* 
The Mancala board is made up of two rows of six pockets (also called “holes,” or “pits”) each.
Four pieces are placed in each of the 12 pockets. The color of the pieces is irrelevant.
Each player has a “store” (also called a “Mancala”) to his/her right side of the Mancala board.

1. The game begins with one player picking up all of the pieces in any one of the pockets on his/her side.
2. Moving counter-clockwise, the player deposits one of the stones in each pocket until the stones run out.
3. If you run into your own Mancala (store), deposit one piece in it. If you run into your opponent's Mancala, skip it and continue moving to the next pocket.
4. If the last piece you move is in your own Mancala, you take another turn.
5. If the last piece you move is in an empty pocket on your side, you capture that piece and any pieces in the pocket directly opposite.
6. Always place all captured pieces in your Mancala (store).
7. The game ends when all six pockets on one side of the Mancala board are empty.
8. The player who still has pieces on his/her side of the board when the game ends captures all of those pieces.
9. Count all the pieces in each Mancala. The winner is the player with the most pieces.
*/

#include <stdexcept>
#include <iostream>

using std::cout;
using std::invalid_argument;
using std::swap;

const int DEFAULT_SIZE = 14;
const int DEFAULT_STONES = 4;
const int MIN_BOARD_SIZE = 10;

class Board
{
private:
    Board(int *state, int size);
    int *state, size, turn = 0, winner = -1, pStore, oStore;
    void rotateBoard();
    void initialize(int *state, int size);
    void makeValidMove(int move);
    void changeTurn() { this->turn = (this->turn + 1) % 2; };
    bool isEndgame();
    void capture(int pit);
    int distribute(int pit);
    void endgame();

public:
    Board() : Board(DEFAULT_SIZE, DEFAULT_STONES){};
    Board(int size, int stones);
    ~Board();
    void makeMove(int move);
    int getTurn() { return this->turn; };
    int getSize() { return this->size; };
    int getWinner() { return this->winner; };
    Board clone();
    void print();
    void copyState(int *state);
};

Board::Board(int size, int stones)
{
    // Assert size is positive, even, large enough
    if (size <= 0)
        throw std::invalid_argument("size must be a positive number");
    if (size < MIN_BOARD_SIZE)
        throw std::invalid_argument("size must be at least " + MIN_BOARD_SIZE);
    if (size % 2 == 1)
        throw std::invalid_argument("size must be an even number");
    // Assert stone count
    if (stones <= 0)
        throw std::invalid_argument("stones must be a positive number");

    int state[size] = {};
    for (int i = 0; i < (size / 2) - 1; i++)
    {
        state[i] = stones;
        state[size - i - 2] = stones;
    }
    this->initialize(state, size);
}

Board::Board(int *state, int size)
{
    this->initialize(state, size);
}

/*
Creates board state on heap and initializes values.
*/
void Board::initialize(int *state, int size)
{
    this->size = size;
    this->pStore = size / 2 - 1;
    this->oStore = size - 1;
    this->state = new int[size];
    for (int i = 0; i < size; i++)
    {
        this->state[i] = state[i];
    }
}

Board::~Board()
{
    delete[] this->state;
}

Board Board::clone()
{
    Board newBoard = Board(this->state, this->size);
    newBoard.turn = this->turn;
    return newBoard;
}

void Board::rotateBoard()
{
    for (int i = 0; i < pStore + 1; i++)
    {
        swap(this->state[i], this->state[(i + pStore + 1)]);
    }
    this->changeTurn();
}

/*
Print board to console.
*/
void Board::print()
{
    printf(" \t%d :P%d\n", this->state[this->oStore], (this->turn + 1) % 2);
    for (int i = 0; i < this->pStore; i++)
    {
        printf("%2d: %d\t%d\n", i, this->state[i], this->state[this->oStore - i - 1]);
    }
    printf("P%d: %d\n", this->turn, this->state[this->pStore]);
}

/*
Public function to make move.  Checks for valid move, then calls private makeValidMove().
*/
void Board::makeMove(int move)
{
    if (this->winner > 0)
        return;
    if (move < 0 || move > this->pStore - 1)
        throw std::invalid_argument("invalid move");
    if (this->state[move] < 1)
        throw std::invalid_argument("space is empty");
    this->makeValidMove(move);
}

/*
Makes a valid move and sets board for next move.
*/
void Board::makeValidMove(int move)
{
    int endPit = this->distribute(move);
    this->capture(endPit);
    this->endgame();

    // check for extra turn
    if (endPit != pStore)
    {
        this->rotateBoard();
    }
}

// distributes stones from pit
int Board::distribute(int pit)
{
    int stones = this->state[pit];
    this->state[pit] = 0;
    while (stones > 0)
    {
        pit = (pit + 1) % size;
        // skip opponent bucket
        if (pit != this->oStore)
        {
            this->state[pit] += 1;
            stones -= 1;
        }
    }
    return pit;
}

/*
Performs a capture of the pit opposite to int pit.
*/
void Board::capture(int pit)
{
    if (pit < this->pStore && this->state[pit] == 1)
    {
        int capPit = this->oStore - pit - 1;
        this->state[this->pStore] += this->state[pit] + this->state[capPit];
        this->state[pit] = 0;
        this->state[capPit] = 0;
    }
}

/*
Perform endgame actions.
*/
void Board::endgame()
{
    if (this->isEndgame())
    {
        // collect remaining stones
        for (int i = 0; i < this->pStore; i++)
        {
            this->state[this->pStore] += this->state[i];
            this->state[this->oStore] += this->state[this->oStore - i - 1];
            this->state[i] = 0;
            this->state[this->oStore - i - 1] = 0;
        }
        //get winner
        if (this->state[this->pStore] > this->state[this->oStore])
            this->winner = this->turn;
        else if (this->state[this->pStore] < this->state[this->oStore])
            this->winner = (this->turn + 1) % 2;
        else
            this->winner = 2; // tie
        return;
    }
}

/*
Check for empty side.
*/
bool Board::isEndgame()
{
    int pPitCount = 0, oPitCount = 0;
    for (int i = 0; i < this->pStore; i++)
    {
        pPitCount += this->state[i];
        oPitCount += this->state[this->oStore - i - 1];
    }
    return (pPitCount == 0 || oPitCount == 0);
}

void Board::copyState(int *state)
{
    for (int i = 0; i < this->size; i++)
    {
        state[i] = this->state[i];
    }
}