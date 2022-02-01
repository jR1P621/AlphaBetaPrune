#pragma once
/*
Child class of ABSearchableState.
Provides framework for adversarial games that are alpha-beta prune friendly.
(Eg., chess, checkers)
*/

#include "ABSearch.h"
#include "Bot.h"

template <class S, class A>
class Game : public ABSearchableState<S, A>
{
protected:
    //member variables
    short winner = -1, turn = 1;
    bool gameStarted = false;

    bool isValidMove(A move);

public:
    //overrides
    void doAction(A action) override
    {
        gameStarted = true;
        makeMove(action);
    };
    virtual Game<S, A> *clone() override = 0;
    std::vector<A *> actions() override { return getValidMoves(); };

    //AI wrappers
    A getAIMove(float (*utilityFunction)(ABSearchableState<S, A> *))
    {
        return Bot<S, A>::getMove(this, utilityFunction);
    };
    A getAIMove(float (*utilityFunction)(ABSearchableState<S, A> *),
                unsigned int searchDepth, std::chrono::milliseconds thinkTime)
    {
        return Bot<S, A>::getMove(this, utilityFunction, searchDepth, thinkTime);
    };

    //getters/setters
    virtual std::vector<A *> getValidMoves() = 0;
    int getTurn() { return turn; };
    int getWinner() { return winner; };
    void setTurn(int turn);

    virtual void makeMove(A move) = 0;
};

#include "Game.tpp"