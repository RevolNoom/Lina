#include "fraction.h"
#include <iostream>
#include <iomanip>
#include "matrix.h"
#include "block.h"
#include "lina.h"

enum Board
{
    DAYS,
    COST,
    SPENT,
    WINNING,
    PROFIT,
    MAX_COLUMN
};

int main()
{   
    /*
    std::cout<<"Days: ";
    long long days;
    std::cin>>days;

    Matrix<std::string> board(days+1, 5);
    board.at(0, DAYS)="Days";
    board.at(0, COST)="Cost";
    board.at(0, SPENT)="Total Spent";
    board.at(0, WINNING)="Winning";
    board.at(0, PROFIT)="Profit";

    long long winningRate=3;
    long long temp1=0;
    long long temp2=0;
    for (long long iii=1, cost=1, totalSpent=0; iii<=days; ++iii)
    {
        temp1 = temp2;
        temp2 = cost;
        cost = temp1 + temp2;
        totalSpent+=cost;
        //cost = (totalSpent+winningRate)/winningRate;

        board.at(iii, DAYS)= std::to_string(iii);
        board.at(iii, COST)= std::to_string(cost);
        board.at(iii, SPENT)= std::to_string(totalSpent);
        board.at(iii, WINNING)= std::to_string(cost*winningRate);
        board.at(iii, PROFIT)= std::to_string(double(cost*winningRate-totalSpent)/totalSpent);
        
    }

    std::cout<<board;
*/

    Lina lina;
    lina.Start();
    
    
    return 0;   
}

