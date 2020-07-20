/*
    Lina_Util is somewhat a struct to pack things together.
    It packs Function name, Function itself, 
        and the helpfile that's associated with it
*/

#ifndef LINA_UTIL
#define LINA_UTIL

/*
    Have never used filesystem before
    Now's as a good time as ever to practice it a bit
*/
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <variant>

class Lina;

using namespace std;

template <typename Return_Type, typename ...Args_Type>
using Util_Ptr = Return_Type (Lina::*) (Args_Type...);

template <typename Return_Type, typename ...Args_Type>
using Util_cPtr = Return_Type (Lina::*) (Args_Type...) const;

template <typename Return_Type, typename ...Args_Type>
class Lina_Utility
{
    friend class Lina;
private:
    //Name of the utility
    string _Util_Name;
    
    //The helpfile name of this function 
    filesystem::path _Util_Helpfile;

    //Pointer to the given function
    std::variant<
            Util_Ptr<Return_Type, Args_Type...>,
            Util_cPtr<Return_Type, Args_Type...> > _Util_Function;

public:

    /*
        @brief: Construct an utility to work for a Lina

        @param: 
        - Name: Name of the function
        - pFunction: A function pointer to the desired Lina function
        - L : The Lina object this utility works for
        - Helpfile: The (string) pathname to the manual file 
    */
    Lina_Utility(const string &Function_Name, 
                Util_Ptr<Return_Type, Args_Type...> Fptr, 
                const string &Helpfile);

    // Construct Utility for a const-qualified function 
    Lina_Utility(const string &Function_Name, 
                Util_cPtr<Return_Type, Args_Type...> Fptr, 
                const string &Helpfile);

    Lina_Utility(const Lina_Utility<Return_Type, Args_Type...> &L);

    Lina_Utility();


    //Return the name of the utility in the form of a string
    const string& Name() const;

    // Write to stdout the manual of this function
    // @return 0 if the manual is written successfully
    //          1 otherwise
    int GetHelp() const;

    // Write to stdout the synopsis of the utility
    // (2 first lines of each helpfile)
    int GetHelpBrief() const;
};

#include "lina_util.tpp"

#endif
