#pragma once
#include "Mancala.h"

/*
Constructor
*/
Mancala::Mancala(size_t size, unsigned int stones)
{
    // Assert size is large enough
    if (size < MIN_BOARD_SIZE)
        throw std::invalid_argument("size must be at least " + MIN_BOARD_SIZE);
    // Assert stone count
    if (stones == 0)
        throw std::invalid_argument("stones must be a positive number");

    size_t boardSize = size * 2 + 2;
    state_t state;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < size; j++)
            state.push_back(stones);
        state.push_back(0);
    }
    initialize(state);
}

/*
Constructor
*/
Mancala::Mancala(state_t state)
{
    initialize(state);
}

/*
Initializes member state and variables.
*/
void Mancala::initialize(state_t state)
{
    size = state.size();
    store1 = size / 2 - 1;
    store2 = size - 1;
    this->state = state;
}

/*
Finds all pits on the current player's side with >0 stones.
Returns a vector of pointers to valid pit numbers.
Calling function is responsible for freeing.
*/
std::vector<action_t *> Mancala::getValidMoves()
{
    std::vector<action_t *> moves;
    for (int i = 0, j; i < store1; i++)
    {
        if (turn == 1)
            j = i;
        else
            j = i + store1 + 1;
        if (state[j] > 0)
            moves.push_back(new action_t(j));
    }
    return moves;
}

/*
Returns a pointer to a clone of the game.
Calling function is responsible for freeing.
*/
Mancala *Mancala::clone()
{
    Mancala *newMancala = new Mancala(this->state);
    newMancala->turn = this->turn;
    newMancala->winner = this->winner;
    newMancala->gameStarted = this->gameStarted;
    return newMancala;
}

/*
Returns a unique numeric hash of the game state based on
pit values and current player turn.
*/
size_t Mancala::hash()
{
    std::string hashString(state.begin(), state.end());
    std::hash<std::string> hashFunction;
    return hashFunction(hashString + std::to_string(turn));
}

/*
Print Mancala board to console.
*/
void Mancala::print()
{
    std::cout << "P2\n";          //P2 label
    printf("%2d", state[store2]); //P2 Store
    //P2 side
    for (int i = store2 - 1; i > store1; i--)
        printf("%3d", state[i]);
    std::cout << "\n  ";
    //P1 side
    for (int i = 0; i < store1; i++)
        printf("%3d", state[i]);
    printf("%3d\n", state[store1]); //P1 Store
    //P1 label
    for (int i = 0; i < size / 2; i++)
        std::cout << "   ";
    std::cout << " P1\n";
    printf("Player %d's turn\n", turn); //turn
}

/*
Public function to make move.  Checks for valid move, then calls private makeValidMove().
Throws invalid_argument exception if move is invalid.
*/
void Mancala::makeMove(action_t move)
{

    if (this->winner > 0)
        return;
    if (isValidMove(move))
        makeValidMove(move);
    else
        throw std::invalid_argument("invalid move");
}

/*
Makes a valid move and sets Mancala for next move.
*/
void Mancala::makeValidMove(action_t pit)
{
    unsigned int endPit = distribute(pit);
    capture(endPit);
    endgame();
    changeTurn();
}

/*
distributes stones in pit.  Returns ending pit.
*/
int Mancala::distribute(unsigned int pit)
{
    unsigned int stones = state[pit]; //Stone count
    state[pit] = 0;                   //Clear pit
    //Distribute
    while (stones > 0)
    {
        pit = (pit + 1) % size;
        // skip if opponent bucket
        if (pit != store2 && turn == 1 ||
            pit != store1 && turn == 2)
        {
            state[pit] += 1;
            stones -= 1;
        }
    }
    return pit;
}

/*
Performs a capture of the pit opposite to int pit.
*/
void Mancala::capture(unsigned int pit)
{
    //Ending pit was empty and was NOT a player store
    if (state[pit] == 1 && pit != store1 && pit != store2)
    {
        int capPit = store2 - pit - 1; //Opposite pit
        //If the opposite pit has stones
        if (state[capPit] > 0)
        {
            if (turn == 1 && pit < store1) //Valid P1 capture
                state[store1] += state[pit] + state[capPit];
            else if (turn == 2 && pit > store1) //Valid P2 capture
                state[store2] += state[pit] + state[capPit];
            else //No capture
                return;
            //clear the pits
            state[pit] = 0;
            state[capPit] = 0;
        }
    }
}

/*
Perform endgame actions.
*/
void Mancala::endgame()
{
    if (isEndgame())
    {
        //Collect remaining stones and move to stores
        for (int i = 0; i < store1; i++)
        {
            state[store1] += state[i];
            state[store2] += state[store2 - i - 1];
            state[i] = 0;
            state[store2 - i - 1] = 0;
        }
        //get winner
        if (state[store1] > state[store2])
            winner = 1;
        else if (state[store2] > state[store1])
            winner = 2;
        else
            winner = 0; //tie
        return;
    }
}

/*
Check for empty side and return true if either side is empty.
*/
bool Mancala::isEndgame()
{
    int pitCount1 = 0, pitCount2 = 0;
    //Count stones on each side
    for (int i = 0; i < store1; i++)
    {
        pitCount1 += state[i];
        pitCount2 += state[store2 - i - 1];
    }
    return (pitCount1 == 0 || pitCount2 == 0);
}