#include "lina.h"

Matrix<Fraction> Lina::Calculate(const std::string &Expression) const
{
    //Grab all + and - at the Expression beginning
    int sign(1);
    int pos(0);

    for (; pos<Expression.size(); ++pos)
    {
        int letter=Expression[pos];

        if (std::isspace(letter) ||
                letter=='+')
            continue;
        else if (letter=='-')
            sign*=-1;
        else 
            break;
    }
    
    // All the +-+-+- are gone
    // Now it's time to create a temporary matrix to remember the matrix of this call
    Matrix<Fraction> Current_Matrix;

    // Bad news! We've got the parentheses!
    if (Expression[pos]=='(')
    {
        /*
            What we're gonna do
            is to not care what's in the parentheses group at all!

            We'll just calculate that, and then return it to the outside world
        */
        int endGroup= FindMatchingParentheses(Expression);

        //An invalid Expression (No matching closing parenthesis). How sad.
        if (endGroup==-1)
            throw(Mexception("No matching parenthesis: " + Expression));

        Current_Matrix= Calculate(Expression.substr(pos+1, endGroup-pos-1));
        
        //After calculates the group, we set the current examining position to after it
        pos=endGroup+1;
    }
    /*
        Or there's no parentheses at all!
        In that case, what we want to look for is the matrix name our user gives
    */
    else
    {
        std::string Matrix_Name;
        int start=pos;
        for (;pos<Expression.size();++pos)
        {
            if (!std::isalnum(Expression[pos]) &&
                Expression[pos]!='_')
                break;
        }

        Matrix_Name= Expression.substr(start, pos-start);

        if (_Matrices.find(Matrix_Name)==_Matrices.end())
            throw(Mexception("Matrix " + Matrix_Name + " not found"));
        
        Current_Matrix=_Matrices.at(Matrix_Name);
    }

    Current_Matrix*=Fraction(sign);
    //std::cout<<"Expression \""<<Expression<<"\": Current_Matrix: \n"<<Current_Matrix<<"\n";


    //With the matrix name, we can do many operators with it
    //Let's start with + and -
    for (; pos<Expression.size(); ++pos)
        switch(Expression[pos])
        {
            case '+':
            case '-':
                return Current_Matrix
                            + Calculate(Expression.substr(pos));
            
            /*
                Multiplication is a huge, huge mess
                Because * must be performed before + -
                So we have to extract each groups of multipliers
            */
            case '*':
            {
                // For convenience, increment pos to ignore the current '*' character
                ++pos;
                int endGroup=pos;
                /*
                    Extraction of multipliers groups is performed as follow:
                    +) Iterate sequencially through the expression
                        Do not bother unless the current character is '(', '+' or '-'
                        Why? Because:
                    +) '*' has higher precedence than '+' and '-'.
                        So '*' is performed before them
                        Then '+' and '-' mark the end of a multiplier group
                    +) But a multiplier may be a combination of sum and subtraction 
                        via parentheses '()'. So we keep a look out for '('
                    +) If the current character is '(', quickly find its matching brace
                        and then move on as if 
                        the whole monster inside the parentheses is just a multiplier
                    +) Finally, if we don't find any + or -
                        that means we have reached the end of the expression
                */
                for (;endGroup<Expression.size(); ++endGroup)
                    switch (Expression[endGroup])
                    {
                        case '+':
                        case '-':
                            return Current_Matrix * Calculate(Expression.substr(pos, endGroup-pos))
                                     + Calculate(Expression.substr(endGroup));

                        case '(':
                            endGroup += FindMatchingParentheses(Expression.substr(endGroup));
                            if (endGroup==-1)
                                throw(Mexception("Expression \"" + Expression + "\" has unmatching brace"));
                    }
                return Current_Matrix * Calculate(Expression.substr(pos));
            }

            /*
                Another hassle!
                Calculate power of a matrix. It may be inverse
            case '^':
            {

            }

            */
            /*
                Now there's a leakage in our algorithm
                There might be cases where between two matrices,
                no operator is applied to them at all!

                That would make the expression invalid
                and we must then treat it like one
            */
            default:
                if (std::isalnum(Expression[pos]) || 
                    Expression[pos]=='_')
                    throw (Mexception("There's a missing operator in this expression: " + Expression));
        }

    
   // There's no operator here, so this's the end of the Expression
   return Current_Matrix;
}

