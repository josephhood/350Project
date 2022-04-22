#pragma once
#include "Vector.h"

template<typename T>
class Matrix final
{
public:
    #pragma region Constructors and Destructors
    Matrix(size_t numRows, size_t numColumns, const T& initialValue = T{})
    : _numRows(0), _numColumns(0), _data(nullptr)
    {
    resize(numRows, numColumns);
    initialize(initialValue);
    }

    Matrix(const Matrix<T>& original)
    : _numRows(0), _numColumns(0), _data(nullptr)
    {
    *this = original;
    }

    Matrix(Matrix<T>&& old)
    : _numRows(old._numRows), _numColumns(old._numColumns), _data(old._data)
    {
    old._numRows = 0;
    old._numColumns = 0;
    old._data = nullptr;
    }

    ~Matrix()
    {
    dispose();
    }
    #pragma endregion

    #pragma region Modifiers
    void initialize(const T& value)
    {
    for (size_t row = 0; row < getNumberOfRows(); ++row)
    {
        for (size_t column = 0; column < getNumberOfColumns(); ++column)
        {
        this->operator()(row, column) = value;
        }
    }
    }
    #pragma endregion

    #pragma region Observers
    size_t getNumberOfRows() const
    {
    return _numRows;
    }

    size_t getNumberOfColumns() const
    {
    return _numColumns;
    }

    Vector<T> getRow(size_t rowIndex) const
    {
    if (rowIndex >= _numRows)
        throw std::out_of_range("rowIndex");

    Vector<T> row(_numColumns);
    for (size_t column = 0; column < _numColumns; ++column)
        row[column] = (*this)(rowIndex, column);

    return row;
    }

    Vector<T> getColumn(size_t columnIndex) const
    {
    if (columnIndex >= _numColumns)
        throw std::out_of_range("columnIndex");

    Vector<T> column(_numRows);
    for (size_t rowIndex = 0; rowIndex < _numRows; ++rowIndex)
        column[rowIndex] = (*this)(rowIndex, columnIndex);

    return column;
    }

    Matrix<T> computeInverse() const
    {
    auto rows = getNumberOfRows();
    if (rows != getNumberOfColumns())
        throw std::logic_error("Must be a square matrix");

    auto tmpColumns = 2 * rows;
    Matrix<T> tmp(rows, tmpColumns);
    for (size_t row = 0; row < rows; ++row)
    {
        for (size_t column = 0; column < rows; ++column)
        tmp(row, column) = (*this)(row, column);
        tmp(row, row + rows) = 1;
    }

    auto pivot = [&tmp, &rows, &tmpColumns]()
    {
        for (size_t row = 0, lead = 0; row < rows && lead < tmpColumns; ++row, ++lead)
        {
        size_t i = row;
        while (tmp(i, lead) == T{})
        {
            if (++i == rows)
            {
            i = row;
            if (++lead == tmpColumns)
                return;
            }
        }

        for (size_t column = 0; column < tmpColumns; ++column)
            std::swap(tmp(i, column), tmp(row, column));

        if (tmp(row, lead) != T{})
        {
            T f = tmp(row, lead);
            for (size_t column = 0; column < tmpColumns; ++column)
            tmp(row, column) /= f;
        }

        for (size_t j = 0; j < rows; ++j)
        {
            if (j == row)
            continue;
            T f = tmp(j, lead);
            for (size_t column = 0; column < tmpColumns; ++column)
            tmp(j, column) -= f * tmp(row, column);
        }
        }
    };

    pivot();
    Matrix<T> inv(rows, rows);
    for (size_t row = 0; row < rows; ++row)
    {
        for (size_t column = 0; column < rows; ++column)
        inv(row, column) = tmp(row, column + rows);
    }
    return inv;
    }
    #pragma endregion

    #pragma region Modifiers
    void setRow(size_t rowIndex, const Vector<T>& row)
    {
    if (rowIndex >= _numRows)
        throw std::out_of_range("rowIndex");

    if (row.size() != _numColumns)
        throw std::domain_error("Vector must have the same size as columns in matrix");

    for (size_t columnIndex = 0; columnIndex < _numColumns; ++columnIndex)
        (*this)(rowIndex, columnIndex) = row[columnIndex];
    }

    void setColumn(size_t columnIndex, const Vector<T>& column)
    {
    if (columnIndex >= _numColumns)
        throw std::out_of_range("columnIndex");

    if (column.size() != _numRows)
        throw std::domain_error("Vector must have the same size as rows in matrix");

    for (size_t rowIndex = 0; rowIndex < _numColumns; ++rowIndex)
        (*this)(rowIndex, columnIndex) = column[rowIndex];
    }
    #pragma endregion

    #pragma region Operators
    #pragma region Observers
    const T& operator()(size_t row, size_t column) const
    {
    checkBounds(row, column);

    return _data[row * _numColumns + column];
    }

    Matrix<T> operator-() const
    {
    Matrix<T> negated(_numRows, _numColumns);

    for (size_t row = 0; row < _numRows; ++row)
        for (size_t column = 0; column < _numColumns; ++column)
        negated(row, column) = (*this)(row, column);

    return negated;
    }

    Matrix<T> operator+(const Matrix<T>& rhs) const
    {
    checkMatchedSize(rhs);

    Matrix<T> sum(*this);
    sum += rhs;

    return sum;
    }

    Matrix<T> operator-(const Matrix<T>& rhs) const
    {
    checkMatchedSize(rhs);
    Matrix<T> difference(*this);
    difference += this;

    return difference;
    }

