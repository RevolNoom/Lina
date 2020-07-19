/*
    ALL THE FUNCTIONS THAT ARE ESSENTIAL
    FOR THE MATRIX TO BEHAVE NORMALLY ARE DEFINED
    IN THIS TPP FILE

    FOR MORE COMPLEX FUNCTIONS TO WORK WITH MATRIX
    (CALCULATING DETERMINANT, FINDING INVERSE,...)
    THEY ARE DEFINED IN "matrix.tpp" AND "matrix1.tpp"
*/

/*
-------------------------------------
-------------------------------------
|||||| C O N S T R U C T O R S ||||||
-------------------------------------
-------------------------------------
*/

template<typename _Type>
Matrix<_Type>::Matrix(): Array2D<_Type>()
{
}

template<typename _Type>
Matrix<_Type>::Matrix(const Matrix<_Type>& M): Array2D<_Type>(M.Rows(), M.Columns())
{
    this->_array2d=M._array2d;
}

template<typename _Type>
Matrix<_Type>::Matrix(long long rows, long long columns): 
                Array2D<_Type>(Array2D<_Type>(rows, columns))
{
}

template<typename _Type>
Matrix<_Type>::Matrix(long long rows, long long columns, const _Type &value): 
                Array2D<_Type>(Array2D<_Type>(rows, columns, value))
{
}

template<typename _Type>
Matrix<_Type> Identity(long long order, const _Type &value)
{
    Matrix<_Type> M(order, order);
    for (long long iii=0; iii<order; ++iii)
        M.at(iii, iii)=value;
    return M;
}



/*
----------------------------------------------
----------------------------------------------
||||||||||||| M O D I F I E R S ||||||||||||||
----------------------------------------------
----------------------------------------------
*/

//Assign another matrix to this
template<typename _Type>
Matrix<_Type>& Matrix<_Type>::operator=(const Matrix<_Type>& M)
{
    this->_array2d= M._array2d;
    this->_rows   = M._rows;
    this->_columns= M._columns;
    return *this;
}


//Change the values in one row by the input of the user
//void ChangeRowValues(long long row);
//void ChangeRow(long long r, ROW&& row)

//Change the values in one column by the input of the user
//void ChangeColumnValues(long long column);
//void ChangeRow(long long c, COLUMN&& column)


template<typename _Type>
Block<_Type> Matrix<_Type>::row(long long index)
{
    return Block<_Type>(this, index, true);
}


template<typename _Type>
Block<_Type> Matrix<_Type>::column(long long index)
{
    return Block<_Type>(this, index, false);
}
/*
----------------------------------------------
----------------------------------------------
||||||||||||   M A T R I C E S   |||||||||||||
|||||||||||| O P E R A T I O N S |||||||||||||
----------------------------------------------
----------------------------------------------
*/

/*
/////////////////
A D D I T I O N S
/////////////////
*/
template<typename _Type>
void Matrix<_Type>::operator+=(const Matrix<_Type>& M)
{
    if (!HasSameSizeWith(M))
        throw(Mexception("Addition operands of different size"));
    for (long long iii=0; iii<this->Rows(); ++iii)
        for (long long jjj=0; jjj<this->Columns(); ++jjj)
            this->at(iii, jjj)+=M.at(iii,jjj);
}


template<typename _Type>
void Matrix<_Type>::operator+=(const _Type& value)
{
    if (!IsSquareMatrix())
        throw(Mexception("Identity addition of non-square matrix"));
    for (long long iii=0; iii<this->Rows(); ++iii)
        this->at(iii, iii)+=value;
}

template<typename _Type>
Matrix<_Type> operator+(const Matrix<_Type>& A, const Matrix<_Type>& B)
{
    Matrix<_Type> result=A;
    result+=B;
    return result;
}

template<typename _Type>
Matrix<_Type> operator+(const Matrix<_Type>& M, const _Type& value)
{
    Matrix<_Type> result=M;
    result+=value;
    return result;
}

template<typename _Type>
Matrix<_Type> operator+(const _Type& value, const Matrix<_Type>& M)
{
    return M + value;
}




/*
///////////////////////////////////////////
M U L T I P L Y I N G   O P E R A T I O N S
///////////////////////////////////////////
*/

template<typename _Type>
void Matrix<_Type>::operator*=(const Matrix<_Type>& M)
{
    if (!IsMultiplicableWith(M))
        throw (Mexception("Matrix multiplication dimension error"));
    
    Matrix<_Type> out(this->Rows(), M.Columns());

    for (long long iii=0; iii<this->Rows(); ++iii)
        for(long long jjj=0; jjj<M.Columns(); ++jjj)
        {
            for (long long index=0; index<this->Columns(); ++index)
                out.at(iii, jjj)+= this->at(iii, index)*M.at(index, jjj);
        }

    *this=out;
}

template<typename _Type>
void Matrix<_Type>::operator*=(const _Type& value)
{
    for (long long row=0; row<this->Rows(); ++row)
        for(long long col=0; col<this->Columns(); ++col)
            this->at(row, col)*=value;
}

template<typename _Type>
Matrix<_Type> operator*(const Matrix<_Type>& A, const Matrix<_Type>& B)
{
    Matrix<_Type> result=A;
    result*=B;
    return result;
}

