/*
    LINA - The ultimate class that is the heart of the application

    +)I/O operations
    +)Matrices allocation
    +)Exceptions handling
    +)Keywords managing
    +)... and many more
*/
/*
    Future updates:
    +) Create more than one matrix with keyword Create
    +) Keyword Name: used with show to show only matrices names

    +) Change() to change more matrices at a time and change language
                Change matrix size

    +) Fraction input format 

    +) Calculate() remade with inclusion of identity matrix
                   can distinguish many expressions inside one string
    +) GetCommand(): Calculating matrices will find the assignment sign anywhere in the arguments
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
#include <unordered_set>


class Lina
{
private:

    std::unordered_set<std::string> _Keywords;

    std::unordered_map<std::string, Matrix<Fraction>> _Matrices;

    //Map a command with a corresponding value
    std::unordered_map<std::string, 
                        std::function<
                            int(std::vector<std::string> &
                                        )>> 
                                        _Commands;

    //The name of the language file
    std::string _CurrentLanguage;
    
    //An array to keep the language pack for use
    std::vector<std::string>  _Language;

public:

    /*
        On creation, initialize the hash maps that map
        the input to commands.
    */
   
    Lina();
    
    int Start();
    
    //int Intro();

    /*
     * GetCommand requests the user to input from keyboard
     * And then it breaks that input to many arguments (based on whitespace)
     * Those arguments are then passed down to another commands
     */
    int GetCommand();

    int ChangeLanguage(std::vector<std::string> & arguments);

    int Help(std::vector<std::string> & arguments) const;

    /*
        Create a matrix with a given size
        The input to create a matrix conforms to:

        CREATE <NAME> [ROWS [x] COLUMNS]

        !!!NOTE!!!: If no argument is given to the size of the matrix
        then the matrix will have size 0x0

        For example: 
            create Matrix 3x4
            create Matrix 3 x 4
            create Matrix 3 4
            create Matrix

        Any superfluous arguments, or lack of numbers
        and the command is treated invalid
    */
    int Create(std::vector<std::string> & arguments);

    /*
        Change can be used for many purpose
        +) Change the application language
        +) Change the name of a matrix
        +) Change the value of a row, column or a whole matrix
    */
    int Change(std::vector<std::string> & arguments);

    int Show(std::vector<std::string> & arguments) const;

    int Exit(std::vector<std::string> & arguments);

//protected:
public:
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
    bool IsValidMatrixName(std::string name) const;

    Matrix<Fraction> Calculate(const std::string &expression) const;

    bool IsMatrix(std::string name) const;

    bool IsCommand(std::string name) const;

    bool IsKeyword(std::string name) const;
};

size_t FindMatchingParentheses(const std::string &str);
#endif  /* LINA_H */