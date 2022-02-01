#pragma once
#include "Bot.h"

/*
Manages threads and futures.  Calls ABSearch::Search.
*/
template <class S, class A>
A Bot<S, A>::getMove(ABSearchableState<S, A> *state, float (*utilityFunction)(ABSearchableState<S, A> *),
                     unsigned int searchDepth, std::chrono::milliseconds thinkTime, float comparePrecision)
{
    size_t maxThreads = std::thread::hardware_concurrency(); //get hardware capability
    std::vector<std::future<A>> futures;
    std::vector<A> results;
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now(); //Get current time

    //While we haven't reached max depth and we're within our time limit
    while (searchDepth <= MAX_DEPTH && startTime + thinkTime >= std::chrono::steady_clock::now())
    {
        //Get a count of all active (not finished/ready) threads
        int f = 0;
        for (auto &future : futures)
        {
            //Take 500 ms to check the futures vector instead of explicitly waiting/sleeping
            if (future.wait_for(std::chrono::milliseconds(500 / futures.size())) == std::future_status::timeout)
            {
                f++;
            }
        }

        //Create new threads upto max
        for (int i = 0; i < maxThreads - f; i++)
        {
            futures.push_back(std::async(&ABSearch<S, A>::Search, state, utilityFunction,
                                         searchDepth, thinkTime, startTime, comparePrecision));
            searchDepth++;
        }
    }

    //Retrieve our results
    for (auto &future : futures)
    {
        try
        {
            results.push_back(future.get());
        }
        //If exception, ignore it and keep checking results
        catch (...)
        {
            continue;
        }
    }
    //Try to return the last element of results (deepest completed depth)
    try
    {
        return results.back();
    }
    //If no search complete, they probably timed out.
    catch (...)
    {
        throw ABTimeout();
    }
}