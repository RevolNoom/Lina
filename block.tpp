/*
-------------------------------------
-------------------------------------
|||||| C O N S T R U C T O R S ||||||
-------------------------------------
-------------------------------------
*/

template<typename T>
Block<T>::Block():  _source(nullptr),
                    _index(-1)
{
}


template<typename T>
Block<T>::Block(const Block<T> &B): Matrix<T>(B),
                                    _source(B._source),
                                    _index(B._index)                                                                      
{
}

template<typename T>
Block<T>::Block(Matrix<T> *A, long long index, bool IsRow):
                          _source(A)
{
    if (index<0) 
    {
        std::cerr<<"Invalid block index";
        throw(Mexception("Invalid block index"));
    }

    this->_index=index;

    if (IsRow)
    {
        if (A->Rows()<=BlockIndex())
        {
            std::cerr<<"Invalid block index";
            throw(Mexception("Invalid block index"));
        }

        this->Resize(1, A->Columns());
        for (size_t iii=0; iii < A->Columns(); ++iii)
            this->at(iii)=A->at(BlockIndex(), iii);
    }
    else
    {
        if (A->Columns()<=BlockIndex())
        {
            std::cerr<<"Invalid block index";
            throw(Mexception("Invalid block index"));
        }
        this->Resize(A->Rows(), 1);
        for (size_t iii=0; iii < A->Rows(); ++iii)
            this->at(iii)=A->at(iii, BlockIndex());
    }
}


/*
----------------------------------------------
----------------------------------------------
|||||||||||||||| G E T T E R S |||||||||||||||
----------------------------------------------
----------------------------------------------
*/


template<typename T>
T& Block<T>::at(long long index)
{
    //Test for out of range indices
    if (index<0 || index>=this->size())
    {
        std::cerr<<"Invalid index";
        throw ("Invalid index");
    }
    if (this->IsRow())
        return this->Array2D<T>::at(0, index);
    return this->Array2D<T>::at(index, 0);
}

template<typename T>
const T& Block<T>::at(long long index) const
{
    //Test for out of range indices
    if (index<0 || index>=this->size())
    {
        std::cerr<<"Invalid index";
        throw ("Invalid index");
    }
    if (this->IsRow())
        return this->Array2D<T>::at(0, index);
    return this->Array2D<T>::at(index, 0);
}

template<typename T>
T& Block<T>::operator[](long long index)
{
    if (IsColumn())
        return _source->at(index, BlockIndex());
    else
        return _source->at(BlockIndex(), index);
}

template<typename T>
const T& Block<T>::operator[](long long index) const
{
    if (IsColumn())
        return _source->at(index, BlockIndex());
    else
        return _source->at(BlockIndex(), index);
}


template<typename T>
const long long Block<T>::BlockIndex() const
{
    return _index;
}

/*
----------------------------------------------
----------------------------------------------
||||||||||||| O P E R A T O R S ||||||||||||||
----------------------------------------------
----------------------------------------------
*/


 /* P E R M A N E N T  O P E R A T I O N S */

template<typename T>
void Block<T>::operator=(const Block<T> &B)
{
    if (this->size()!=B.size())
        throw(Mexception("Assignment of different size blocks"));
    
    for (size_t iii=0; iii<this->size(); ++iii)
    {
        (*this)[iii]=this->at(iii)=B.at(iii);
    }
    /*
        Using Update() here will accidentally 
        make a recursive call to operator=

        So DO NOT update anything
    */
   //Update();

}   

template<typename T>
void swap(Block<T> A, Block<T> B)
{
    if (!A.HasSameSizeWith(B))
        throw(Mexception("Swapping different size blocks"));

    for (size_t iii=0; iii<A.size(); ++iii)
        std::swap(A[iii], B[iii]);

    A.Update();
    B.Update();
}


template<typename T>
Matrix<T>* Block<T>::PointTo() const
{
    return this->_source;
}


template<typename T>
void Block<T>::PointTo(const Block<T> &B)
{
    this->_source = B._source;
    this->_index  = B._index;
}

/*
--------------------------------------------
--------------------------------------------
||||      THESE FUNCTIONS ARE USED      ||||
||||TO  KEEP  THE  CLASS  FROM  BREAKING||||
--------------------------------------------
--------------------------------------------
*/
template<typename T>
bool Block<T>::IsColumn() const
{
    return this->Columns()==1;
}


template<typename T>
bool Block<T>::IsRow() const
{
    return this->Rows()==1;
}

template<typename T>
bool Block<T>::HasSameSizeWith(const Block<T> &B) const
{
    return this->size()== B.size();
}

template<typename T>
const int Block<T>::size() const
{
    if (this->IsRow())
        return this->Columns();
    return this->Rows();
}

template<typename T>
void Block<T>::Update()
{

    *this = this->IsRow()?  
                _source->row(BlockIndex())
                                            : _source->column(BlockIndex());
}


/* I/O  O P E R A T I O N S 
template<typename T>
std::istream& operator>>(std::istream& is, Block<T> &b)
{
    for (size_t iii=0; iii<b.size(); ++iii)
        is>>b[iii];
    return is;
}*/