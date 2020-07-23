#include "fraction.h"
#include <iostream>
#include <iomanip>
#include "matrix.h"
#include "block.h"
#include "lina.h"


int main()
{   
    Lina lina;
    lina.Start();
    
    /*
    Matrix<Fraction> A(2, 2);
    A.at(0, 0)= 1;
    A.at(0, 1)= 2;
    A.at(1, 0)= 3;
    A.at(1, 1)= 4;
    lina._Matrices.insert({"A", A});

    Matrix<Fraction> B(2, 2);
    B.at(0, 0)= 5;
    B.at(0, 1)= 6;
    B.at(1, 0)= 7;
    B.at(1, 1)= 8;
    lina._Matrices.insert({"B", B});

    //auto result=lina.Calculate("(A+B)^(2)");
    auto result=lina.Calculate("(A+B)");

    if (std::holds_alternative<Fraction>(result))
        std::cout<<std::get<Fraction>(result);
    else
        std::cout<<std::get<Matrix<Fraction>>(result);
    std::cout<<"\n";

    auto _result=lina.Calculate("A-B");

    if (std::holds_alternative<Fraction>(_result))
        std::cout<<std::get<Fraction>(_result);
    else
        std::cout<<std::get<Matrix<Fraction>>(_result);
    std::cout<<"\n";
    */
    return 0;   
}

