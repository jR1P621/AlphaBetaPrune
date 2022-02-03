#include <iostream>
#include "src/Checkers.h"

using std::cin;

int main(int argc, char const *argv[])
{
    Checkers myBoard = Checkers();
    int selection;
    // do
    // {
    //     myBoard.print();
    //     cout << "Select Pit: ";
    //     cin >> selection;
    //     try
    //     {
    //         myBoard.makeMove(selection);
    //     }
    //     catch (invalid_argument e)
    //     {
    //         printf("%s\n", e.what());
    //     }
    // } while (myBoard.getWinner() < 0);
    myBoard.print();
}
