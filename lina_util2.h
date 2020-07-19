/*
    This header is already included in lina_util.h
    
    Inside this header, the derived class for Lina utility is declared
    It's an abstract class to wrap Lina function to be called in the application
*/

/*
    Testing Parameter-pack
*/
#include <functional>
#include <variant>

class Lina;

template <typename Return_Type, typename ...Args_Type>
using Util_Ptr = Return_Type (Lina::*) (Args_Type...);

template <typename Return_Type, typename ...Args_Type>
using Util_cPtr = Return_Type (Lina::*) (Args_Type...) const;

template <typename Return_Type, typename ...Args_Type>
class Lina_Utility: public Lina_Utility_Base
{

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
    Lina_Utility(const string &Function_Name, 
                Util_Ptr<Return_Type, Args_Type...> Fptr, 
                const string &Helpfile);

    Lina_Utility(const string &Function_Name, 
                Util_cPtr<Return_Type, Args_Type...> Fptr, 
                const string &Helpfile);

    ~Lina_Utility() override;

    Lina_Utility<Return_Type, Args_Type...>::Name() const;

    //Implicit conversion to pointer form (for convenience)
    operator Util_Ptr<Return_Type, Args_Type...> () const;

    operator Util_cPtr<Return_Type, Args_Type...> () const;
};


template<typename Return_Type, typename ...Args_Type>
Lina_Utility<Return_Type, Args_Type...>::~Lina_Utility()
{
}


template<typename Return_Type, typename ...Args_Type>
Lina_Utility<Return_Type, Args_Type...>::operator Util_Ptr<Return_Type, Args_Type...>() const
{
    return std::get<0>(this->_Util_Function);
}

template<typename Return_Type, typename ...Args_Type>
Lina_Utility<Return_Type, Args_Type...>::operator Util_cPtr<Return_Type, Args_Type...>() const
{
    return std::get<1>(this->_Util_Function);
}

template <typename Return_Type, typename ...Args_Type>
Lina_Utility<Return_Type, Args_Type...>::Name() const