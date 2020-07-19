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
    QUIT,
    CHANGE,
    HELP,
    SHOW,
    CREATE,
    CALCULATE,
    CHANGE_LANGUAGE,
    MAX_VALUE,
    MAX_LINE_SIZE = 300
};

void Lina::Intro()
{
    std::fstream Banner(BANNER);
    if (!Banner.is_open()) 
    {
        std::cout<<"HEY! WHO STOLE OUR PRETTY BANNER? >:( \n";
    }
    else
    {
        std::string Str;

        while (1)
        {
            if (std::getline(Banner, Str))
            {
                std::cout<<Str<<"\n";
            }
            else if (Banner.eof())
                break;
            else 
            {
                std::cerr<<"Banner Ripped! Reason unknown!";
            }
        }
    }
    std::cout<<"\nWhat would you like to do? \n"
             <<"Type \"help\" for a list of commands to use.\n";

}

int Lina::Start()
{
    Intro();
    //ChangeLanguage();
    int LastCommand=1;
    while(LastCommand!=QUIT)
    {
        std::cout<<"Your command: ";
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
    std::string input;
    std::getline(std::cin, input);

    std::stringstream ss(input);
    std::string CommandName;
    ss>>CommandName;

    std::transform(CommandName.begin(), 
                    CommandName.end(), 
                    CommandName.begin(), 
                    (int (*)(int)) std::tolower);


    std::string Arguments;
    std::getline(ss, Arguments);

    return CallUtility(CommandName, BreakExpressions(Arguments, IsComma));
}

int Lina::CallUtility(const string& Util_Name, const vector<string> &Args)
{
    if (!IsCommand(Util_Name))
        throw(Mexception("There's no command available with keyword \"" + Util_Name + "\"."));
    
    int Util_Result(-1);

    std::visit([this, &Util_Result, &Util_Name, &Args](const auto& Util) 
                {
                    Util_Result = (this->*Util)(Args);
                }
                , _Commands[Util_Name]._Util_Function);

    return Util_Result;
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

    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //I wrote this and then forgot what it does. I'm keeping it just in case...
    //std::vector<bool> HelpNeeded(MAX_VALUE);

/*
    if (_CurrentLanguage=="VN-vn")
        fileName="vn_help.txt";
*/

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
    
    std::string MatrixName;

    for (auto &Arg: arguments)
    {
        if (Arg==std::string(""))
            continue;

        auto Verses=BreakExpressions(Arg, [](int c)->int{return c== '=';});
        
        if (Verses.size()>2)
            throw (Mexception("Too many assignment: " + Arg));

        /*
            Change the matrix by assignment to an expression
        */
        if (Verses.size()==2)
        {
            if (!IsMatrix(Verses[0]))
                throw (Mexception("\"" + Verses[0] + std::string("\" is not a matrix name.")));
            MatrixName=Verses[0];
            _Matrices[MatrixName] = Calculate(Verses[1]);
        }
        /*
            Change the matrix by keyboard input
        */
        else
        {
            std::smatch sm;

            //Search for matrix name and size (if there's any)
            if (!std::regex_match(Arg, sm, 
                    std::regex("([_a-zA-Z][_\\w\\d]*)(\\s+(\\d+)\\s*x?\\s*(\\d+)\\s*)?")))
                throw(Mexception("Invalid arguments: " + Arg));

            MatrixName=sm[1].str();
            if (sm[3].str()!=std::string(""))
                _Matrices[MatrixName]=Matrix<Fraction>(std::stoll(sm[3].str()), std::stoll(sm[4].str()));
             
            std::cin>>_Matrices[MatrixName];
            std::cin.ignore(10000, '\n');
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
    if (arguments.size()==1 && arguments[0]=="--info")
    {
        for (auto &pair: _Matrices)
            std::cout<<"Matrix \""<<pair.first<<"\" is a "
                        <<pair.second.Rows()<<"x"<<pair.second.Columns()
                        <<" matrix\n";
        return SHOW;
    }

    //Print name/expression and result
    for (auto &Arg: arguments)
    {
        auto ArgumentOption = BreakExpressions(Arg, std::isspace);
        if (ArgumentOption[0]=="--info")
        {
            auto Expression=Arg.substr(6);
            auto Matrix = Calculate(Expression);
            std::cout<<"Matrix \""<<Expression<<"\" is a "
                        <<Matrix.Rows()<<"x"<<Matrix.Columns()<<" matrix.\n";
        }
        else
            std::cout<<"Matrix \""<<Arg<<"\":\n"<<Calculate(Arg)<<"\n";  
    }   
    return SHOW;
}




int Lina::Quit(const std::vector<std::string> &arguments) const
{
    std::cout<<"See you again!\n";
    return QUIT;
}

