#include "lina.h"

//Some helper functions, for, uhhh..., faster and cleaner coding
std::ostream& operator<<(std::ostream& os, const Lina_Operand &Op)
{
    std::visit([&](auto &whatever)
    {
        os<<whatever;
    }, Op);
    return os;
}

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

//Some helper functions, for, uhhh..., faster and cleaner coding
Lina_Operand operator-(const Lina_Operand &Op1, const Lina_Operand &Op2)
{
    Lina_Operand result;
    std::visit([Op1, Op2, &result](const auto & Frac_Or_Matrix_1)
                {
                    std::visit([Op1, Op2, &Frac_Or_Matrix_1, &result](const auto &Frac_Or_Matrix_2)
                    {
                        result = Frac_Or_Matrix_1 - Frac_Or_Matrix_2;
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

//Some helper functions, for, uhhh..., faster and cleaner coding
Lina_Operand operator/(const Lina_Operand &Op1, const Lina_Operand &Op2)
{
    Lina_Operand result;
    std::visit([Op1, Op2, &result](const auto & Frac_Or_Matrix_1)
                {
                    result = Frac_Or_Matrix_1 / std::get<Fraction>(Op2);
                },
                Op1);
    return result;
}

std::variant<Fraction, Matrix<Fraction>> Lina::Calculate(const std::string &Expression) const
{
    int state=Verify_Parentheses_Expression(Expression);
    if (state!=1)
    {
        std::string error="Expression has invalid parentheses placing.\n";
        if (state==-1)
            error+="Maybe you forget to wrap the number in () after ^ ?\n";

        throw (Mexception(error));
    }
    std::vector<Lina_Operand> Operands{Fraction(0)};
    std::vector<char> Operator{'+'};

    // Calculate until the precedence level reach the given one
    auto Pop_The_Stack= [&](int Precedence_Level=0) -> void
                        {
                            while (Operands.size()>1
                                    && 
                                    OperatorPrecedence(Operator.back())
                                                >Precedence_Level)
                            {
                                Lina_Operand Right_Op = Operands.back();
                                Operands.pop_back();
                                switch(Operator.back())
                                {
                                    case '+':
                                        Operands.back() = Operands.back() + Right_Op;
                                        break;
                                    case '-':
                                        Operands.back() = Operands.back() - Right_Op;
                                        break;
                                    case '*':
                                        Operands.back() = Operands.back() * Right_Op;
                                        break;
                                    case '^':
                                        //std::cout<<"Calculating "<<Right_Op<<" and "<<std::get<Fraction>(Operands.back());
                                        if (!std::holds_alternative<Fraction>(Right_Op) &&
                                            !std::get<Fraction>(Right_Op).IsInteger())
                                                throw (Mexception("Power operation must take an integer."));
                                        Operands.back() = Pow(Operands.back(), 
                                                    std::get<Fraction>(Right_Op).ToDouble());

                                        break;
                                }
                                Operator.pop_back();
                            }
                        };
    
    bool Expecting_Operand(true);

    for (int iii=0; iii<Expression.size(); ++iii)
    {
        if (Expecting_Operand)
        {

            Expecting_Operand=false;

            switch(Expression[iii])            
            {
                case '(':
                {
                    int Matching_Par= FindMatchingParentheses(
                                        Expression, iii, 
                                        Expression.size());
                    Operands.push_back(
                            Calculate(Expression.
                                            substr(iii+1,
                                                        Matching_Par - iii-1)));
                    iii=Matching_Par;
                    break;
                }
                //    We are grabbing a whole lot of prefix +-+-
                case '+':
                case '-':
                    if (Operator.back()!='+' 
                        &&
                        Operator.back()!='-')
                        Operator.push_back(Expression[iii]);
                    else 
                        if (Expression[iii]=='-')
                            Operator.back() = (Operator.back()=='+'?
                                                    '-' : '+');
                    break;

                default:
                    if (!std::isspace(Expression[iii]))
                    {
                        if (std::isalnum(Expression[iii]))
                        {
                            int start(iii);
                            while (std::isalnum(Expression[iii])&&
                                    iii<Expression.size())
                            {
                                ++iii;
                            }
                            std::string Name = Expression.substr(start, iii-start);
                            //Put iii back one place to prepare for the next loop
                            --iii;
                            

                            if (IsInteger(Name))
                            {
                                Fraction F(std::stoi(Name));
                                Operands.push_back(F);
                            }
                            else if (_Matrices.find(Name) != _Matrices.end())
                            {
                                Operands.push_back(_Matrices.at(Name));
                            }
                            else 
                                throw (Mexception(Name +
                                                    " is neither a number or matrix."));
                        }
                    }
                    else 
                                throw (Mexception(Expression.substr(iii) + " is neither a number or matrix."));
            }
        }
        else    //Expecting Operator
        {
            Expecting_Operand=true;

            while (std::isspace(Expression[iii]) && 
                    iii<Expression.size())
                ++iii;
            
            //Test to see is it an operator
            if (OperatorPrecedence(Expression[iii]))
            {
                //Since operator ^ yields different results 
                //  when we calculate from different directions
                //So, prioritize calculating it first
                
                Pop_The_Stack(OperatorPrecedence(Expression[iii]));
                Operator.push_back(Expression[iii]);
            }
            else    //No operator found
                throw(Mexception(Expression.substr(0, iii) +
                            " <Lack an operator> " + 
                            Expression.substr(iii)));
        }
    }
    Pop_The_Stack();

    //std::cout<<"\n";
    return Operands.back();
}

int OperatorPrecedence(char op)
{
    switch (op)
    {
        case '+':
        case '-':
            return 1;
        
        case '*':
        case '/':
            return 2;

        case '^':
            return 3;
        
        default:
            return 0;
    }
}
/*
    Test the Expression to see if it has the following properties:
    - All Parentheses pairs are valid.
    - After every ^ operator, there's a following pair of parentheses
*/
int Verify_Parentheses_Expression(const std::string &Expression)
{
    int Count_Par(0);
    for (int iii=0; iii<Expression.size(); ++iii)
    {
        switch(Expression[iii])
        {
            case '(':
                ++Count_Par;
                break;
            case ')':
                --Count_Par;
                if (Count_Par<0)
                    return false;
                break;
            
            case '^':
                while (std::isspace(Expression[++iii]) && 
                        iii<Expression.size())
                        {}

                if (Expression[iii] != '(')
                    return -1;
                
                ++Count_Par;
                break;
        }
    }
    return Count_Par==0? true : false;
}
