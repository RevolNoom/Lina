#include "fraction.h"

/*
---------------------------------
---------------------------------
||||||||||CONSTRUCTORS|||||||||||
---------------------------------
---------------------------------
*/
Fraction::Fraction() noexcept: _Numerator(0), _Denominator(1)
{
}

Fraction::Fraction(long long Num, long long Denom): _Numerator(Num), _Denominator(Denom)
{
    if (_Denominator==0)
    {
        throw (Mexception("Zero denominator fraction"));
    }
    Reduce();
}

Fraction::Fraction(long long integer) noexcept: Fraction(integer, 1)
{
}

Fraction::Fraction(const Fraction& f)
{
    *this=f;
}


/*
------------------------------------------------
------------------------------------------------
||||THESE FUNCTIONS ARE FOR YOU TO WORK WITH||||
------------------------------------------------
------------------------------------------------
*/

/* G E T T E R S */

//Get the Numerator
long long Fraction::Numerator()
{
    return _Numerator;
}

//Get the Denominator
long long Fraction::Denominator()
{
    return _Denominator;
}

//Get the value in double
double Fraction::ToDouble() const
{
    return double(_Numerator)/_Denominator;
}

/*
Fraction::operator double()
{
    return ToDouble();
}*/

bool Fraction::IsInteger() const
{
    return _Denominator==1;
}

/* I/O  O P E R A T I O N S */




std::ostream& operator<<(std::ostream& os, const Fraction& f)
{
    if (f.IsInteger()) 
        os<< (f._Numerator);
    else
        os<<f._Numerator<<'/'<<f._Denominator;
    return os;
}

long long Fraction::WriteLength() const
{
    return std::to_string(_Numerator).length() + 
            (IsInteger()? 
                            0   :
                                    std::to_string(_Denominator).length() + 1);   
                                    //1 extra character for the slash "/"
}

/*
---------------------------
---------------------------
|||| F U N C T I O N S ||||
---------------------------
---------------------------
*/
void Fraction::Reduce()
{
    long long div=std::__gcd(abs(_Numerator), abs(_Denominator));
    if (_Denominator<0)
        div*=-1;
    _Numerator/=div;
    _Denominator/=div;
}


/*
---------------------------------
---------------------------------
||||||ARITHMETIC OPERATIONS||||||
---------------------------------
---------------------------------
*/
Fraction operator+(Fraction f)
{
    return f;
}

Fraction operator-(Fraction f)
{
    return Fraction(-f._Numerator, f._Denominator);
}

void Fraction::operator+=(Fraction a)
{
    _Numerator= _Numerator*a._Denominator + a._Numerator * _Denominator;
    _Denominator*=a._Denominator;
    Reduce();
}


void Fraction::operator-=(Fraction f)
{
    *this += -f;
}


void Fraction::operator*=(Fraction f)
{
    _Numerator *= f._Numerator;
    _Denominator *= f._Denominator;
    Reduce();
}

void Fraction::operator/=(Fraction f)
{
    if (f==0)
    {
        throw(Mexception("Fraction division for zero"));
    }
    _Numerator*= f._Denominator;
    _Denominator*= f._Numerator;
    Reduce();
}


bool operator==(Fraction a, Fraction b)
{
    //It's assumed that a and b are reduced
    return a._Numerator==b._Numerator &&
           a._Denominator==b._Denominator;
}

bool operator!=(Fraction a, Fraction b)
{
    return !(a==b);
}

Fraction operator+(Fraction a, Fraction b)
{
    a+=b;
    return a;
}

Fraction operator-(Fraction a, Fraction b)
{
    a-=b;
    return a;
}

Fraction operator*(Fraction a, Fraction b)
{
    a*=b;
    return a;
}

Fraction operator/(Fraction a, Fraction b)
{
    a/=b;
    return a;
}


bool operator>(Fraction a, Fraction b)
{
    return a._Numerator * b._Denominator >
            a._Denominator * b._Numerator;
}

bool operator>=(Fraction a, Fraction b)
{
    return a._Numerator * b._Denominator >=
            a._Denominator * b._Numerator;
}

bool operator<(Fraction a, Fraction b)
{
    return b>a;
}

bool operator<=(Fraction a, Fraction b)
{
    return b>=a;
}


Fraction operator+(long long a, Fraction b)
{
    return b+a;
}

Fraction operator-(long long a, Fraction b)
{
    return b-a;
}

Fraction operator*(long long a, Fraction b)
{
    return Fraction(a)*b;
}

Fraction operator/(long long a, Fraction b)
{
    return Fraction(a)/b;
}

bool operator!=(long long a, Fraction f)
{
    return Fraction(a)!=f;
}

bool operator>(long long a, Fraction f)
{
    return Fraction(a)>f;
}

bool operator>=(long long a, Fraction f)
{
    return Fraction(a)>=f;
}

bool operator<(long long a, Fraction f)
{
    return f>a;
}

bool operator<=(long long a, Fraction f)
{
    return f>=a;
}


/*
    Grab a fraction in the following format:

    +) A single integer
        E.g.:   1 10 20 30
    +) Two integers separated by a slash "/".
        They must be "glued" to the slash
        E.g:    1/1     30/7    1000/50
            
        FALSE FORMAT:   1 /2    2/ 3    2 / 3
*/
std::istream& operator>>(std::istream& is, Fraction& f)
{   
    if (!(is>>f._Numerator))
    {
        is.clear();
        is.ignore(32767, '\n');
        throw(Mexception("Fraction numerator input failed"));
    }
    if (is.peek()=='/') 
    {    
        is.ignore(1);
        if (!std::isdigit(is.peek()))
        {
            f._Numerator=0;
            throw(Mexception("Fraction denominator input failed"));
        }
        is>>f._Denominator;
    }
    return is;
}