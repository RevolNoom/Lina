/*
    AUXILIARY LINA COMPONENT

    This file contains functions that's not features of Lina
    But rather the functions to help writing Lina easier
*/
#include "lina.h"

bool Lina::IsValidMatrixName(std::string name) const
{
    /*
        A name must have at least a character
        And its first character must not be a digit
    */
    if (name.size()==0          || 
        std::isdigit(name.front()))
        return false;

    /*
        A name include only _underscores, digits, and latin characters
    */
    for (auto &c: name)
        if (!(std::isalnum(c) || c=='_'))
            return false;

    /*
        And it must not collide with other keyword/matrix name
    */
    if (_Matrices.find(name)!=_Matrices.end() ||
        _Commands.find(name)!=_Commands.end())
        return false;
    
    return true;
}

bool Lina::IsMatrix(std::string name) const
{
    return _Matrices.find(name)!=_Matrices.end();
}

bool Lina::IsCommand(std::string name) const
{
    return _Commands.find(name)!=_Commands.end();
}

bool Lina::IsKeyword(std::string name) const
{
    return _Keywords.find(name)!=_Keywords.end();
}

/*
    @brief  Look for the leftmost opening parentheses, then find its corresponding closing 
    @param  The string in need to find the parentheses
    @return Index of the closing parenthesis of the leftmost opening parenthesis
            -1 if there's no valid parentheses group
*/
size_t FindMatchingParentheses(const std::string &str)
{
    int result(-1);
    for (int iii=0; iii<str.size(); ++iii)
        if (str[iii]=='(')
        {
            result=iii;
            break;
        }

    if (result==-1)
        return -1;
    
    //Counter checks if the right ')' is found or not
    int counter=0;
    for (int iii=result; iii<str.size(); ++iii)
    {
        if (str[iii]=='(')
            ++counter;
        else if (str[iii]==')')
            --counter;
        if (counter==0)
            return iii;
    }

    return -1;
}



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

            
            default:
                if (!std::isspace(Expression[pos]))
                    break;
        }

    
   // There's no operator here, so this's the end of the Expression
   return Current_Matrix;
}


    /*
Matrix<Fraction> Lina::Calculate(std::string &Expression, size_t begin, size_t end)
{
    //There might be +-+-+- in front of the matrix name
    //So we grab them all and switch the sign of the matrix accordingly
    int sign=1;
    int index=begin;

    for (;index<end; ++index)
    {
        //A plus and an innocent space
        if (Expression[index]=='+' ||
            std::isspace(Expression[index]))
                continue;
        //A devilish minus sign
        else if (Expression[index]=='-')
            sign*=-1;

        //Find the whole matching pair of parentheses
        else if (Expression[index]=='(')
        {
            int endGroup=FindMatchingParentheses(Expression.substr(index));
            return Fraction(sign) * 
                    Calculate(Expression, index+1, endGroup);
        }
            //We can't find a matching brace
            //Oh well. Invalid input
            throw(Mexception("Unmatching brace in: \"" + Expression));
        }
        //This is the beginning of a matrix name
        else 
            break;
    }
    
    //Just grab the matrix name,
    //If it's invalid
    //then it wouldn't have happened to exist in the first place

    //>>>>>>>>>>>> FUTURE UPDATE <<<<<<<<<<<<<
    //If it's a number, then we are in big trouble
    std::string MatrixName;
    while (index<end &&
            (Expression[index]=='_' ||
             std::isalnum(Expression[index])
            )
          )
          MatrixName+=Expression[index];
    
    //Test for matrix validity
    if (_Matrices.find(MatrixName)==_Matrices.end())
        throw("Matrix \"" + MatrixName + "\" does not exist. Please try again.");        
    /*
        Now grab the operator. It may be 
        +) Addition (+)
        +) Subtraction (-)
        +) Multiplication (*)
        +) Parentheses ()
        
        or nothing. In which case we return the matrix itself
    */
    /*
    for (;index<end; ++index)
    {
        if (std::isspace(Expression[index]))
            continue;

        switch(Expression[index])
        {
            case '+':
            case '-':
                return Fraction(sign) * _Matrices[MatrixName] +
                            (Expression[index]=='+'? 
                                Calculate(Expression, index+1, end)   :
                                    -Calculate(Expression, index+1, end));

            /*
                Multiplication case is hard for reasons

                A*B + C             (1)
                A*B*(C + (D+E)) * F     (2)

                In the first case, if we return recursively like the case of + and -, 
                the result would be A*(B+C), not as we expect it to be A*B + C
                So we have to find the multiplying group (in this case, A*B)

                In the second case, 
                we must bear in mind that 
                there may be parentheses when doing search for multiplying group

                Parentheses complicate things up incredibly fast 
                because we have to check for braces correspondence
            */
           /*
            case '*':
                //groupEnd remembers the index at which a multiplying group ends
                int groupEnd(index);
                //Endmark checks to see if we have grabbed operator + or -
                //
                bool endMark(false);
                for (; groupEnd<end; ++groupEnd)
                {
                    if (std::isspace(Expression[groupEnd]))
                        continue;
                    
                    switch(Expression[groupEnd])
                    {
                        case '(':
                            FindMatchingParentheses)(;
                            break;
                        
                        case '-':
                        case '+':
                            return _Matrices[MatrixName] * 
                                    Calculate(Expression, index, groupEnd) +
                                        Calculate(Expression, groupEnd, end);
                    }


                    //There's no + or - after the multiplying group
                    //So the group spans until the end of this Expression
                    return _Matrices[MatrixName] * 
                            Calculate(Expression, index, end);
                }
    }

}

    /*
        Nothing interesting happens here
        (Only spaces are found after matrix name)
    */
    // return _Matrices[MatrixName];
//}


