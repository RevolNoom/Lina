template<typename T>
void Block<T>::operator+=(const Block<T> &B)
{
    if (!(this->HasSameSizeWith(B))) 
        throw(Mexception("Addition of two different size blocks"));
    
    for (size_t iii = 0; iii < this->size(); ++iii)
    {
        (*this)[iii] += B.at(iii);
    }

    this->Update();
}



template<typename T>
void Block<T>::operator*=(const T& value)
{
    for (size_t iii = 0; iii < this->size(); ++iii)
    {
        (*this)[iii]*=value;
    }    
    this->Update();
}


template<typename T>
void Block<T>::operator/=(const T& value)
{
    for (size_t iii = 0; iii < this->size(); ++iii)
    {
        (*this)[iii]/=value;
    }    
    this->Update();
}


template<typename T>
void Block<T>::operator-=(const Block<T> &B)
{
    *this += T(-1)*B;
    this->Update();
}


/* T E M P O R A R Y  O P E R A T I O N S */
template<typename T>
Block<T> operator+(const Block<T> &A, const Block<T> &B)
{
    if (!A.HasSameSizeWith(B))
    {
        throw(Mexception("Addition operands of different size"));
    }

    Block<T> result(A);
    for (size_t iii=0; iii<A.size(); ++iii)
        result.at(iii)+=B.at(iii);
    
    return result;
}

template<typename T>
Block<T> operator*(const Block<T> &A, const T& value)
{
    Block<T> result(A);
   
    for (size_t iii=0; iii<A.size(); ++iii)
    {
        result.at(iii)*=value;
    }
    return result;
}

template<typename T>
Block<T> operator*(const T& value, const Block<T> &A)
{
    return A*value;
}

template<typename T>
Block<T> operator/(const Block<T> &A, const T& value)
{
    return A* (1/value);
}

template<typename T>
Block<T> operator-(const Block<T> &A, const Block<T> &B)
{
    return A + B*(-1); 
}

template<typename T>
Block<T> Block<T>::operator+() const
{
    return *this;
}

template<typename T>
Block<T> Block<T>::operator-() const
{
    return *this * (-1);
}
