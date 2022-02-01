#pragma once
/*
Node for alpha-beta/minimax tree structures.
*/
#include <vector>

template <class S, class A>
class Node
{
public:
    //member variables
    S *state;
    A *action;
    float value;
    unsigned int depth;
    std::vector<Node *> children;

    //construct/destruct
    Node<S, A>(S *state,
               A *action,
               unsigned int depth)
        : state{state},
          action{action},
          depth{depth} {};
    Node<S, A>(S *state,
               unsigned int depth)
        : Node(state, nullptr, depth){};
    ~Node<S, A>()
    {
        //Deallocate all used memory
        clearChildren();
        if (state)
            delete state;
        if (action)
            delete action;
    };

    /*
    Deallocates child nodes.
    */
    void clearChildren()
    {
        while (children.size() > 0)
        {
            delete children.back();
            children.pop_back();
        }
    };

    /*
    Creates a new Node based on parameters and adds pointer to children vector.
    */
    void addChild(S *state, A *action)
    {
        children.push_back(new Node(state, action, depth + 1));
    };
};
