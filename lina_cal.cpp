#include "lina.h"

//Some helper functions, for, uhhh..., faster and cleaner coding
Lina_Operand operator+(const Lina_Operand &Op1, const Lina_Operand &Op2)
{
    Lina_Operand result;
    std::visit([Op1, Op2, &result](const auto & Frac_Or_Matrix_1)
                {
                    std::visit([Op1, Op2, &Frac_Or_Matrix_1, &result](const auto &Frac_Or_Matrix_2)
                    {
                        result = Frac_Or_Matrix_1 + Frac_Or_Matrix_2;
                    }
                    , Op2);
                },
                Op1);
    return result;
}

Lina_Operand operator*(const Lina_Operand &Op1, const Lina_Operand &Op2)
{
    Lina_Operand result;
    std::visit([Op1, Op2, &result](const auto & Frac_Or_Matrix_1)
                {
                    std::visit([Op1, Op2, &Frac_Or_Matrix_1, &result](const auto &Frac_Or_Matrix_2)
                    {
                        result = Frac_Or_Matrix_1 * Frac_Or_Matrix_2;
                    }
                    , Op2);
                },
                Op1);
    return result;
}

std::variant<Fraction, Matrix<Fraction>> Lina::Calculate(const std::string &Expression) const
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
    std::variant<Fraction, Matrix<Fraction>> Current_Result;

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

        Current_Result= Calculate(Expression.substr(pos+1, endGroup-pos-1));
        
        //After calculates the group, we set the current examining position to after it
        pos=endGroup+1;
    }
    /*
        Or there's no parentheses at all!
        In that case, what we want to look for is the matrix name our user gives
    */
    else
    {
        //Finding the next Operand
        std::string Matrix_Name;
        int start=pos;
        for (;pos<Expression.size();++pos)
        {
            if (!std::isalnum(Expression[pos]) &&
                Expression[pos]!='_')
                break;
        }

        Matrix_Name= Expression.substr(start, pos-start);

        /*
            Evaluate and assign Current_Result with the suitable value
                (either a Matrix, 
                    or a Fraction (which is converted from an int))
        */
        if (_Matrices.find(Matrix_Name)!=_Matrices.end())
        {
            Current_Result=_Matrices.at(Matrix_Name);
        }
        else if (IsInteger(Matrix_Name))
        {
            Current_Result=std::stoi(Matrix_Name);
        }    
        else 
            throw(Mexception("Matrix " + Matrix_Name + " not found"));
    }

    /*
        Adding std::variant does mess up a lot
        This expression multiply our current result with the sign 
        we gobbled at the beginning of the function
    */
    std::visit([&sign, &Current_Result](auto &Whether_Matrix_Or_Fraction)
                {
                    Whether_Matrix_Or_Fraction*=Fraction(sign);
                }
                ,Current_Result);

    //std::cout<<"Expression \""<<Expression<<"\": Current_Matrix: \n"<<Current_Matrix<<"\n";


    //With the matrix name, we can do many operators with it
    //Let's start with + and -
    for (; pos<Expression.size(); ++pos)
        switch(Expression[pos])
        {
            case '+':
            case '-':
                //I still use operator+ here
                //  because in future evaluations,
                //  the minus sign will be multiplied with 
                //  its own variable "sign" above
                return Current_Result
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
                            return Current_Result * Calculate(Expression.substr(pos, endGroup-pos))
                                     + Calculate(Expression.substr(endGroup));

                        case '(':
                            endGroup += FindMatchingParentheses(Expression.substr(endGroup));
                            if (endGroup==-1)
                                throw(Mexception("Expression \"" + Expression + "\" has unmatching brace"));
                    }
                return Current_Result * Calculate(Expression.substr(pos));
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
   return Current_Result;   
}


/*  T O  B E  U P D A T E D

std::variant<Matrix<Fraction>, Fraction> Lina::RealCalculate(const std::string &Expression, size_t begin, size_t end) const
{
    //These codes will be swapped to inside Calculate()
    //And what's in Calculate() will end up in here
    auto Result = RealCalculate(Expression, 0, Expression.size());
    if (std::holds_alternative<Matrix<Fraction>>(Result))
        return std::get<Matrix<Fraction>>(Result);
} 
*/