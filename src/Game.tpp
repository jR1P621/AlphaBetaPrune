#pragma once
#include "Game.h"

/*
Retrieves vector of valid moves and checks if passed in move exists in vector.
*/
template <class S, class A>
bool Game<S, A>::isValidMove(A thisMove)
{
    std::vector<A *> validMoves = getValidMoves();
    bool validMove = false;
    for (A *move : validMoves)
    {
        if (*move == thisMove)
            validMove = true;
        delete move;
    }
    return validMove;
}

/*
Changes the current turn to/from Player1 from/to Player2.
Does nothing if the game has already started
*/
template <class S, class A>
void Game<S, A>::setTurn(int turn)
{
    if (!gameStarted)
        if (turn % 2 == 1)
            this->turn = 1;
        else
            this->turn = 2;
}