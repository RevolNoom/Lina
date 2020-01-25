/*
    "Block" is an one-column/row Matrix

    It resembles "Iterator" of the Standard Template Library (STL) of C++ somehow
    That is, it:
    +) Can be used to read / modify the matrix it points to
    +) When the pointed-to object dies, it's extremely dangerous if used inappropriately (reference to delocated memory, something like that)

    The following behaviours are guaranteed when you use Block:
    +) If used as l-value (Left-handside operator)
         any operation on the Block will change the content of the matrix
    +) If used as r-value, any operation on it won't change the pointed-to matrix 
*/

#ifndef BLOCK_H
#define BLOCK_H

#include <algorithm>
#include <vector>
#include "fraction.h"
#include "mexception.h"

template<typename T>
class Matrix;

template<typename T>
class Block: public Matrix<T>
{
protected:
    //The matrix this block is pointing to
    //Mutable-ed to let other Blocks see this address when they copy-construct 
    mutable Matrix<T> *_source;

    //It tells you which column/row of the source matrix you are pointing to
    long long _index;
    
public:
/*
-------------------------------------
-------------------------------------
|||||| C O N S T R U C T O R S ||||||
-------------------------------------
-------------------------------------
*/
    Block();
    Block(const Block<T> &B);
    Block(Matrix<T> *A, long long index, bool IsRow);


/*
----------------------------------------------
----------------------------------------------
|||||||||||||||| G E T T E R S |||||||||||||||
----------------------------------------------
----------------------------------------------
*/
    //Return the copied-content of the block at 'index'
    T& at(long long index);

    //Return the copied-content of the block at 'index'
    const T& at(long long index) const;

    //Return the reference of an element in the matrix
    T& operator[](long long index);

    //Return a read-only reference of an element in the matrix
    const T& operator[](long long index) const;
    
    //Return the index of the row/column being pointed to
    const long long BlockIndex() const;

/*
----------------------------------------------
----------------------------------------------
||||||||||||| O P E R A T O R S ||||||||||||||
----------------------------------------------
----------------------------------------------
*/
    /* T E M P O R A R Y  O P E R A T I O N S */
    /*
        Create a temporary copied block
        Apply operations to the _content of this block
        Then return it
    */

    template<typename T1>
    friend Block<T1> operator+(const Block<T1> &A, const Block<T1> &B);

    template<typename T1>
    friend Block<T1> operator*(const Block<T1> &A, const T1 &value);
    
    template<typename T1>
    friend Block<T1> operator*(const T1 &value, const Block<T1> &A);

    template<typename T1>
    friend Block<T1> operator/(const Block<T1> &A, const T1 &value);
    
    template<typename T1>
    friend Block<T1> operator-(const Block<T1> &A, const Block<T1> &B);

    /* U N A R Y   O P E R A T O R S */
    Block<T> operator+() const;
    Block<T> operator-() const;
    
/* P E R M A N E N T  O P E R A T I O N S */
    /*
        Using these operations will mutate the Matrix being pointed to
    */

    //Assign the content of a Block to another
    void operator=(const Block<T> &B);

    //Swap two rows/columns of a matrix
    template<typename Y>
    friend void swap(Block<Y> A, Block<Y> B);
    
    //Add another row/column to this row/column of the matrix
    void operator+=(const Block<T> &B);
    
    //Multiply this row/column with a value
    void operator*=(const T& value);

    //Divide this row/column by a value
    void operator/=(const T& value);
    
    //Subtract this row/column with another row/column 
    void operator-=(const Block<T> &B);
   
/*
--------------------------------------------
--------------------------------------------
||||      THESE FUNCTIONS ARE USED      ||||
||||TO  KEEP  THE  CLASS  FROM  BREAKING||||
--------------------------------------------
--------------------------------------------
*/
    //Return true if this block is pointing to a row
    bool IsRow() const;
    //Return true if this block is pointing to a column
    bool IsColumn() const;

    //Return true if two blocks have the same size of content
    bool HasSameSizeWith(const Block<T>& B) const;

    // Copy the content of the matrix it points to
    // to its internal content
    void Update();

    /*
        Return a pointer to the matrix2d being pointed to
        If the matrix was delocated, destroyed, a dog hid it away, etc etc... The pointer '_source' will be invalid
        So use it with caution
    */
    Matrix<T>* PointTo() const;

    //Change the targeted matrix of this block
    void PointTo(const Block<T> &B);

    //Return the length of the block 
    const int size() const;

    /* I/O  O P E R A T I O N S */
    template<typename Y>
    friend std::istream& operator>>(std::istream& is, Block<Y> &B);

    template<typename Y>
    friend std::ostream& operator<<(std::ostream& os, const Block<Y> &B);
};

#include "block.tpp"
#include "block2.tpp"

#endif /*BLOCK_H*/