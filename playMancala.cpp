/*
Shows basic Mancala functionality.  Let's a human play against the computer in the console.
*/

#include <iostream>
#include <random>
#include "src/Mancala.h"
#include "src/Bot.h"
#include <stdio.h>
#include <stdlib.h>

using std::cin;
using std::cout;
using std::invalid_argument;
using std::rand;
using std::chrono::milliseconds;

Mancala *myBoard;
float utility1(Mancala *game, int position);
bool maxLayerFunction(Mancala *game, int position);

int main(int argc, char const *argv[])
{
    myBoard = new Mancala();
    int move;
    do
    {
        myBoard->print();
        if (myBoard->getTurn() == 1)
        {
            cout << "Select Pit: ";
            cin >> move;
        }
        else
        {
            move = myBoard->getAIMove([](auto *game)
                                      { return utility1((Mancala *)game, 2); },
                                      [](auto *game)
                                      { return maxLayerFunction((Mancala *)game, 2); },
                                      (unsigned int)5,
                                      (milliseconds)5000);
        }
        try
        {
            myBoard->makeMove(move);
        }
        catch (invalid_argument e)
        {
            printf("%s\n", e.what());
        }
    } while (myBoard->getWinner() < 0);
    myBoard->print();
    delete myBoard;
}

float utility1(Mancala *game, int position)
{
    float utility = (float)game->getPlayer1Score() - (float)game->getPlayer2Score();
    utility += ((float)game->getState(game->getSize() - 2) + (float)game->getState(game->getSize() - 3) / 2) -
               ((float)game->getState(game->getSize() / 2 - 2) + (float)game->getState(game->getSize() / 2 - 3) / 2) / 2;
    float variation = 0.5;
    utility += (float)rand() / RAND_MAX / (1 / variation) - variation;

    if (position == 2)
        return utility *= -1;
    else
        return utility;
}

bool maxLayerFunction(Mancala *game, int position)
{
    return game->getTurn() == position;
}