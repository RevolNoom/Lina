template <typename Return_Type, typename ...Args_Type>
Lina_Utility<Return_Type, Args_Type...>::
            Lina_Utility(const string &Function_Name,
                            Util_Ptr<Return_Type, Args_Type...> Fptr,
                            const string &Helpfile
                            )
{
    this->_Util_Name     = Function_Name;
    this->_Util_Helpfile = Helpfile;
    this->_Util_Function = Fptr;
}

template <typename Return_Type, typename ...Args_Type>
Lina_Utility<Return_Type, Args_Type...>::
            Lina_Utility(const string &Function_Name,
                            Util_cPtr<Return_Type, Args_Type...>  Fptr,
                            const string &Helpfile
                            )
{
    this->_Util_Name     = Function_Name;
    this->_Util_Helpfile = Helpfile;
    this->_Util_Function = Fptr;
}

template <typename Return_Type, typename ...Args_Type>
Lina_Utility<Return_Type, Args_Type...>::
            Lina_Utility(const Lina_Utility<Return_Type, Args_Type...> &L)
{
    this->_Util_Name     = L._Util_Name;
    this->_Util_Helpfile = L._Util_Helpfile;
    this->_Util_Function = L._Util_Function; 
}

template <typename Return_Type, typename ...Args_Type>
Lina_Utility<Return_Type, Args_Type...>::
            Lina_Utility()
{
    this->_Util_Name     = std::string("");
    this->_Util_Helpfile = std::string("");
    this->_Util_Function = {};
}

template <typename Return_Type, typename ...Args_Type>
const string& Lina_Utility<Return_Type, Args_Type...>::Name() const
{
    return this->_Util_Name;
}

template <typename Return_Type, typename ...Args_Type>
int Lina_Utility<Return_Type, Args_Type...>::GetHelp() const
{
    std::fstream Helpfile(_Util_Helpfile);

    if (!Helpfile.is_open())
    {
        return 1;
    }

    else
    {
        std::string Str("");
        while (1)
        {
            if (std::getline(Helpfile, Str))
            {
                std::cout<<Str<<"\n";
            }
            else
            {
                if (Helpfile.eof())
                    break;
                else 
                {
                    Helpfile.clear();
                    Helpfile.ignore(std::numeric_limits<std::streampos>::max(), '\n');
                }
            }
        }
        std::cout<<"\n";
    }
    return 0;
}

template <typename Return_Type, typename ...Args_Type>
int Lina_Utility<Return_Type, Args_Type...>::GetHelpBrief() const
{
    std::fstream Helpfile(_Util_Helpfile);

    if (!Helpfile.is_open())
    {
        std::cout<<"Can't open help file: "<<_Util_Helpfile<<"\n";
        return 1;
    }

    else
    {
        std::string Str("");
        for (int iii=0; iii<2 && std::getline(Helpfile, Str); ++iii)
        {
                std::cout<<Str<<"\n";
        }
        std::cout<<"\n";
    }
    return 0;
}

