#include "lina.h"

enum CommandValues
{
    EXIT,
    IDLE,
    CHANGE,
    HELP,
    SHOW,
    CREATE,
    CALCULATE,
    CHANGE_LANGUAGE,
    MAX_VALUE,
    MAX_LINE_SIZE = 300
};


Lina::Lina(): _Matrices(std::unordered_map<std::string, Matrix<Fraction>>()),
              _Commands(MAX_VALUE),
              _CurrentLanguage("EN-en")
{
    _Commands.insert({std::string("exit"), 
                      std::function<int()>(std::bind(&Lina::Exit, this)) 
                      });
   
    _Commands.insert({std::string("help"), 
                      std::function<int()>(std::bind(&Lina::Help, this))
                      });

    _Commands.insert({std::string("create"), 
                      std::function<int()>(std::bind(&Lina::Create, this))
                      });
    /*
   _Commands.insert({std::string(""), IDLE});
   _Commands.insert({std::string("change"), CHANGE});
   _Commands.insert({std::string("show"), SHOW});
   _Commands.insert({std::string(""), });*/
}

int Lina::Start()
{
    ChangeLanguage();

    do
    {
        std::cout<<"\nWhat would you like to do? \n"
                 <<"type \"help\" for a list of commands to use.\n";
        
        try
        {
            if (GetCommand()==EXIT)
                break;
        }
        catch(std::exception &M)
        {
            std::cout<<M.what()<<"\n";
            std::cout<<"Please re-enter your command.\n";
        }
    }
    while(1);

    return 0;
}

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
}


int Lina::GetCommand()
{
    //Get the command from keyboard
    std::cin.ignore();
    std::getline(std::cin, _Buffer);
    std::transform(_Buffer.begin(), _Buffer.end(), _Buffer.begin(), (int (*)(int))std::toupper);
    

    //Get the command (or the name of a matrix, whichever is the first thing)
    std::regex Command("^//s*([\\w_][\\w\\d_]*)");
    std::smatch sm;
    std::regex_search(_Buffer, sm, Command);

    /*
    std::string part("([\\w\\d_]+|[+-*=]+))
    std::string space("")
    std::regex rgx(part+)
    */

    
    auto execute =_Commands.find(sm[1].str());
    if (execute!=_Commands.end())
        return execute->second();

    auto matrix= _Matrices.find(sm[1].str());
    if (matrix!=_Matrices.end())
        return Calculate();
    
    return (execute->second)();
}


int Lina::Exit()
{
    std::cout<<"See you again!\n";
    return EXIT;
}

int Lina::Help()
{
    std::ifstream help;
    if (_CurrentLanguage=="VN-vn")
        help.open("VN-help");
    
    for (std::string line;  std::getline(help, line); )
    {
        std::cout<<line<<"\n";
    }
    
    return HELP;
}


/*
    !!Find a way to initialize Regexes only once!!!
*/
int Lina::Create()
{
    /*
        The following things will be captured after regexing:
        +) Matrix_Name (Mandatory)
        +) Digits for matrix size (Only two or no numbers are accepted)
    */
    std::string Command("[\\w\\d_]+");
    std::string space("\\s+");
    std::string maybeSpace("\\s*");
    std::string Matrix_Name("([_\\w][_\\w\\d]*)");
    std::string MatrixSizeStyle("(\\d+)?(?>\\s*x?\\s*)(\\d+)?");

    std::regex rgx( Command + space + 
                    Matrix_Name + space + 
                    MatrixSizeStyle + maybeSpace
                    ,
                    std::regex_constants::icase);
    std::smatch sm;

    
    if (std::regex_search(_Buffer, sm, rgx))
    {
        if (sm[2].str().size()==0)
            _Matrices.insert({sm[1].str(), Matrix<Fraction>(0, 0)});
        else if (sm[3].str().size()==0)
            throw(Mexception("And what am i supposed to do with only one number? :/"));
        else 
            _Matrices.insert({sm[1].str(), 
                                Matrix<Fraction>(
                                    std::stoll(sm[2].str()), 
                                    std::stoll(sm[3].str())
                                )});
    }
    else 
        throw (Mexception("There might have been some mistakes. Could you try again?"));
    return CREATE;
}

int Lina::Calculate()
{
    return EXIT;
}


bool Lina::IsValidName(std::string name)
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