#include "matrix.h"
//Represent the matrix by a board
std::ostream& operator<<(std::ostream& OS, const Matrix<Fraction>& M)
{

    //Find the length of the longest element
    //So that we can format the matrix according to it 
    long long LongestLength(0);
    for (long long row=0; row<M.Rows(); ++row)
        for(long long col=0; col<M.Columns(); ++col)
        {
            long long length=M.at(row, col).WriteLength();
            if (length>LongestLength)
                LongestLength=length;
        }

    //Draw the matrix
    for (long long row=0; row<M.Rows()*2+1; ++row)
    {
        if (row%2) //This is an element-displaying line
        {
            std::cout<<'|';
            for (long long col=0; col<M.Columns(); ++col)
                OS<<std::string(
                                LongestLength - 
                                M.at(row/2, col).WriteLength()
                                , ' ')
                    <<M.at(row/2, col)
                        <<'|';
        }
        else      //This is a horizontal border
        {
            OS<<std::string("+");
            std::string border=std::string(LongestLength, '-') + std::string("+");
            for (long long col=0; col<M.Columns(); ++col)
                OS<<border;
        }
        OS<<"\n";
    }
    return OS;
}


//Represent the matrix by a board
std::ostream& operator<<(std::ostream& OS, const Matrix<double>& M)
{
    int LongestLength=0;
    for (long long row=0; row<M.Rows(); ++row)
        for(long long col=0; col<M.Columns(); ++col)
        {
            double length=M.at(row, col);
            if (std::to_string(length).size()>LongestLength)
                LongestLength=std::to_string(length).size();
        }

    //Draw the matrix
    for (long long row=0; row<M.Rows()*2+1; ++row)
    {
        if (row%2) //This is an element-displaying line
        {
            std::cout<<'|';
            for (long long col=0; col<M.Columns(); ++col)
            {
                std::string element=std::to_string(M.at(row/2, col));
                OS<<std::string(
                                LongestLength - element.size()
                                , ' ')
                    <<element
                        <<'|';
            }
        }
        else      //This is a horizontal border
        {
            OS<<std::string("+");
            std::string border=std::string(LongestLength, '-') + std::string("+");
            for (long long col=0; col<M.Columns(); ++col)
                OS<<border;
        }
        OS<<"\n";
    }
    return OS;
}


//Receive input for ALL elements in the matrix
template<typename _Type>
std::istream& operator>>(std::istream& IS, Matrix<_Type>& M)
{
    for (size_t iii=0; iii<M.Rows(); ++iii)
    {
        std::cout<<"Enter row "<<iii<<": ";
        for (size_t jjj=0; jjj<M.Columns(); ++jjj)
        {
            IS>>M.at(iii, jjj);
        }

        while (IS.peek()!='\n')
        {
            if (!std::isspace(IS.peek()))
                throw(Mexception("Input has too many elements for one row\n"));
            
            IS.ignore(1);
        }
    }   
    return IS;
}