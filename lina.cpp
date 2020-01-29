#include "lina.h"

enum CommandValues
{
    EXIT,
    CHANGE,
    HELP,
    SHOW,
    CREATE,
    CALCULATE,
    CHANGE_LANGUAGE,
    MAX_VALUE,
    MAX_LINE_SIZE = 300
};


Lina::Lina():_CurrentLanguage("EN-en")
{
    using namespace std::placeholders;
    using namespace std;

    _Commands.insert({string("exit"), 
                      function<int(vector<string> &)>(bind(&Lina::Exit, this, _1)) 
                      });
   
    _Commands.insert({string("help"), 
                      function<int(vector<string> &)>(bind(&Lina::Help, this, _1))
                      });

    _Commands.insert({string("create"), 
                      function<int(vector<string> &)>(bind(&Lina::Create, this, _1))
                      });

    _Commands.insert({string("show"), 
                      function<int(vector<string> &)>(bind(&Lina::Show, this, _1))
                      });
    /*
   _Commands.insert({string("change"), CHANGE});
   _Commands.insert({string(""), });*/
}

int Lina::Start()
{
    //ChangeLanguage();
    int LastCommand=1;
    while(LastCommand!=EXIT)
    {
        try
        {
            LastCommand= GetCommand();  
        }

        catch(std::exception &M)
        {
            std::cout<<M.what()<<"\n";
            std::cout<<"Please re-enter your command.\n";
        }
    }
    return 0;
}

/*
int Lina::ChangeLanguage()
{
    //std::cout<<"Tieng Viet (vn)\nEnglish (en)\n";
    std::cout<<"English (en)\n";
    std::string answer;
    std::cin>>answer;

    if (answer=="vn")
        _CurrentLanguage="VN-vn";
    else if (answer=="en")
        _CurrentLanguage="EN-en";


    std::ifstream langPack(_CurrentLanguage);
    _Language.resize(ALL_CATEGORY);

    for (size_t iii=0; iii<ALL_CATEGORY; ++iii)
        langPack>>_Language[iii];

    return CHANGE_LANGUAGE;
}*/

int Lina::Show(std::vector<std::string> &arguments)
{
    //If the user only specify SHOW as their command
    //Then we'll show them ALL the matrix
    if (arguments.size()==1)
        for (auto &pair: _Matrices)
        {
            std::cout<<"Matrix "<<pair.first<<": \n"<<pair.second<<"\n\n";
        }

    else 
        for (int iii=1; iii<arguments.size(); ++iii)
        {
            if (_Matrices.find(arguments[iii])==_Matrices.end())
                throw(Mexception(std::string("There's no matrix with name ")+arguments[iii]));
            std::cout<<"Matrix "<<arguments[iii]<<": \n"<<_Matrices[arguments[iii]]<<"\n\n";
        }
    return SHOW;
}

int Lina::GetCommand()
{
    //Get the command from keyboard
    std::cout<<"\nWhat would you like to do? \n"
             <<"type \"help\" for a list of commands to use.\n";

    std::stringstream ss;
    std::string temp;
    std::getline(std::cin, temp);
    ss<<temp;
    std::vector<std::string> arguments;
    while (!ss.eof())
    {
        arguments.push_back(std::string(""));
        ss>>arguments.back();
    }
    
    /*
    std::cout<<"These are the grabbed arguments:\n";
    for (auto &a: arguments)
    {
        std::cout<<a<<"\n";
    }
    std::cout<<"\n\n";
    */
    //If the first thing the user gives us is a matrix name
    //Then we need to calculate something
    if (IsValidMatrixName(arguments[0]) && 
        _Matrices.find(arguments[0])!=_Matrices.end())
    {
        //std::cout<<"This is matrix\n";
        return Calculate(arguments);
    }
    
    std::transform(arguments[0].begin(), 
                    arguments[0].end(), 
                    arguments[0].begin(), 
                    (int (*)(int)) std::tolower);

    if (_Commands.find(arguments[0])==_Commands.end())
        throw(Mexception("There's no command available with that keyword."));

    //std::cout<<"Command: "<<arguments[0]<<"\n";
    return _Commands[arguments[0]](arguments);
}


int Lina::Exit(std::vector<std::string> &arguments)
{
    std::cout<<"See you again!\n";
    return EXIT;
}

int Lina::Help(std::vector<std::string> &arguments)
{
    std::string fileName("en_help.txt");
    std::fstream help;

    if (_CurrentLanguage=="VN-vn")
        fileName="vn_help.txt";

    help.open(fileName, std::ios::in);

    std::string line;
    if (help.is_open())
        while (std::getline(help, line))
        {
            std::cout<<line<<"\n";
        }
    else 
        std::cerr<<"Error opening file \""<<fileName<<"\"\n";

    help.close();
    return HELP;
}


int Lina::Create(std::vector<std::string> &arguments)
{
    if (arguments.size()==1)
        throw (Mexception("You gave me no argument :/ Why?"));

    if (!IsValidMatrixName(arguments[1]))
        throw(Mexception("The name \""+arguments[1]+"\" is already reserved. Please come up with another one."));

    // At this point, there's at least 2 arguments that have been given
    
    // The user just want to create a default matrix for future use.
    // And that would be a zero matrix
    if (arguments.size()==2)
    {
        _Matrices.insert({arguments[1], Matrix<Fraction>(0, 0)});
        return CREATE;
    }

    // Have i ever tell you how much i love regex?
    std::string input;
    std::smatch sm;
    for (int iii=2; iii<arguments.size(); ++iii)
        input+=arguments[iii]+" ";
    
    /*
        These are the correct ways to create a matrix:

        <NAME> <Number> <Number>
        <Number> x <Number>     Yes, a literal 'x' character
        <Number>x <Number>
        <Number> x<Number>
        <Number>x<Number>


        For example:

        2 4
        2 x 4
        2x 4
        2    x4
        2x4

        Any superfluous arguments, or lack of numbers
        and the command is treated invalid

    */
    //std::cout<<"We need to regex this:"<<input<<"\n";
    if (!std::regex_match(input, sm, std::regex("\\s*(\\d+)\\s*x?\\s*(\\d+)\\s*")))
        throw (Mexception("There might have been some mistakes. Could you try again?"));

    _Matrices.insert({arguments[1], 
                        Matrix<Fraction>(
                            std::stoi(sm[1].str()), 
                            std::stoi(sm[2].str()))
                    });
    return CREATE;
}

int Lina::Calculate(std::vector<std::string> &arguments)
{
    return CALCULATE;
}


