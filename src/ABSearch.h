#pragma once
/*
Class with static functions to perform alpha-beta pruning searches on
ABSearchableState classes.
*/
#include <map>
#include <chrono>
#include <cfloat>
#include "Node.h"

//Defaults
const float DEFAULT_PRECISION = 0.001; //for checking float equality
const int DEBUG_LEVEL = 0;             //1 for some console printing

/*
Custom exception for a timed out alpha-beta search.
*/
class ABTimeout : public std::exception
{
    virtual const char *what() const throw()
    {
        return "search failed to complete in allotted time";
    }
};

/*
A state template object that is compatible with the ABSearch class.
*/
template <class S, class A>
class ABSearchableState
{
protected:
    S state;

public:
    virtual std::vector<A *> actions() = 0;
    virtual ABSearchableState<S, A> *clone() = 0;
    virtual size_t hash() = 0;
    virtual void doAction(A action) = 0;
    virtual bool isABTerminalState() = 0;
};

template <class S, class A>
class ABSearch
{
public:
    static A Search(ABSearchableState<S, A> *rootState,
                    float (*utilityFunction)(ABSearchableState<S, A> *),
                    unsigned int maxDepth, std::chrono::milliseconds maxTime,
                    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now(),
                    float comparePrecision = DEFAULT_PRECISION);

private:
    static float minLayer(Node<ABSearchableState<S, A>, A> *node, float a, float b,
                          float (*utilityFunction)(ABSearchableState<S, A> *),
                          std::map<size_t, float> *visitedStates, unsigned int maxDepth,
                          std::chrono::steady_clock::time_point startTime, std::chrono::milliseconds maxTime);
    static float maxLayer(Node<ABSearchableState<S, A>, A> *node, float a, float b,
                          float (*utilityFunction)(ABSearchableState<S, A> *),
                          std::map<size_t, float> *visitedStates, unsigned int maxDepth,
                          std::chrono::steady_clock::time_point startTime, std::chrono::milliseconds maxTime);
};

#include "ABSearch.tpp"