#pragma once
/* 
The Mancala Board is made up of two rows of six pockets (also called “holes,” or “pits”) each.
Four (default) pieces are placed in each of the 12 (default) pockets.
Each player has a “store” (also called a “Mancala”) to his/her right side of the Mancala Board.

RULES
1. The game begins with one player picking up all of the pieces in any one of the pockets on his/her side.
2. Moving counter-clockwise, the player deposits one of the stones in each pocket until the stones run out.
3. If you run into your own Mancala (store), deposit one piece in it. If you run into your opponent's Mancala, skip it and continue moving to the next pocket.
4. If the last piece you drop is in your own Mancala, you take another turn.
5. If the last piece you move is in an empty pocket on your side, you capture that piece and any pieces in the pocket directly opposite.
6. Always place all captured pieces in your Mancala (store).
7. The game ends when all six pockets on one side of the Mancala Board are empty.
8. The player who still has pieces on his/her side of the Mancala when the game ends captures all of those pieces.
9. Count all the pieces in each Mancala. The winner is the player with the most pieces.

BOARD LAYOUT W/ PIT #'s EXAMPLE (size = 6)
    Player 2's side
Store
13 12 11 10  9  8  7
    0  1  2  3  4  5  6
                      Store
    Player 1's side    

*/

#include <string>
#include <iostream>
#include "Game.h"

typedef std::vector<unsigned int> state_t;
typedef unsigned int action_t;

//Some defaults
const size_t DEFAULT_SIZE = 6;
const unsigned int DEFAULT_STONES = 4;
const size_t MIN_BOARD_SIZE = 4;

class Mancala : public Game<state_t, action_t>
{
private:
    //member variables
    size_t size;
    unsigned int store1, store2;

    //construct/destruct
    Mancala(state_t state);
    void initialize(state_t state);

    void makeValidMove(action_t move);
    void changeTurn() { turn = (turn % 2) + 1; };
    bool isEndgame();
    void capture(unsigned int pit);
    int distribute(unsigned int pit);
    void endgame();

public:
    //construct/destruct
    Mancala() : Mancala(DEFAULT_SIZE, DEFAULT_STONES){};
    Mancala(size_t size, unsigned int stones);
    ~Mancala(){};

    //overrides
    void makeMove(action_t move) override;
    std::vector<action_t *> getValidMoves() override;
    bool isABTerminalState() override { return isEndgame(); };
    size_t hash() override;
    Mancala *clone() override;

    //getters
    int getStoneCount(unsigned int pit) { return state[pit % size]; };
    int getPlayer1Score() { return state[store1]; };
    int getPlayer2Score() { return state[store2]; };
    size_t getSize() { return size; };
    unsigned int getState(int i) { return this->state[i % size]; }

    //helpers
    void print();
};

#include "Mancala.cpp"