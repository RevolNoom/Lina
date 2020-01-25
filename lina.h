/*
    LINA - The ultimate class that is the heart of the application

    +)I/O operations
    +)Matrices allocation
    +)Exceptions handling
    +)Keywords managing
    +)... and many more
*/

#ifndef LINA_H
#define LINA_H

#include <unordered_map>
#include <functional>
#include <list>
#include <fstream>
#include "matrix.h"
#include "language.h"
#include <regex>

class Lina
{
private:
    std::unordered_map<std::string, Matrix<Fraction>> _Matrices;

    //Map a command with a corresponding value
    std::unordered_map<std::string, std::function<int()>> _Commands;

    //The name of the language file
    std::string _CurrentLanguage;
    
    //An array to keep the language pack for use
    std::vector<std::string> _Language;

    //A string used to save the command given by the user
    //This command is then shared between function calls
    std::string _Buffer;

public:

    /*
        On creation, initialize the hash maps that map
        the input to commands.
    */
   
    Lina();
    
    int Start();
    
    //int Intro();

    int ChangeLanguage();

    int Help();

    /*
        Create a matrix with a given size
        The input to create a matrix conforms to:

        CREATE <NAME> [ROWS [x] COLUMNS]

        !!!NOTE!!!: If no argument is given to the size of the matrix
        then the matrix will have size 0x0

        For example: 
            Matrix 3x4
            Matrix 3 x 4
            Matrix 3 4
            Matrix
    */
    int Create();

    int Calculate();

    int GetCommand();

    int Exit();

protected:
    /*
        Test a name to see whether it's fit to be a matrix name
        A name must satisfy these condition to be allowed:
        +) It includes ONLY:
            -)Latin characters (a-z, A-Z)
            -)Digits (0-9)
            -)Underscore _
        +) Does not begin with a digit
        +) It does not collide with a KEYWORD, or name of an Already Existing Matrix
    */
    bool IsValidName(std::string name);


};

#endif  /* LINA_H */