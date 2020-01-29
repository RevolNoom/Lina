/*
    AUXILIARY LINA COMPONENT

    This file contains functions that's not features of Lina
    But rather the functions to help writing Lina easier
*/
#include "lina.h"

bool Lina::IsValidMatrixName(std::string name)
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
        if (!std::isalnum(c) || c!='_')
            return false;

    /*
        And it must not collide with other keyword/matrix name
    */
    if (_Matrices.find(name)!=_Matrices.end() ||
        _Commands.find(name)!=_Commands.end())
        return false;
    
    return true;
}