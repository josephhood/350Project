#pragma once

#include <iterator>
#include <numeric>
#include <stdexcept>
#include <ostream>


template<typename T>
class Vector final
{
public:
    #pragma region Constructors and Destructors
    Vector(size_t size, const T& initialValue = T{})
    : _size(0), _data(nullptr)
    {
    resize(size);
    initialize(initialValue);
    }

    Vector(const Vector<T>& original)
    : _size(0), _data(nullptr)
    {
    *this = original;
    }

    Vector(Vector<T>&& old)
    : _size(old._size), _data(old._data)
    {
    old._size = 0;
    old._data = nullptr;
    }

    ~Vector()
    {
    dispose();
    }
    #pragma endregion

    #pragma region Modifiers
    void initialize(const T& initialValue)
    {
    for (size_t i = 0; i < _size; ++i)
        _data[i] = initialValue;
    }
    #pragma endregion

    #pragma region Observers
    size_t size() const
    {
    return _size;
    }

    bool empty() const
    {
    return _size == 0;
    }

    T dotProduct(const Vector<T>& rhs) const
    {
    checkMatchedSize(rhs);

    T product = T{};
    for (size_t i = 0; i < _size; ++i)
        product += _data[i] * rhs[i];
    return product;
    }
    #pragma endregion

    #pragma region Operators
    #pragma region Observers
    const T& operator[](size_t index) const
    {
    checkBounds(index);

    return _data[index];
    }

    Vector<T> operator-() const
    {
    Vector<T> negated;
    negated.resize(_size);
    for (size_t i = 0; i < _size; ++i)
        negated[i] = -_data[i];
    return negated;
    }

    Vector<T> operator+(const Vector<T>& rhs) const
    {
    checkMatchedSize(rhs);

    Vector<T> sum(*this);
    sum += rhs;
    return sum;
    }

    Vector<T> operator-(const Vector<T>& rhs) const
    {
    checkMatchedSize(rhs);

    Vector<T> difference(*this);
    difference -= rhs;

    return difference;
    }

    Vector<T> operator*(const T& scalar) const
    {
    Vector<T> product(*this);
    product *= scalar;

    return product;
    }

    Vector<T> operator/(const T& scalar) const
    {
    Vector<T> quotient(*this);
    quotient /= scalar;

    return quotient;
    }

    bool operator==(const Vector<T>& rhs) const
    {
    if (_size != rhs._size)
        return false;

    for (size_t i = 0; i < _size; ++i)
        if (_data[i] != rhs[i])
        return false;

    return true;
    }

    bool operator!=(const Vector<T>& rhs) const
    {
    return !(*this == rhs);
    }
    #pragma endregion

    #pragma region Modifiers
    T& operator[](size_t index)
    {
    return const_cast<T&>(const_cast<const Vector<T>*>(this)->operator[](index));
    }

    Vector<T>& operator+=(const Vector<T>& rhs)
    {
    checkMatchedSize(rhs);

    for (size_t i = 0; i < _size; ++i)
        _data[i] += rhs[i];

    return *this;
    }

    Vector<T>& operator-=(const Vector<T>& rhs)
    {
    checkMatchedSize(rhs);

    for (size_t i = 0; i < _size; ++i)
        _data[i] -= rhs[i];

    return *this;
    }

    Vector<T>& operator*=(const T& scalar)
    {
    for (size_t i = 0; i < _size; ++i)
        _data[i] *= scalar;

    return *this;
    }

    Vector<T>& operator/=(const T& scalar)
    {
    for (size_t i = 0; i < _size; ++i)
        _data[i] /= scalar;

    return *this;
    }

    Vector<T>& operator=(const Vector<T>& rhs)
    {
    if (_size != rhs._size)
    {
        dispose();
        resize(rhs.size());
    }

    for (size_t i = 0; i < rhs._size; ++i)
        _data[i] = rhs[i];

    return *this;
    }

    Vector<T>& operator=(Vector<T>&& old)
    {
    dispose();
    _size = old._size;
    _data = old._data;

    old._size = 0;
    old._data = nullptr;

    return *this;
    }
    #pragma endregion
    #pragma endregion
private:
    #pragma region Variables
    size_t _size;
    T* _data;
    #pragma endregion

    #pragma region Constructors
    Vector()
    : _size(0), _data(nullptr)
    {
    }
    #pragma endregion

    #pragma region Modifiers
    void resize(size_t size)
    {
    _size = size;
    _data = new T[size];
    }

    void dispose()
    {
    if (_data)
    {
        delete[] _data;
        _data = nullptr;
        _size = 0;
    }
    }

    void checkBounds(size_t index) const
    {
    if (index >= _size)
        throw std::out_of_range("index is out of range");
    }

    void checkMatchedSize(const Vector<T>& vector) const
    {
    if (_size != vector._size)
        throw std::domain_error("Vectors must have the same size");
    }
    #pragma endregion
};

#pragma region Operators
template<typename T>
std::ostream& operator<<(std::ostream& stream, const Vector<T>& vector)
{
    stream << "[ ";
    for (size_t i = 0; i < vector.size(); ++i)
    stream << vector[i] << ' ';
    stream << ']';

    return stream;
}
#pragma endregion