/*
    This template is currently very simple and 
    hasn't been through enough up and down
    hence its integrity isn't guaranteed.
    Use with caution.
*/
#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <iostream>
#include <vector>
#include <cstddef>
#include "mexception.h"

template<typename T>
class Block;

template<typename T>
class Array2D
{
protected:
    std::vector<T> _array2d;
    size_t         _rows;
    size_t         _columns;

public:
/*
----------------------------------------------
----------------------------------------------
|||||||||| C O N S T R U C T O R S |||||||||||
----------------------------------------------
----------------------------------------------
*/
    Array2D();
    Array2D(const Array2D<T> &A);
    Array2D(long long rows, long long columns);
    Array2D(long long rows, long long columns, const T& value);

/*
----------------------------------------------
----------------------------------------------
|||||||||||||||| G E T T E R S |||||||||||||||
----------------------------------------------
----------------------------------------------
*/
    //Get the number of Rows this array has
    const size_t Rows() const;

    //Get the number of Columns this array has
    const size_t Columns() const;

    
    //Return a reference to the chosen element
    T& at(long long row, long long column);
    
    //Return a read-only reference to the chosen element
    const T& at(long long row, long long column) const;

    //Return the chosen row
    Block<T> row(long long row);

    //Return the chosen column
    Block<T> column(long long column);
    


/*
----------------------------------------------
----------------------------------------------
||||||||||||| M O D I F I E R S ||||||||||||||
----------------------------------------------
----------------------------------------------
*/
    
    //Assign this array with the given one
    void operator=(const Array2D<T>& A);

   
    /*
        Resize the array to an array with the specified number of rows and columns
        New elements will be copied by the specified value
        Or by default constructor if none is given
    */
    void Resize(long long rows, long long column);

/*
----------------------------------------------
----------------------------------------------
||||||||| T H E S E   F U N C T I O N S ||||||
|||||||||  K E E P   T H E   A R R A Y  ||||||
|||||||||   F R O M   B R E A K I N G   ||||||
----------------------------------------------
----------------------------------------------
*/ 
    //Return true if two arrays hava the same number of columns, rows
    //and value of each element
    template<typename Ty>
    friend bool operator==(const Array2D<Ty>& A, const Array2D<Ty>& B);
    
    //Return true if two arrays don't have the same number of columns, rows
    //or value of each element
    template<typename Ty>
    friend bool operator!=(const Array2D<Ty>& A, const Array2D<Ty>& B);
    
    //Return true if the specified position is in the array
    bool IsValidCoordinate(long long row, long long column) const;


};

#include "array2d.tpp"

#include "block.h"

#endif /*ARRAY2D_H*/
