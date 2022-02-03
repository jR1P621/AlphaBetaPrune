#pragma once
/*
Wrapper class for calling multithreaded, iterative deepening alpha-beta searches.
*/
#include <thread>
#include <future>
#include <pthread.h>
#include "ABSearch.h"

//Defaults
const unsigned int DEFAULT_DEPTH = 10;
const unsigned int MAX_DEPTH = 50; //Will never search deeper than this
const std::chrono::milliseconds DEFAULT_TIME =
    std::chrono::milliseconds(10); //Max time spent searching

template <class S, class A>
class Bot
{
public:
    static A getMove(ABSearchableState<S, A> *state,
                     float (*utilityFunction)(ABSearchableState<S, A> *),
                     unsigned int searchDepth = DEFAULT_DEPTH,
                     std::chrono::milliseconds thinkTime = DEFAULT_TIME,
                     float comparePrecision = DEFAULT_PRECISION,
                     bool (*maxLayerFunction)(ABSearchableState<S, A> *) = nullptr);
};

#include "Bot.tpp"