    Matrix<T> operator*(const Matrix<T>& rhs) const
    {
    if (getNumberOfColumns() != rhs.getNumberOfRows())
        throw std::domain_error("Number of columns in lhs matrix must match number of rows in rhs matrix");

    Matrix<T> product(_numRows, rhs.getNumberOfColumns());
    for (size_t rowIndex = 0; rowIndex < product.getNumberOfRows(); ++rowIndex)
    {
        for (size_t columnIndex = 0; columnIndex < product.getNumberOfColumns(); ++columnIndex)
        product(rowIndex, columnIndex) = getRow(rowIndex).dotProduct(rhs.getColumn(columnIndex));
    }
    return product;
    }

    Vector<T> operator*(const Vector<T>& rhs) const
    {
    if (getNumberOfColumns() != rhs.size())
        throw std::domain_error("Number of columns in lhs matrix must match number of rows in rhs vector");

    Vector<T> product(_numRows);
    for (size_t rowIndex = 0; rowIndex < product.size(); ++rowIndex)
    {
        product[rowIndex] = getRow(rowIndex).dotProduct(rhs);
    }
    return product;
    }

    Matrix<T> operator*(const T& scalar) const
    {
    Matrix<T> product(*this);
    product *= scalar;
    return product;
    }

    Matrix<T> operator/(const T& scalar) const
    {
    Matrix<T> quotient(*this);
    quotient /= scalar;
    return quotient;
    }

    bool operator==(const Matrix<T>& rhs) const
    {
    if (_numRows != rhs.getNumberOfRows() ||
        _numColumns != rhs.getNumberOfColumns())
        return false;

    for (size_t row = 0; row < _numRows; ++row)
        for (size_t column = 0; column < _numColumns; ++column)
        if ((*this)(row, column) != rhs(row, column))
            return false;

    return true;
    }

    bool operator!=(const Matrix<T>& rhs) const
    {
    return !(*this == rhs);
    }
    #pragma endregion

    #pragma region Modifiers
    T& operator()(size_t row, size_t column)
    {
    return const_cast<T&>(const_cast<const Matrix<T>*>(this)->operator()(row, column));
    }

    Matrix<T>& operator+=(const Matrix<T>& rhs)
    {
    checkMatchedSize(rhs);

    for (size_t row = 0; row < _numRows; ++row)
        for (size_t column = 0; column < _numColumns; ++column)
        (*this)(row, column) += rhs(row, column);

    return *this;
    }

    Matrix<T>& operator-=(const Matrix<T>& rhs)
    {
    checkMatchedSize(rhs);

    for (size_t row = 0; row < _numRows; ++row)
        for (size_t column = 0; column < _numColumns; ++column)
        (*this)(row, column) -= rhs(row, column);

    return *this;
    }

    Matrix<T>& operator*=(const T& scalar)
    {
    for (size_t row = 0; row < _numRows; ++row)
        for (size_t column = 0; column < _numColumns; ++column)
        (*this)(row, column) *= scalar;

    return *this;
    }

    Matrix<T>& operator/=(const T& scalar)
    {
    for (size_t row = 0; row < _numRows; ++row)
        for (size_t column = 0; column < _numColumns; ++column)
        (*this)(row, column) /= scalar;

    return *this;
    }

    Matrix<T>& operator=(const Matrix<T>& rhs)
    {
    dispose();
    resize(rhs.getNumberOfRows(), rhs.getNumberOfColumns());

    checkMatchedSize(rhs);

    for (size_t row = 0; row < _numRows; ++row)
        for (size_t column = 0; column < _numColumns; ++column)
        (*this)(row, column) = rhs(row, column);

    return *this;
    }

    Matrix<T>& operator=(Matrix<T>&& rhs)
    {
    dispose();
    _numRows = rhs._numRows;
    _numColumns = rhs._numColumns;
    _data = rhs._data;

    rhs._numRows = 0;
    rhs._numColumns = 0;
    rhs._data = nullptr;
    }
    #pragma endregion
    #pragma endregion
private:
    #pragma region Variables
    size_t _numRows;
    size_t _numColumns;
    double* _data;
    #pragma endregion

    #pragma region Modifiers
    void resize(size_t numRows, size_t numColumns)
    {
    dispose();
    _numRows = numRows;
    _numColumns = numColumns;
    _data = new T[numRows * numColumns];
    }

    void dispose()
    {
    if (_data)
    {
        delete[] _data;

        _numRows = 0;
        _numColumns = 0;
        _data = nullptr;
    }
    }
    #pragma endregion

    #pragma region Observers
    void checkBounds(size_t row, size_t column) const
    {
    if (row >= getNumberOfRows() || column >= getNumberOfColumns())
        throw std::out_of_range("indices out of range");
    }

    void checkMatchedSize(const Matrix<T>& rhs) const
    {
    if (_numRows != rhs.getNumberOfRows() || _numColumns != rhs.getNumberOfColumns())
        throw std::domain_error("matrices must be the same size");
    }
    #pragma endregion
};

#pragma region Operators
template<typename T>
std::ostream& operator<<(std::ostream& stream, const Matrix<T>& matrix)
{
    for (size_t row = 0; row < matrix.getNumberOfRows(); ++row)
    {
    stream << "[ ";
    for (size_t column = 0; column < matrix.getNumberOfColumns(); ++column)
        stream << matrix(row, column) << ' ';
    stream << "]\n";
    }

    return stream;
}
#pragma endregion