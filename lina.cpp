/*
    In this file lies the core features of Lina
    They are:
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

    std::string input;
    std::getline(std::cin, input);

    std::stringstream ss(input);
    std::string CommandName;
    ss>>CommandName;
    // std::cout<<"CommandName: "<<CommandName<<"\n";

    std::transform(CommandName.begin(), 
                    CommandName.end(), 
                    CommandName.begin(), 
                    (int (*)(int)) std::tolower);

    if (!IsCommand(CommandName))
        throw(Mexception("There's no command available with keyword \"" + CommandName + "\"."));


    std::string Arguments;
    std::getline(ss, Arguments);

    // std::cout<<"Arguments: "<<Arguments<<"\n";
    return _Commands[CommandName](BreakExpressions(Arguments, IsComma));
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



int Lina::Help(const std::vector<std::string> &arguments) const
{
    std::string fileName("en_help.txt");
    std::fstream help;

    if (_CurrentLanguage=="VN-vn")
        fileName="vn_help.txt";

    help.open(fileName, std::ios::in);

    std::string line;
    if (help.is_open())
        while (std::getline(help, line))
            std::cout<<line<<"\n";
    else 
        std::cerr<<"Error opening file \""<<fileName<<"\"\n";

    help.close();
    return HELP;
}



int Lina::Create(const std::vector<std::string> &arguments)
{
    if (arguments.size()==0)
        throw (Mexception("You gave me no argument :/ Why?"));

    /*
        Matrix naming rules:
        +) Only contains underscore "_" , latin letters, numbers
        +) First character is not a digit 
        In a Create command argument, MatrixName is mandatory
    */
    std::string MatrixName("([_a-zA-Z][_\\w\\d]*)");

    /*
        This part of argument is optional, hence the "?" at the end
    */
    std::string MatrixSize("(\\s+(\\d+)\\s*x?\\s*(\\d+)\\s*)?");
    
    std::regex  Rgx( MatrixName + MatrixSize);
    std::smatch ArgumentParts;

    for (auto &Arg: arguments)
    {
        if (!std::regex_match(Arg, ArgumentParts, Rgx))
        {
            std::cerr<<"Cannot create matrix with argument: \""<<Arg<<"\".\n";
            continue;
        }

        //  Check if the size of the matrix is grabbed
        if (ArgumentParts[2].str().size())
        {
            _Matrices.insert({ArgumentParts[1].str(),
                                Matrix<Fraction>(std::stoll(ArgumentParts[3].str()),
                                                std::stoll(ArgumentParts[4].str()))
                            });
            std::cout<<"Matrix "
                        <<ArgumentParts[1].str() + " "
                        <<ArgumentParts[3].str() + "x" + ArgumentParts[4].str()
                        <<" has been created\n";
        }

        // This matrix has no size argument
        // Create a default matrix
        else
        {
            _Matrices.insert({ArgumentParts[1].str(), Matrix<Fraction>(0, 0)});
            std::cout<<"Matrix "
                        <<ArgumentParts[1].str()
                        <<" 0x0 has been created\n";
        }
    }
    

    return CREATE;
}


int Lina::Change(const std::vector<std::string> &arguments)
{
    
    if (arguments.size()==0)
        throw(Mexception("Ummmm... What am i supposed to change? :/"));
    

    for (auto &Arg: arguments)
    {
        std::string MatrixName;
        int assignmentSign=Arg.find('=');

        MatrixName = (assignmentSign!=std::string::npos) ?
                        Arg.substr(0, assignmentSign) :     
                            Arg.substr(0, Arg.find_first_of(' '));
        
        if (!IsMatrix(MatrixName))
            throw (Mexception(MatrixName + std::string(" is not a matrix name.")));

        std::cout<<"Modifying matrix "<<MatrixName<<": \n";

        /*
            Change the matrix by assignment to an expression
        */
        if (assignmentSign!=std::string::npos)
        {
            _Matrices[MatrixName] = Calculate(MatrixName.substr(assignmentSign+1));
        }
        /*
            Change the matrix by keyboard input
        */
        else
        {
            //In case the user wants to change size also
            //We have to recreate this matrix
            if (MatrixName==Arg)
            {
                std::cin>>_Matrices[MatrixName];
                std::cin.ignore(10000, '\n');
            }
        }
        
        std::cout<<"\nYour matrix \""<<MatrixName<<"\" after modification:\n"<<_Matrices[MatrixName];
    }
    
    return CHANGE;
}






int Lina::Show(const std::vector<std::string> &arguments) const
{
    //If the user only specify SHOW as their command
    //Then we'll show them ALL the matrix
    if (arguments.size()==0)
    {
        for (auto &pair: _Matrices)
            std::cout<<"Matrix "<<pair.first<<": \n"<<pair.second<<"\n\n";
        return SHOW;
    }
    
    //If option --info is given, print matrices names and size
    if (arguments.size()==1 && arguments[1]=="--info")
    {
        for (auto &pair: _Matrices)
            std::cout<<"Matrix \""<<pair.first<<"\" "
                        <<pair.second.Rows()<<"x"<<pair.second.Columns()
                        <<"\n";
        return SHOW;
    }

    //No option is given, or given but we don't understand
    //Print name/expression and result
    for (auto &Arg: arguments)
        std::cout<<"Matrix of \""<<Arg
                    <<"\":\n"<<Calculate(Arg);
        
    return SHOW;
}




int Lina::Exit(const std::vector<std::string> &arguments) const
{
    std::cout<<"See you again!\n";
    return EXIT;
}