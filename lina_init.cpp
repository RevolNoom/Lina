/*
    This file has one and only one thing: Lina constructor
    Internally, Lina initialize its keywords to their appropriate functions
*/

#include "lina.h"

template<typename Return_Type, typename ...Args_Type>
int Lina::CreateUtility(const std::string &UtilName, 
                        Return_Type (Lina::*Fptr)(Args_Type...), 
                        const std::string &HelpfilePath)
{
    //It means this keyword is in use
    //We cannot create an utility with this keyword
    if (_Keywords.find(UtilName)!=_Keywords.end())
        return 1;

    _Commands.insert({UtilName, Lina_Utility<Return_Type, Args_Type...>
                                        (UtilName, Fptr, HelpfilePath)
                    });

    _Keywords.insert(UtilName);
    return 0;
}


template<typename Return_Type, typename ...Args_Type>
int Lina::CreateUtility(const std::string &UtilName, 
                        Return_Type (Lina::*Fptr)(Args_Type...) const, 
                        const std::string &HelpfilePath)
{
    //It means this keyword is in use
    //We cannot create an utility with this keyword
    if (_Keywords.find(UtilName)!=_Keywords.end())
        return 1;

    _Commands.insert({UtilName, Lina_Utility<Return_Type, Args_Type...>
                                (UtilName, Fptr, HelpfilePath)

                    });

    _Keywords.insert(UtilName);
    return 0;
}

Lina::Lina():_CurrentLanguage("EN-en")
{
    using namespace std::placeholders;
    using namespace std;

    /*
        Binds keywords to functions
    */
    std::string HelpDir("res/help/");

    CreateUtility(string("create"), &Lina::Create, string(HelpDir + "create.txt"));

    CreateUtility(string("help"), &Lina::Help, string(HelpDir + "help.txt"));

    CreateUtility(string("quit"), &Lina::Quit, string(HelpDir + "quit.txt"));

    CreateUtility(string("show"), &Lina::Show, string(HelpDir + "show.txt"));

    CreateUtility(string("change"), &Lina::Change, string(HelpDir + "change.txt"));

    //CreateUtility(string(""), &Lina::, string(".txt"));



    

    /*

*/
    /*
        Reserved keywords
    */

    _Keywords.insert("det");
    _Keywords.insert("inv");
    /*
        Used in Show()
        When used alone, print all matrix name
    */
    _Keywords.insert("--info");
    // "i" as in Identity (Matrix) (Yeah, i need to work on that real soon)
    //_Keywords.insert("i");
    _Keywords.insert("--language");
    
    // _Keywords.insert("");
    
}