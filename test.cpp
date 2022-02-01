#include <iostream>
#include "src/Mancala.cpp"

using std::cin;

int main(int argc, char const *argv[])
{
    Mancala myBoard = Mancala();
    int selection;
    do
    {
        myBoard.print();
        cout << "Select Pit: ";
        cin >> selection;
        try
        {
            myBoard.makeMove(selection);
        }
        catch (invalid_argument e)
        {
            printf("%s\n", e.what());
        }
    } while (myBoard.getWinner() < 0);
    myBoard.print();
}
