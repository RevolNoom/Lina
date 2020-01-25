#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include "array2d.h"
#include "block.h"
#include "mexception.h"
/*
----------------------------------------------
----------------------------------------------
||||||||||||||||||||||||||||||||||||||||||||||
----------------------------------------------
----------------------------------------------
*/


/*
    Note to developers:

    Because if the matrix is implemented as an array of arrays, 
    resizing matrix may cause all the child-array to be resized,
    and dynamically relocated many times

    So to prevent this, the matrix is implemented as a continuous
    array, with each rows placed after another
*/

template<typename _Type>
class Matrix: public Array2D<_Type>
{
public:
/*
-------------------------------------
-------------------------------------
|||||| C O N S T R U C T O R S ||||||
-------------------------------------
-------------------------------------
*/
    Matrix();
    Matrix(const Matrix<_Type>& m);

    //Initialize a matrix with the given number of rows and columns
    //All elements are 0
    Matrix(long long rows, long long columns);

    //Initialize a matrix with the given number of rows and columns
    //All elements are 0
    Matrix(long long rows, long long columns, const _Type &value);

    template<typename __Type>
    friend Matrix<__Type> Identity(long long order, const __Type& t);



/*
----------------------------------------------
----------------------------------------------
||||||||||||| M O D I F I E R S ||||||||||||||
----------------------------------------------
----------------------------------------------
*/
    //Change the values in one row by the input of the user
    //void ChangeRowValues(long long row);
    //void ChangeRow(long long r, ROW&& row)

    //Change the values in one column by the input of the user
    //void ChangeColumnValues(long long column);
    //void ChangeRow(long long c, COLUMN&& column)

    /* I/O  O P E R A T I O N S */

    template<typename __Type>
    friend std::ostream& operator<<(std::ostream& OS, const Matrix<_Type>& M);
    
    template<typename __Type>
    friend std::istream& operator>>(std::istream& IS, Matrix<_Type>& M);

    //Assign another matrix to this
    Matrix<_Type>& operator=(const Matrix<_Type>& M);


/*
----------------------------------------------
----------------------------------------------
||||||||||| M A T H E M A T I C ||||||||||||||
|||||||||||    T H I N G Y S    ||||||||||||||
----------------------------------------------
----------------------------------------------
*/
    //Return the determinant of the matrix
    const _Type Det() const;

    //Return the sum of all diagonal elements
    const _Type Trace() const;

    //@return the inverse matrix if it's invertible
    //        zero matrix otherwise
    Matrix<_Type> Inverse() const;

    //Return the transpose matrix
    Matrix<_Type> Transpose() const;

    //Return the echelon matrix after using Gaussian elimination
    Matrix<_Type> Echelon() const;

    //Return the rank of the matrix
    const size_t Rank() const;

    //Return the algebraic complement of the specified element
    //Fraction AComplement(long long row, long long column) const;

    /*
        @param Matrix M with same number of rows as this matrix
        @return Augmented matrix [A|M]
    */
    Matrix<_Type> AugmentedWith(const Matrix &M) const;
/*
----------------------------------------------
----------------------------------------------
||||||||||||   M A T R I C E S   |||||||||||||
|||||||||||| O P E R A T I O N S |||||||||||||
----------------------------------------------
----------------------------------------------
*/
    /*ELEMENTARY OPERATIONS*/
    /*
    //Swap two chosen rows of the matrix
    void SwapTwoRows(long long row1, long long row2);
    */
    Block<_Type> row(long long index);
    Block<_Type> column(long long index);

    /*SOME SIMPLE OPERATORS*/
    
    //Unary operators
    
    template<typename __Type>
    friend Matrix<_Type> operator+(const Matrix<_Type>& M);
    
    template<typename __Type>
    friend Matrix<_Type> operator-(const Matrix<_Type>& M);

    //ADDITION
    void operator+=(const Matrix<_Type>& M);
    void operator+=(const _Type &value);
    
    template<typename __Type>
    friend Matrix<_Type> operator+(const Matrix<_Type>& A, const Matrix<_Type>& B);
    
    template<typename __Type>
    friend Matrix<_Type> operator+(const Matrix<_Type>& M, const _Type &value);
    
    template<typename __Type>
    friend Matrix<_Type> operator+(const _Type &value, const Matrix<_Type>& M);

    //MULTIPLICATION
    void operator*=(const Matrix<_Type>& M);
    void operator*=(const _Type &value);
    
    template<typename __Type>
    friend Matrix<_Type> operator*(const Matrix<_Type>& A, const Matrix<_Type>& B);
    
    template<typename __Type>
    friend Matrix<_Type> operator*(const Matrix<_Type>& A, const _Type &value);
    
    template<typename __Type>
    friend Matrix<_Type> operator*(const _Type &value, const Matrix<_Type>& M);

    //SUBTRACTION
    void operator-=(const Matrix<_Type>& M);
    void operator-=(const _Type &value);
    
    template<typename __Type>
    friend Matrix<_Type> operator-(const Matrix<_Type>& A, const Matrix<_Type>& B);
    
    template<typename __Type>
    friend Matrix<_Type> operator-(const Matrix<_Type>& A, const _Type &value);
    
    template<typename __Type>
    friend Matrix<_Type> operator-(const _Type &value, const Matrix<_Type>& M);

    //Not sure if they would prove to be useful or not

    //Return true if each element in matrix A
    //is equivalent to the corresponding element in B
    template<typename __Type>
    friend bool operator==(const Matrix<_Type>& A, const Matrix<_Type>& B);

    //Return true if there's at least one element in matrix A
    //is NOT equivalent to the corresponding element in B
    template<typename __Type>
    friend bool operator!=(const Matrix<_Type>& A, const Matrix<_Type>& B);
    /*


/*
----------------------------------------------------------------
----------------------------------------------------------------
|||||||||||||||   T H E S E   F U N C T I O N S   ||||||||||||||
||||||||||||||| K E E P   T H E   M A T R I C E S |||||||||||||| 
|||||||||||||||     F R O M   B R E A K I N G     ||||||||||||||
----------------------------------------------------------------
----------------------------------------------------------------
*/
    bool IsMultiplicableWith(const Matrix<_Type>& M) const;
    bool IsSquareMatrix() const;
    bool IsInvertible() const;
    bool HasSameSizeWith(const Matrix<_Type>& M) const;
};
template<typename _Type>
Matrix<_Type> Identity(long long order, const _Type &value);

#include "matrix.tpp"
#include "matrix2.tpp"

std::ostream& operator<<(std::ostream& OS, const Matrix<Fraction> &M);
std::ostream& operator<<(std::ostream& OS, const Matrix<double>& M);
#endif /* MATRIX_H */
