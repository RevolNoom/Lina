/*
    FOR DECLARATION AND DEFINITIONS OF BASIC FUNCTIONS
    THAT MAKE MATRIX WORKS, PLEASE REFERS TO 
    "matrix.h" AND "matrix.tpp"
*/
template<typename _Type>
Matrix<_Type> Matrix<_Type>::Inverse() const
{
    if (!this->IsSquareMatrix())
        throw(Mexception("Only square matrices have inverse"));
    
    /*
        Inverse matrix is found by merging (augmenting) the original matrix
        with an identity of same size into [A|I]

        Then performs elementary operations on the matrix, until we get an identity on
        the left, [I|B]. B is then the inverse matrix of A

        That's why for every change in matrix temp
        matrix inv will have a corresponding change
    */

    Matrix<_Type> inv=Identity(this->Rows(), (_Type)(1));
    Matrix<_Type> temp(*this);

    for (size_t cur=0; cur<this->Rows(); ++cur)
    {   
        //This row has the diagonal element = 0
        //So we have to swap it with another one
        if (temp.at(cur, cur)==0)
            for (size_t r=cur+1; r<this->Rows(); ++r)
            {
                if (temp.at(r, cur)!=0)
                {
                    swap(temp.row(r), temp.row(cur));
                    swap(inv.row(r), inv.row(cur));
                    break;
                }
            }
            // std::cout<<"After swapping:\n";
            // std::cout<<temp<<"\n"<<inv<<"\n";

        /*  After looking for elements differ from zero
            We have none.
            So we cannot turn the original matrix into an identity
        
            As a result, zero matrix is returned
        */
        if (temp.at(cur, cur)==0)
            return Matrix<_Type>();



        //Transform this row so that it has the element at (iii, iii) is 1
        _Type Normalize= temp.at(cur, cur);
        // std::cout<<"Normalize: "<<Normalize<<"\n";
        // std::cout<<"inv.row("<<cur<<") before:\n";
        // std::cout<<(Matrix<_Type>)(inv.row(cur))<<"\n";
        inv.row(cur)/=Normalize;

        // std::cout<<"inv.row("<<cur<<") after:\n";
        // std::cout<<(Matrix<_Type>)(inv.row(cur))<<"\n";

        temp.row(cur)/=Normalize;


        //std::cout<<"After transformation:\n";
        //std::cout<<temp<<"\n"<<inv<<"\n";
    
        // Start eliminating so that all elements on this column (except the diagonal one) 
        // are zeroes.

        //std::cout<<"Eliminating: \n";
        for (size_t r=0; r<this->Rows(); ++r)
        {
            // std::cout<<"r="<<r<<"\n";
            if (r!=cur)
            {
                _Type Multiplier= temp.at(r, cur);
                // std::cout<<"temp Before: \n";
                // std::cout<<temp;

                // std::cout<<"temp After: \n";
                temp.row(r)-= temp.row(cur)*Multiplier;
                // std::cout<<temp;

                
                // std::cout<<"inv Before: \n";
                // std::cout<<inv;

                // std::cout<<"inv After: \n";

                inv.row(r)-= inv.row(cur)*Multiplier;
                // std::cout<<inv;

            }
        }
            // std::cout<<"After elimination:\n";
            // std::cout<<temp<<"\n"<<inv<<"\n<><><><><><><><>\n";

    }
    
    return inv;
}

template<typename _Type>
const size_t Matrix<_Type>::Rank() const
{
    //Echelonize the matrix
    //The rank of the matrix is the number of rows
    //That's not zero row
    Matrix<_Type> M=Echelon();
    for (size_t iii=this->Rows()-1; iii>=0; --iii)
        for (size_t jjj=M.Columns()-1; jjj>=0; --jjj)
            if (M.at(iii, jjj)!=0)
                return iii+1;
    return 0;
}

template<typename _Type>
const _Type Matrix<_Type>::Det() const
{
    if (!this->IsSquareMatrix())
    {
        std::cerr<<"Only square matrices have determinant";
        throw("Only square matrices have determinant");
    }

    //Turn this matrix into an echelon matrix for easier calculation
    Matrix<_Type> M = this->Echelon();
    _Type result(1);

    //An echelon matrix has determinant equals to 
    //The product of all diagonal elements
    for (size_t iii=0; iii<this->Rows(); ++iii)
        result*=M.at(iii, iii);
    
    return result;
}

template<typename _Type>
const _Type Matrix<_Type>::Trace() const
{
    if (!this->IsSquareMatrix())
    {
        std::cerr<<"Dimensional Error";
        throw("Dimensional Error");
    }
    _Type sum(0);
    for (long long iii=0; iii<this->Rows(); ++iii)
        sum+= this->at(iii, iii);
    
    return sum;
}


template<typename _Type>
Matrix<_Type> Matrix<_Type>::Echelon() const
{
    //Create a copy of our current Matrix and work on it
    Matrix<_Type> result = *this;

    /*  
        This variable flips sign of a number
        to help the calculation of determinant

        Swapping rows will resulting in 
        sign of matrix determinant be flipped
        So, in the final step, we'll flip the sign of a row
        Depend on the number of times swapped
    */
    int flip(1);

    //Using Gaussian elimination to calculate echelon matrix
    for (size_t cur=0; cur<this->Rows() && cur<this->Columns(); ++cur)
    {
        /*
            See if this row has the element on the diagonal = 0
           
            If it does, we have to swap this row with another one in order to
            eliminate any non-zero elements in this column
        */
        
        if (result.at(cur, cur)==0)
            for (size_t col=cur+1; col<this->Rows(); ++col)
            {
                if (result.at(col, cur)!=0)
                {
                    swap(result.row(col), result.row(cur));
                    flip*=-1;
                    break;
                }
            }
        /*
            If after looking for non-zero element and we have none,
            It means this row is already echelon-ized
            So we ignore it
        */
        if (result.at(cur, cur)==0)
            continue;

        //Start eliminating
        for (size_t r=cur+1; r<this->Rows(); ++r)
        {
            result.row(r)-= result.row(cur)*
                                (result.at(r, cur)/result.at(cur, cur));
       }
    }

    if (flip==-1 && result.Columns() && result.Rows())
        result.row(0)*=-1; 

    return result;
}

template<typename _Type>
Matrix<_Type> Matrix<_Type>::Transpose() const
{
    Matrix<_Type> result(this->Columns(), this->Rows());
    for (long long iii=0; iii<this->Rows(); ++iii)
        for (long long jjj=0; jjj<this->Columns(); ++jjj)
            result.at(jjj, iii)= this->at(iii, jjj);
    
    return result;
}

//Fraction Matrix::AComplement(long long row, long long column);

template<typename _Type>
Matrix<_Type> Matrix<_Type>::AugmentedWith(const Matrix<_Type> &M) const
{
    if (this->Rows()!=M.Rows())
    {
        std::cerr<<"Incompatible size augmenting matrix";
        throw("Incompatible size augmenting matrix");
    }

    Matrix<_Type> result(*this);
    result.Resize(this->Rows(), this->Columns()+M.Columns());

    //Write the M matrix to where it belongs
    for (size_t iii=0; iii<M.Rows(); ++iii)
        for(size_t jjj=0; jjj<M.Columns(); ++jjj)
        {
            result.at(iii, jjj+this->Columns())=M.at(iii, jjj);
        }
    return result;
}