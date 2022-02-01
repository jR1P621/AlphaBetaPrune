#pragma once
#include "ABSearch.h"

/*
Public static function for performing a search.
Arguments:
rootState - Pointer to your starting state
utilityFunction - Function pointer. Return type = float. Arg = ABSearchableState*
maxDepth - Maximum depth of the search tree
startTime - optional - default = now()
comparePrecision - optional - precision used when checking utility equality
*/
template <class S, class A>
A ABSearch<S, A>::Search(ABSearchableState<S, A> *rootState,
                         float (*utilityFunction)(ABSearchableState<S, A> *),
                         unsigned int maxDepth, std::chrono::milliseconds maxTime,
                         std::chrono::steady_clock::time_point startTime,
                         float comparePrecision)
{
    std::map<size_t, float> *visitedStates = new std::map<size_t, float>; //for tracking visited states
    Node<ABSearchableState<S, A>, A> *root =
        new Node<ABSearchableState<S, A>, A>(rootState->clone(), 0); //starting node
    try
    {
        //Begin recursive search
        float value = maxLayer(root, FLT_MAX * -1, FLT_MAX, utilityFunction,
                               visitedStates, maxDepth, startTime, maxTime);

        delete visitedStates; //clean up
        //Find best move
        A bestAction;
        for (auto &child : root->children)
            if (child->value + comparePrecision >= value)
            {
                bestAction = *child->action;
                break;
            }
        delete root; //clean up

        // Print some search information to console
        if (DEBUG_LEVEL)
            printf("Depth: %d | Utility: %f\n", maxDepth, value);
        return bestAction;
    }
    // If exception, do some cleanup and let calling function deal with it
    catch (...)
    {
        delete root;
        delete visitedStates;
        throw;
    }
}

/*
Maximizing layer of alpha-beta pruning tree.
*/
template <class S, class A>
float ABSearch<S, A>::maxLayer(Node<ABSearchableState<S, A>, A> *node, float a, float b,
                               float (*utilityFunction)(ABSearchableState<S, A> *),
                               std::map<size_t, float> *visitedStates, unsigned int maxDepth,
                               std::chrono::steady_clock::time_point startTime, std::chrono::milliseconds maxTime)
{
    //Check our runtime
    if (std::chrono::steady_clock::now() > startTime + maxTime)
    {
        throw ABTimeout();
    }
    //Check for terminal tree node
    if (node->depth >= maxDepth || node->state->isABTerminalState())
    {
        return utilityFunction(node->state);
    }

    float value = FLT_MAX * -1;
    //Get valid actions and assign to new children
    std::vector<A *> actions = node->state->actions();
    for (auto &action : actions)
        node->addChild(node->state->clone(), action);

    //Process each new child
    for (auto &child : node->children)
    {
        child->state->doAction(*child->action);  //Apply the action
        size_t childHash = child->state->hash(); //Retrieve the state hash
        //If child state has not been visited
        if (visitedStates->find(childHash) == visitedStates->end())
        {
            (*visitedStates)[childHash] = child->value; //Add to visited states
            try
            {
                //Continue recursive search
                child->value = minLayer(child, a, b, utilityFunction,
                                        visitedStates, maxDepth, startTime, maxTime);
            }
            //If exception, clean everything up at this node and pass exception up
            catch (...)
            {
                node->clearChildren();
                throw;
            }
        }
        //If child state has been visited, get utility from hash table
        else
        {
            child->value = (*visitedStates)[childHash];
        }
        child->clearChildren(); //Don't need them anymore
        //Perform A-B pruning actions
        value = std::max(value, child->value);
        if (value >= b)
        {
            return value;
        }
        a = std::max(a, value);
    }
    return value;
}

/*
Minimizing layer of alpha-beta pruning tree.
*/
template <class S, class A>
float ABSearch<S, A>::minLayer(Node<ABSearchableState<S, A>, A> *node, float a, float b,
                               float (*utilityFunction)(ABSearchableState<S, A> *),
                               std::map<size_t, float> *visitedStates, unsigned int maxDepth,
                               std::chrono::steady_clock::time_point startTime, std::chrono::milliseconds maxTime)
{
    if (std::chrono::steady_clock::now() > startTime + maxTime)
    {
        throw ABTimeout();
    }
    if (node->depth >= maxDepth || node->state->isABTerminalState())
    {
        return utilityFunction(node->state);
    }
    float value = FLT_MAX;
    std::vector<A *> actions = node->state->actions();
    for (auto &action : actions)
        node->addChild(node->state->clone(), action);
    for (auto &child : node->children)
    {
        child->state->doAction(*child->action);
        size_t childHash = child->state->hash();
        if (visitedStates->find(childHash) == visitedStates->end())
        {
            (*visitedStates)[childHash] = child->value;

            try
            {
                child->value = maxLayer(child, a, b, utilityFunction,
                                        visitedStates, maxDepth, startTime, maxTime);
            }
            catch (ABTimeout e)
            {
                node->clearChildren();
                throw e;
            }
        }
        else
        {
            child->value = (*visitedStates)[childHash];
        }
        child->clearChildren();
        value = std::min(value, child->value);
        if (value <= a)
            return value;
        b = std::min(b, value);
    }
    return value;
}
