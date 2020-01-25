//INCLUDED IN array2d.h

/*
----------------------------------------------
----------------------------------------------
|||||||||| C O N S T R U C T O R S |||||||||||
----------------------------------------------
----------------------------------------------
*/

template<typename T>
Array2D<T>::Array2D(): _array2d(std::vector<T>(0)), 
                       _rows(0), 
                       _columns(0)
{
}

template<typename T>
Array2D<T>::Array2D(const Array2D<T> &A): 
                        _array2d(A._array2d), 
                        _rows(A._rows), 
                        _columns(A._columns)
{
}

template<typename T>
Array2D<T>::Array2D(long long rows, long long columns):
                _array2d(std::vector<T>(rows*columns)),
                _rows(rows),
                _columns(columns)
{
}

template<typename T>
Array2D<T>::Array2D(long long rows, long long columns, const T& value):
                _array2d(std::vector<T>(rows*columns)),
                _rows(rows),
                _columns(columns)
{
    for (size_t iii=0; iii<_array2d.size(); ++iii)
        _array2d[iii]=value;
}

/*
----------------------------------------------
----------------------------------------------
|||||||||||||||| G E T T E R S |||||||||||||||
----------------------------------------------
----------------------------------------------
*/

template<typename T>
const size_t Array2D<T>::Rows() const
{
    return _rows;
}
 
template<typename T>
const size_t Array2D<T>::Columns() const
{
    return _columns;
}

template<typename T>
T& Array2D<T>::at(long long row, long long column) 
{
    if (!IsValidCoordinate(row, column))
        throw(Mexception("Out of range element access"));
    
    return _array2d[row*_columns + column];
}

template<typename T>
const T& Array2D<T>::at(long long row, long long column) const
{
    if (!IsValidCoordinate(row, column))
        throw(Mexception("Out of range element access"));
    
    return _array2d[row*_columns + column];
}


template<typename T>
Block<T> Array2D<T>::row(long long row) 
{
    return Block<T>(this, row, true);
}

template<typename T>
Block<T> Array2D<T>::column(long long column)
{
    return Block<T>(this, column, false);
}


/*
----------------------------------------------
----------------------------------------------
||||||||||||| M O D I F I E R S ||||||||||||||
----------------------------------------------
----------------------------------------------
*/
template<typename T>
void Array2D<T>::operator=(const Array2D& a)
{
    _array2d = a._array2d;
    _columns = a._columns;
    _rows    = a._rows;
}


template<typename T>
void Array2D<T>::Resize(long long rows, long long columns)
{
    if (rows<0||columns<0)
        throw(Mexception("Ridiculous size to resize"));


    Array2D<T> replace(rows, columns);

    for (size_t iii=0; iii<rows; ++iii)
        for (size_t jjj=0; jjj<columns; ++jjj)
            if (IsValidCoordinate(iii, jjj))
                replace.at(iii, jjj)=at(iii, jjj);
    
    *this=std::move(replace);
}


/*
----------------------------------------------
----------------------------------------------
||||||||| T H E S E   F U N C T I O N S ||||||
|||||||||  K E E P   T H E   A R R A Y  ||||||
|||||||||   F R O M   B R E A K I N G   ||||||
----------------------------------------------
----------------------------------------------
*/ 

template<typename T>
bool operator==(const Array2D<T>& A, const Array2D<T>& B) 
{
    return A._array2d==B._array2d;
}

template<typename T>
bool operator!=(const Array2D<T>& A, const Array2D<T>& B)
{
    return !(A==B);
}


template<typename T>
bool Array2D<T>::IsValidCoordinate(long long row, long long column) const
{
    return 0<=row && row<= _rows &&
            0<=column && column<_columns;
}