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
        _Utility.find(name)!=_Utility.end())
        return false;
    
    return true;
}

bool Lina::IsMatrix(std::string name) const
{
    return _Matrices.find(name)!=_Matrices.end();
}

bool Lina::IsCommand(std::string name) const
{
    return _Utility.find(name)!=_Utility.end();
}

bool Lina::IsKeyword(std::string name) const
{
    return _Keywords.find(name)!=_Keywords.end();
}


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


std::vector<std::string> BreakExpressions(const std::string &Expression, int (*delimPred)(int c))
{
    std::vector<std::string> result;
    int begin(0);
    int current(0);
    for (; current<Expression.size(); ++current)
        if (delimPred(Expression[current]))
        {
            if (current-begin>0)
                result.push_back(Expression.substr(begin, current-begin));

            begin=current+1;
        }

    //A last loop to test for the forgotten command
    if (current-begin>0)
        result.push_back(Expression.substr(begin, current-begin));

    //Trim whitespace
    for (auto &s: result)
        s= TrimWhiteSpace(s);
    return result;
}

bool IsInteger(const std::string &str)
{
    //Ignore the whitespace at the front and back of the string
    int front(0), back(str.size()-1);
    while (std::isspace(str[front]) && front<str.size())
        ++front;

    while (std::isspace(str[back]) && back>=0 )
        --back;
    
    for (; front<=back; ++front)
        if (!std::isdigit(str[front]))
            return false;

    return true;
}
std::string TrimWhiteSpace(const std::string &str)
{
    int begin(0), end(str.size()-1);
    while (end>=begin && 
            std::isspace(str[end]))
        --end;

    while (begin<=end && 
           std::isspace(str[begin]))
        ++begin;

    return str.substr(begin, end-begin+1);
}

int IsComma(int c)
{
    return c==',';
}