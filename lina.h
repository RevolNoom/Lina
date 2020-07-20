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
    +) Wrapper object to each utility of Lina
    +) Change() to change language
    +) Read Makefile and then update g++ with -std=c++17 for std::variant
    +) Calculate() remade with inclusion of identity matrix
    +) Redirect output to text file
    +) Remove a matrix
*/


#ifndef LINA_H
#define LINA_H

#include <variant>
#include <unordered_map>
#include <fstream>
#include <functional>
#include <limits>
#include <fstream>
#include "matrix.h"
#include "lina_util.h"
#include <regex>
#include <unordered_set>

using Lina_Operand = std::variant<Fraction, Matrix<Fraction>>;

class Lina
{
    friend class Lina_Utility<int, const vector<string> &>;
private:

    std::unordered_set<std::string> _Keywords;

    std::unordered_map<std::string, Lina_Operand> _Matrices;

    //Map a command with a corresponding name
    std::unordered_map<std::string, Lina_Utility<int, const vector<string>&>> _Utility;

    //The name of the language file
    std::string _CurrentLanguage;
    
    //An array to keep the language pack for use
    std::vector<std::string>  _Language;

    //Defined at lina_init.cpp
    //Tell the path to the banner and help files
    static std::string BannerDir;
    static std::string HelpDir;
public:

    /*
        On creation, initialize the hash maps that map
        the input to commands.
    */
   
    Lina();

    //Draw a pretty banner on the terminal at application startup
    void Intro();

    int Start();
    
    //int Intro();

    /*
     * GetCommand requests the user to input from keyboard
     * And then it breaks that input to many arguments (based on whitespace)
     * Those arguments are then passed down to another commands
     */
    int GetCommand();

    int Help(const std::vector<std::string> & arguments) const;

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
    int Create(const std::vector<std::string> & arguments);

    /*
        Change can be used for many purpose
        +) Change the application language
        +) Change the name of a matrix
        +) Change the value of a row, column or a whole matrix
    */
    int Change(const std::vector<std::string> & arguments);

    /*
        Show existing matrices by names, or by drawing a board
    */
    int Show(const std::vector<std::string> & arguments) const;

    int Quit(const std::vector<std::string> & arguments) const;

protected:
//public:
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

    /*
        @brief  Calculate an expression that contains names of the matrix this application know
        @param  The expression you want to calculate
        @return The resulting matrix
    */
    std::variant<Fraction, Matrix<Fraction>> Calculate(const std::string &expression) const;

    
    /*
        @brief  Test a name to see whether it has been created before
        @param  The matrix name
        @return True if the matrix has been created, false otherwise
    */
    bool IsMatrix(std::string name) const;

    /*
        @brief  Test a word to see whether it's a command keyword
        @param  The word to test
        @return True if that word (case - INSENSITIVE) is a command. 
                False otherwise
    */
    bool IsCommand(std::string name) const;
    
    /*
        @brief  Test a word to see whether it conflicts with Lina's mathematic function names
        @param  The word to test
        @return True if that word (in case - INSENSITIVE) is a keyword. 
                False otherwise
    */
    bool IsKeyword(std::string name) const;


    /*
        Create an utility to better manage the application
        return 0 if the utility is added successfully
                1 otherwise.

    */
   
    template<typename Return_Type, typename ...Args_Type>
    int CreateUtility(const std::string &UtilName, 
                        Return_Type (Lina::*Fptr)(Args_Type...), 
                        const std::string &HelpfilePath);


    template<typename Return_Type, typename ...Args_Type>
    int CreateUtility(const std::string &UtilName, 
                        Return_Type (Lina::*Fptr)(Args_Type...) const, 
                        const std::string &HelpfilePath);

    int CallUtility(const string &Util_Name, const vector<string> &Args);


    /*
        Actually, Calculate() is only a wrapper function.
        What really happened in Calculate() starts her.
        RealCalculate() deals with recursive calls, identity matrices,...
        All the things you shouldn't worry about
        @param 
                begin: The beginning position of the position you want to calculate
                end:    The past-the-end position of the said expression
        @return
                The first factor in the expression.
    */
    //std::variant<Matrix<Fraction>, Fraction> RealCalculate(const std::string &expression, size_t begin, size_t end) const;

};

/*
    @brief  Look for the leftmost opening parentheses, then find its corresponding closing one
    @param  The string in need to find the parentheses
    @return Index of the closing parenthesis of the leftmost opening parenthesis
            -1 if there's no valid parentheses group
*/
size_t FindMatchingParentheses(const std::string &str);

bool IsInteger(const std::string &str);

/*
        @brief  Break a string into many substrings, based on the delimiter
        @param  Expression: The string contains expressions
        @param  delimPred:  A function which return true if a given character is considered delimiter.
        @return An array contains broken substrings in @param, from left to right
                All whitespaces at the beginning and the end of each expressions are trimmed 
*/    
std::vector<std::string> BreakExpressions(const std::string &Expression, int (*delimPred)(int c) );
std::string TrimWhiteSpace(const std::string &str);
/*
    Return true if c==','
*/
int IsComma(int c);


#endif  /* LINA_H */