template<typename _Type>
Matrix<_Type> operator*(const Matrix<_Type>& A, const _Type& value)
{
    Matrix<_Type> result=A;
    result*=value;
    return result;
}

template<typename _Type>
Matrix<_Type> operator*(const _Type& value, const Matrix<_Type>& M)
{
    return M*value;
}


/*
///////////////////////////////////////////
S U B T R A C T I N G   O P E R A T I O N S 
///////////////////////////////////////////
*/
template<typename _Type>
void Matrix<_Type>::operator-=(const Matrix<_Type>& M)
{
    *this+= (-M);
}

template<typename _Type>
void Matrix<_Type>::operator-=(const _Type& value)
{
    *this+= -value;
}

template<typename _Type>
Matrix<_Type> operator-(const Matrix<_Type>& A, const Matrix<_Type>& B)
{
    return A + (-B);
}

template<typename _Type>
Matrix<_Type> operator-(const Matrix<_Type>& A, const _Type& value)
{
    return A + (-value);
}

template<typename _Type>
Matrix<_Type> operator-(const _Type& value, const Matrix<_Type>& M)
{
    return M + (-value);
}




/*
/////////////////////////////
U N A R Y   O P E R A T O R S
/////////////////////////////
*/

template<typename _Type>
Matrix<_Type> operator+(const Matrix<_Type>& M)
{
    return M;
}

template<typename _Type>
Matrix<_Type> operator-(const Matrix<_Type>& M)
{
    return M * _Type(-1);
}





/*
//////////////////////////
S O M E  P R E D I C A T E 
//////////////////////////
*/
template<typename _Type>
bool Matrix<_Type>::IsMultiplicableWith(const Matrix<_Type>& M) const
{
     return this->Columns()==M.Rows();
}

template<typename _Type>
bool operator==(const Matrix<_Type>& A, const Matrix<_Type>& B)
{
    return A.HasSameSizeWith(B) && 
            A._array2d==B._array2d;
}

template<typename _Type>
bool operator!=(const Matrix<_Type>& A, const Matrix<_Type>& B)
{
    return !(A==B);
}

template<typename _Type>
bool Matrix<_Type>::IsSquareMatrix() const
{
    return this->Rows()==this->Columns() && this->Rows()>0;
}

template<typename _Type>
bool Matrix<_Type>::HasSameSizeWith(const Matrix<_Type>& M) const
{
    return (this->Rows()==M.Rows() && this->Columns()==M.Columns());
}


template<typename _Type>
bool Matrix<_Type>::IsInvertible() const
{
    return (this->IsSquareMatrix()  &&  this->Rank()==this->Rows());
}



/* I/O  O P E R A T I O N S */

//Receive input for ALL elements in the matrix
template<typename _Type>
std::istream& operator>>(std::istream& IS, Matrix<_Type>& M)
{
    Matrix<_Type> tempMatrix(M.Rows(), M.Columns());
    for (size_t iii=0; iii<tempMatrix.Rows(); ++iii)
    {
        
        std::cout<<"Enter row "<<iii<<": ";
        
        for (size_t jjj=0; jjj<tempMatrix.Columns(); ++jjj)
        {
            while (std::isspace(IS.peek()) && IS.peek()!='\n')
            {
                IS.ignore(1);
            }
            if (IS.peek()=='\n')
            {
                IS.ignore(1);
                std::cout<<"<"<<tempMatrix.Columns()-jjj
                        <<" more elements for this rows>\n"
                        <<"Enter row "<<iii<<": ";
                
                //Print out what has already written into the row
                for (int kkk=0; kkk<jjj; ++kkk)
                    std::cout<<tempMatrix.at(iii, kkk)<<"  ";
            }
            IS>>tempMatrix.at(iii, jjj);
        }

        //Check for abundant elements
        while (std::isspace(IS.peek()) && IS.peek()!='\n')
        {
            IS.ignore(1);
        }

        if (IS.peek()!='\n')
        {
            IS.ignore(std::numeric_limits<int>::max(), '\n');
            std::cout<<"Too many elements for one row.\nPlease try again.\n\n";
            //Reinitialize this row to default value of _Type
            tempMatrix.row(iii) = (Matrix<_Type> (1, tempMatrix.Columns(), _Type())).row(0);
            //Reset the count value
            --iii;
        }
        else 
            //Ignore the last '\n'
            IS.ignore(1);
    }   
    M=tempMatrix;
    return IS;
}

//Represent the matrix by a board
template<typename _Type>
std::ostream& operator<<(std::ostream& OS, const Matrix<_Type> &M)
{

    //Find the length of the longest element
    //So that we can format the matrix according to it 
    long long LongestLength(0);
    
    //This matrix represents the length of each elements
    Matrix<size_t> ElementLength(M.Rows(), M.Columns());

    for (long long row=0; row<M.Rows(); ++row)
        for(long long col=0; col<M.Columns(); ++col)
        {
            ElementLength.at(row, col)=M.at(row, col).WriteLength();
            if (ElementLength.at(row, col)>LongestLength)
                LongestLength=ElementLength.at(row, col);
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
                                ElementLength.at(row/2, col)
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
