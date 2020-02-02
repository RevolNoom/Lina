/*
    In this file lies the core features of Lina
    They are:
    +) Application start up
    +) User's commands getter
    +) Language option (To be updated)
    +) Help display
    +) Matrices creation
    +) Matrices modification
    +) Matrices display
    +) Application exiting
*/

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



int Lina::GetCommand()
{
    //Get the command from keyboard
    std::cout<<"\nWhat would you like to do? \n"
             <<"Type \"help\" for a list of commands to use.\n";

    std::stringstream ss;
    std::string temp;
    std::getline(std::cin, temp);
    ss<<temp;
    
    /*
        'arguments' is an array of string and is guaranteed that:
        +) The first element is a command name or matrix name
        +) Every elements do not contain space
        +) There's no null string argument ("")
    */
    std::vector<std::string> arguments;
    while (!ss.eof())
    {
        arguments.push_back(std::string(""));
        ss>>arguments.back();
        if (!ss.good())
        {
            ss.clear();
            ss.ignore(32767, '\n');
        }
    }
    while (arguments.back()==std::string(""))
        arguments.pop_back();
    
    //If the first thing the user gives us is a matrix name
    //Then we need to calculate something
    if (IsMatrix(arguments[0]))
    {
        if (arguments[1]!="=")
            throw("We don't support that right now. Please come back later");

        std::string arg;
        for (int iii=2; iii<arguments.size(); ++iii)
            arg+=arguments[iii];

        _Matrices[arguments[0]] = Calculate(arg);
        return CALCULATE;
    }

    /*
        If the first thing we get is not a matrix
        Then it's a command
        Find it, and then execute it
        If it's not, well then, throw an error
    */
    std::transform(arguments[0].begin(), 
                    arguments[0].end(), 
                    arguments[0].begin(), 
                    (int (*)(int)) std::tolower);

    if (!IsCommand(arguments[0]))
        throw(Mexception("There's no command available with keyword \"" + arguments[0] + "\"."));

    return _Commands[arguments[0]](arguments);
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



int Lina::Help(std::vector<std::string> &arguments) const
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
        _Matrices.insert({arguments[1], 
                            Matrix<Fraction>(0, 0)});
        return CREATE;
    }

    // Have i ever tell you how much i love regex?
    std::string input;
    std::smatch sm;
    for (int iii=2; iii<arguments.size(); ++iii)
        input+=arguments[iii]+" ";
    
  
    if (!std::regex_match(input, sm, std::regex("\\s*(\\d+)\\s*x?\\s*(\\d+)\\s*")))
        throw (Mexception("There might have been some mistakes. Could you try again?"));

    _Matrices.insert({arguments[1], 
                        Matrix<Fraction>(
                            std::stoi(sm[1].str()), 
                            std::stoi(sm[2].str()))
                    });
    return CREATE;
}


int Lina::Change(std::vector<std::string> & arguments)
{
    
    if (arguments.size()==1)
        throw(Mexception("Ummmm... What am i supposed to change? :/"));
    if (IsMatrix(arguments[1]))
    {
        if (arguments.size()==2)
        {
            std::cin>>_Matrices[arguments[1]];
        }
        else
            throw("Too many arguments: " + std::to_string(arguments.size()));
        std::cout<<"\nYour matrix \""<<arguments[1]<<"\" after changing:\n"<<_Matrices[arguments[1]];
        std::cin.ignore(10000, '\n');
    }
    else if (IsKeyword(arguments[1]))
    {
        throw(Mexception("Pretty sorry, you can't change a command."));
    }
    else 
        throw(Mexception("That's not a matrix name. Could you try again?"));
    return CHANGE;
}






int Lina::Show(std::vector<std::string> &arguments) const
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
                throw(Mexception("There's no matrix with name "+arguments[iii]));
            std::cout<<"Matrix "<<arguments[iii]<<": \n"<<_Matrices.at(arguments[iii])<<"\n";
        }
    return SHOW;
}




int Lina::Exit(std::vector<std::string> &arguments)
{
    std::cout<<"See you again!\n";
    return EXIT;
}







