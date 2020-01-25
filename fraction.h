#ifndef FRACTION_H
#define FRACTION_H

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include "mexception.h"


class Fraction
{
private:
    long long _Numerator;
    long long _Denominator;


public:
/*
--------------------
--------------------
||||CONSTRUCTORS||||
--------------------
--------------------
*/
//EXCEPTIONS WILL BE THROWN WHEN A FRACTION HAS A DENOMINATOR=0
    Fraction() noexcept;
    Fraction(long long integer) noexcept;
    Fraction(long long Num, long long Denom);
    Fraction(const Fraction& f);

/*
------------------------------------------------
------------------------------------------------
||||THESE FUNCTIONS ARE FOR YOU TO WORK WITH||||
------------------------------------------------
------------------------------------------------
*/
//GETTERS
    //Get the Numerator
    long long Numerator();
    //Get the Denominator
    long long Denominator();

    //Return true if the numerator divides evenly with the denominator
    bool IsInteger() const;

    //Return the value in real number
    double ToDouble() const;



    /* I/O  O P E R A T I O N S */

    //Get the fraction from input
    friend std::istream& operator>>(std::istream& is, Fraction& f);

    //Write the fraction to the console
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f);

    //Return the number of characters this fraction will take to be represented
    long long WriteLength() const;

/*
    TO BE IMPLEMENTED LATER

    //Raise this fraction to the power of n
    Fraction pow(Fraction f, long long n);
*/


    
    /*
    LATER IMPLEMENTED FOR DOUBLE COMPATIBILITY
    //And also implicit cast to real
    operator double();
    */

    

/*
---------------------------------
---------------------------------
||||||ARITHMETIC OPERATIONS||||||
---------------------------------
---------------------------------
*/
    //Unary operator for fractions:
    friend Fraction operator+(Fraction f);
    friend Fraction operator-(Fraction f);
    
    //Perform elementary operations on this fraction
    void operator+=(Fraction f);
    void operator-=(Fraction f);
    void operator*=(Fraction f);
    void operator/=(Fraction f);

//ELEMENTARY OPERATIONS WITH INTEGERS AND FRACTIONS
    //Fractions
    friend Fraction operator+(Fraction a, Fraction b);
    friend Fraction operator-(Fraction a, Fraction b);
    friend Fraction operator*(Fraction a, Fraction b);
    friend Fraction operator/(Fraction a, Fraction b);
    
    friend bool operator==   (Fraction a, Fraction b);
    friend bool operator!=   (Fraction a, Fraction b);
    friend bool operator>    (Fraction a, Fraction b);
    friend bool operator>=   (Fraction a, Fraction b);
    friend bool operator<    (Fraction a, Fraction b);
    friend bool operator<=   (Fraction a, Fraction b);
    
    
    friend Fraction operator+(long long a, Fraction f);
    friend Fraction operator-(long long a, Fraction f);
    friend Fraction operator*(long long a, Fraction f);
    friend Fraction operator/(long long a, Fraction f);

    friend bool operator==   (long long a, Fraction f);
    friend bool operator!=   (long long a, Fraction f);
    friend bool operator>    (long long a, Fraction b);
    friend bool operator>=   (long long a, Fraction b);
    friend bool operator<    (long long a, Fraction b);
    friend bool operator<=   (long long a, Fraction b);
/*
    LATER IMPLEMENTED FOR DOUBLE COMPATIBILITY

    friend bool operator==   (double a, Fraction f);
    friend bool operator!=   (double a, Fraction f);
    friend bool operator>    (double a, Fraction b);
    friend bool operator>=   (double a, Fraction b);
    friend bool operator<    (long long a, Fraction b);
    friend bool operator<=   (long long a, Fraction b);
*/
/*
--------------------------------------------
--------------------------------------------
||||THESE FUNCTIONS ARE USED INTERNALLY ||||
||||TO  KEEP  THE  CLASS  FROM  BREAKING||||
--------------------------------------------
--------------------------------------------
*/
private:
    /*
        Automatically called after any functions
        that may mutates the Fraction
    
        Turn this Fraction into a reduced one
    */
    void Reduce();
};

#endif /* FRACTION_H */