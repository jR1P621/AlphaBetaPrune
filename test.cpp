#include <iostream>
#include "src/Board.cpp"
#include "src/Bot.cpp"

using std::cin;

int main(int argc, char const *argv[])
{
    Board myBoard = Board();
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
