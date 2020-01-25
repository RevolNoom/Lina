#include "fraction.h"
#include <iostream>
#include <iomanip>
#include "matrix.h"
#include "block.h"
 

int main()
{   
    Matrix<Fraction> b(2, 2);

    b.at(0, 0)=1;
    b.at(0, 1)=2;
    b.at(1, 0)=3;
    b.at(1, 1)=4;

/*
    b.at(0, 0)=1;
    b.at(0, 1)=1;
    b.at(0, 2)=1;
    b.at(0, 3)=1;
    b.at(0, 4)=1;
    b.at(1, 1)=1;
    b.at(1, 2)=1;
    b.at(1, 3)=1;
    b.at(1, 4)=1;
    b.at(2, 2)=1;
    b.at(2, 3)=1;
    b.at(2, 4)=1;
    b.at(3, 3)=1;
    b.at(3, 4)=1;
    b.at(4, 4)=1;
  */  


    std::cout<<"b: \n"<<b;
    std::cout<<"inverse: \n";
    auto b1=b.Inverse();

    std::cout<<"Now, for the grand finale: b * b.Inverse():\n"<<b*b1;

    return 0;   
}

