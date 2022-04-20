/*
MIT License

Copyright (c) 2017  Joe Hood

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <vector>
#include <complex>
#include <cmath>

template<class T = double> class matrix;
template<class T = double> class columnVector;
template<class T = double> class rowVector;
template<class T = double> class LU;

typedef matrix<double> Matrix;
typedef columnVector<double> ColumnVector;
typedef rowVector<double> RowVector;

typedef std::complex<double> Complex;

typedef matrix< std::complex<double> > ComplexMatrix;
typedef columnVector< std::complex<double> > ComplexColumnVector;
typedef rowVector< std::complex<double> > ComplexRowVector;

/**
generic 2-d dense rectangular matrix

@ingroup math
**/

template<class T> class matrix
{

protected:

    int numRows;                    ///<number of rows in matrix
    int numColumns;                 ///<number of columns in matrix
    bool isColumnVector;            ///<indicates if column vector
    bool isRowVector;               ///<indicates if row vector
    std::vector<T> elements;        ///<elements of the matrix
    T dummy;                        ///<dummy value for out-of-bounds access

public:

    /**
    default constructor
    **/
    matrix();

    /**
    copy constructor
    **/
    matrix(const matrix<T>& base);

    /**
    upcast constructor from columnVector
    **/
    matrix(const columnVector<T>& base);

    /**
    upcast constructor from rowVector
    **/
    matrix(const rowVector<T>& base);
    
    /**
    upcast constructor from single element (1x1 matrix)
    **/
    matrix(const T& element);

    /**
    parameterized constructor
    **/
    matrix(const int rows, const int columns);

    /**
    parameterized constructor for square matrix
    **/
    matrix(const int size);

    /**
    indicates if matrix is a column vector
    **/
    bool IsColumnVector() const;

    /**
    indicates if matrix is a row vector
    **/
    bool IsRowVector() const;

    /**
    sets size of matrix
    **/
    void SetSize(const int rows, const int columns);

    /**
    clears the matrix
    **/
    void Clear();

    /**
    gets number of rows in matrix
    **/
    int GetNumRows() const;

    /**
    gets number of columns in matrix
    **/
    int GetNumColumns() const;

    /**
    gets number of matrix elements
    **/
    int GetNumElements() const;

    /**
    gets an element from matrix
    **/
    T GetElement(int row, int column) const;

    /**
    sets an element of matrix to particular value
    **/
    void SetElement(int row, int column, T value);

    /**
    sets matrix to become identity
    **/
    void SetAsEye();


    ////////////////////// Equality /////////////////


    /**
    compares operator
    **/
    bool operator==(const matrix<T>&) const;


    ////////////////// Add ///////////////////////


    /**
    matrix assignment add operator
    **/
    matrix<T>& operator+=(const matrix<T>& B);

    /**
    matrix add operator
    **/
    matrix<T> operator+(const matrix<T>& B) const
    {
        return matrix<T>(*this).operator+=(B);
    }


    ////////////////// Subtract ///////////////////////


    /**
    matrix assignment subtract operator
    **/
    matrix<T>& operator-=(const matrix<T>& B);

    /**
    matrix subtract operator
    **/
    matrix<T> operator-(const matrix<T>& B) const
    {
        return matrix<T>(*this).operator-=(B);
    }

    /**
    negation operator
    **/
    matrix<T> operator-() const;


    ////////////////// Transpose ///////////////////////


    /**
    Transpose operator
    **/
    matrix<T> operator~() const;


    ////////////////// Augment ///////////////////////


    /**
    matrix augment operator
    **/
    matrix<T> operator|(const matrix<T>& B) const;


    ////////////////// Multiply by scalar ///////////////////////


    /**
    assignment multiply operator
    **/
    matrix<T>& operator*=(const T k);

    /**
    matrix dot product operator for scalar
    **/
    matrix<T> operator*(const T k) const
    {
        return matrix<T>(*this).operator*=(k);
    }

    ////////////////// Divide by scalar ///////////////////////


    /**
    assignment divide operator
    **/
    matrix<T>& operator/=(const T k);

    /**
    matrix operator for divide
    **/
    matrix<T> operator/(const T k) const
    {
        return matrix<T>(*this).operator/=(k);
    }


    ////////////////// Inner matrix Product (Dot) /////////////////


    /**
    assignment dot operator
    **/
    matrix<T> operator*(const matrix<T>& B) const;


    /**
    matrix dot operator for columnVector
    **/
    columnVector<T> operator*(const columnVector<T>& B) const;


    ///////////////////// Row/Column Operations /////////////////////


    /**
    interchanges two rows of matrix
    **/
    void InterchangeRows(int row1, int row2);

    /**
    sets elements of a row to given values
    **/
    void SetRow(int row, T* values);

    /**
    replaces a column by given column matrix
    **/
    void ReplaceColumn(int column, const columnVector<T>& C);

    /**
    replaces a row by given row matrix
    **/
    void ReplaceRow(int row, const rowVector<T>& R);

    /**
    partitions the matrix into left-hand and right-hand side matrices
    **/
    matrix<T> Partition(int afterWhichColumn); //returns RHS of partition, original matrix now contains LHS

                                               /**
                                               gets a row from matrix
                                               **/
    rowVector<T> ExtractRow(int row) const;

    /**
    gets a column from matrix
    **/
    columnVector<T> ExtractColumn(int column) const;


    /////////////////////////// Inverse/Factorization ///////////////////////


    /**
    computes the inverse of a matrix
    **/
    matrix<T> GetInverse(const matrix<T>& A, const LU<T>& LUFactor);

    /**
    computes the inverse of this matrix
    **/
    matrix<T> GetInverse(const LU<T>& LUFactor) const;
    
    /**
    computes the inverse of this matrix
    **/
    matrix<T> GetInverse() const;

    /**
    computes the LU factorization of the matrix
    **/
    LU<T> GetLU() const;

    /**
    matrix exponent operator (only supports -1 for inverse)
    **/
    matrix<T> operator^(const T e) const;


    //////////////////////////////// Solve //////////////////////////////


    /**
    computes the result of a linear system given A, B, and LU Factors
    **/
    matrix<T> Solve(const matrix<T>& A, const matrix<T>& B, const LU<T>& LUFactor) const;

    /**
    solve linear system for A matrix given B vector, and LU Factors
    **/
    columnVector<T> Solve(const columnVector<T>& B, const LU<T>& LUFactor) const;

    /**
    computes the columnVector result of this linear system
    **/
    columnVector<T> LeftDivide(const columnVector<T>& B) const;


    /////////////////// Subscript Operators ///////////////////////


    /**
    allows matrix to be accessed like 2D array with () operator
    **/
    T& operator()(int row, int column);

    /**
    allows matrix to be accessed like constant 2D array with () operator
    **/
    const T& operator()(int row, int column) const;

    /**
    allows Column or Row Vector to be accessed like 1D array with () operator
    **/
    T& operator()(int index);

    /**
    allows Column or Row Vector to be accessed like constant 1D array with () operator
    **/
    const T& operator()(int index) const;


    //////////////////////////// Print ///////////////////////////////////////////


    /**
    pretty prints matrix to stdout
    **/
    void Print() const;


    ///////////////////////////// Assignment //////////////////////////////////

    /**
    Assignment operator
    **/
    matrix<T>& operator=(const matrix<T>& B);

};


/**
Column Vector Type

@ingroup math
**/

template<class T> class columnVector : public matrix<T>
{

public:

    /**
    default constructor
    **/
    columnVector();

    /**
    copy constructor
    **/
    columnVector(const columnVector<T>& base);

    /**
    downcast constructor
    **/
    columnVector(const matrix<T>& base);

    /**
    constructor that accepts size (number of rows)
    **/
    explicit columnVector(int size);


    /**
    columnVector assignment add operator
    **/
    columnVector<T>& operator+=(const columnVector<T>& B);

    /**
    columnVector assignment subtract operator
    **/
    columnVector<T>& operator-=(const columnVector<T>& B);

    /**
    columnVector add operator
    **/
    columnVector<T> operator+(const columnVector<T>& B) const
    {
        return columnVector<T>(*this).operator+=(B);
    }

    /**
    columnVector subtract operator
    **/
    columnVector<T> operator-(const columnVector<T>& B) const
    {
        return columnVector<T>(*this).operator-=(B);
    }

    /**
    assignment multiply operator
    **/
    columnVector<T>& operator*=(const double k);

    /**
    multiply operator
    **/
    columnVector<T> operator*(const double k) const
    {
        return columnVector<T>(*this).operator*=(k);
    }

    /**
    assignment divide operator
    **/
    columnVector<T>& operator/=(const T k);

    /**
    divide operator
    **/
    columnVector<T> operator/(const T k) const
    {
        return columnVector<T>(*this).operator/=(k);
    }

    /**
    Assignment operator
    **/
    columnVector<T>& operator=(const matrix<T>& B);

    /**
    columnVector dot operator for size(1,1) matrix
    **/
    columnVector<T> operator*(const matrix<T>& B) const;

    /**
    columnVector dot operator for size(1) columnVector
    **/
    columnVector<T> operator*(const columnVector<T>& B);

};


/**
Row Vector Type

@ingroup math
**/

template<class T> class rowVector : public matrix<T>
{

public:

    /**
    default constructor
    **/
    rowVector();

    /**
    copy constructor
    **/
    rowVector(const rowVector<T>& base);

    /**
    downcast constructor
    **/
    rowVector(const matrix<T>& base);

    /**
    constructor that accepts size (number of rows)
    **/
    explicit rowVector(int size);

    /**
    rowVector assignment add operator
    **/
    rowVector<T>& operator+=(const rowVector<T>& B);

    /**
    rowVector add operator
    **/
    rowVector<T> operator+(const rowVector<T>& B) const
    {
        return rowVector<T>(*this).operator+=(B);
    }

    /**
    rowVector assignment subtract operator
    **/
    rowVector<T>& operator-=(const rowVector<T>& B);

    /**
    columnVector subtract operator
    **/
    rowVector<T> operator-(const rowVector<T>& B) const
    {
        return rowVector<T>(*this).operator-=(B);
    }

    /**
    assignment multiply operator
    **/
    rowVector<T>& operator*=(const T k);

    /**
    multiply operator
    **/
    rowVector<T> operator*(const T k) const
    {
        return rowVector<T>(*this).operator*=(k);
    }

    /**
    assignment divide operator
    **/
    rowVector<T>& operator/=(const T k);

    /**
    divide operator
    **/
    rowVector<T> operator/(const T k) const
    {
        return rowVector<T>(*this).operator/=(k);
    }

    /**
    dot with columnVector (produces element)
    **/
    T operator*(const columnVector<T>& B) const;

    /**
    dot with matrix (produces column vector)
    **/
    columnVector<T> operator*(const matrix<T>& B) const;

    /**
    assignment operator
    **/
    rowVector<T>& operator=(const matrix<T>& B);

};


/**
Embodies algorithms to extract upper and lower matrices from a given matrix for LU factorization

@ingroup math
**/

template<class T> class LU
{

public:

    LU() { Q = P = matrix<T>(); determinant = 0; };

    /**
    extracts upper triangular matrix of given matrix Q

    @return upper triangular matrix of matrix Q
    **/
    matrix<T> U() const;

    /**
    extracts lower triangular matrix of given matrix Q

    @return lower triangular matrix of matrix Q
    **/
    matrix<T> L() const;

    /**
    computes the result of a linear system where this LU is the set of LU
    factors for system A matrix
    **/
    columnVector<T> Solve(const columnVector<T>& B) const;

    matrix<T> Q;             ///< matrix to extract LU triangular matrices from
    matrix<T> P;             ///< row permutation matrix
    T determinant;           ///< matrix determinant
};

template<class T> matrix<T>::matrix()
{
    numRows = numColumns = 0;
    elements.assign(0, 0.0);
    isColumnVector = false;
    isRowVector = false;
    dummy = 0;
}

template<class T> matrix<T>::matrix(const matrix<T>& base)
{
    numRows = base.GetNumRows();
    numColumns = base.GetNumColumns();
    elements = base.elements;
    isColumnVector = base.IsColumnVector();
    isRowVector = base.IsRowVector();
    dummy = 0;
}

template<class T> matrix<T>::matrix(const columnVector<T>& base)
{
    numRows = base.GetNumRows();
    numColumns = base.GetNumColumns();
    elements = base.elements;
    isColumnVector = false;
    isRowVector = false;
    dummy = 0;
}

template<class T> matrix<T>::matrix(const rowVector<T>& base)
{
    numRows = base.GetNumRows();
    numColumns = base.GetNumColumns();
    elements = base.elements;
    isColumnVector = false;
    isRowVector = false;
    dummy = 0;
}

template<class T> matrix<T>::matrix(const T& element)
{
    numRows = 1;
    numColumns = 1;
    elements.assign(0, 0.0);
    elements.assign(1, element);
    isColumnVector = false;
    isRowVector = false;
    dummy = 0;
}

template<class T> matrix<T>::matrix(const int rows, const int columns)
{
    numRows = rows;
    numColumns = columns;
    elements.assign(rows * columns, 0);
    isColumnVector = numRows > 1 && numColumns == 1;
    isRowVector = numColumns > 1 && numRows == 1;
    dummy = 0;
}

template<class T> matrix<T>::matrix(const int size)
{
    numRows = size;
    numColumns = size;
    elements.assign(size * size, 0);
    isColumnVector = false;
    isRowVector = false;
    dummy = 0;
}

template<class T> bool matrix<T>::IsColumnVector() const
{
    return isColumnVector;
}

template<class T> bool matrix<T>::IsRowVector() const
{
    return isRowVector;
}

template<class T> void matrix<T>::SetSize(const int rows, const int columns)
{
    numRows = rows;
    numColumns = columns;
    elements.assign(rows * numColumns, 0);
}

template<class T> void matrix<T>::Clear()
{
    elements.assign(numRows * numColumns, 0);
}

template<class T> int matrix<T>::GetNumRows() const
{
    return numRows;
}

template<class T> int matrix<T>::GetNumColumns() const
{
    return numColumns;
}

template<class T> int matrix<T>::GetNumElements() const
{
    return numRows * numColumns;
}

template<class T> T matrix<T>::GetElement(int row, int column) const
{
    if (row > 0 && row <= numRows && column > 0 && column <= numColumns)
    {
        return elements[numColumns * (row - 1) + (column - 1)];
    }
    else
    {
        throw std::runtime_error("Out of bounds in matrix method GetElement");

    }
}

template<class T> void matrix<T>::SetElement(int row, int column, T value)
{
    if (row > 0 && row <= numRows && column > 0 && column <= numColumns)
    {
        if (elements[numColumns * (row - 1) + (column - 1)] != value)
        {
            elements[numColumns * (row - 1) + (column - 1)] = value;
        }
    }
}

template<class T> void matrix<T>::SetAsEye()
{
    if (numRows != numColumns)
    {
        throw std::runtime_error("Invalid dimensions in matrix calling matrix.setAsIdentitymatrix() - matrix must be square.");
    }

    for (int i = 1; i <= numRows; i++)
    {
        for (int j = 1; j <= numColumns; j++)
        {
            if (i == j)
            {
                SetElement(i, j, 1);
            }
            else
            {
                SetElement(i, j, 0);
            }
        }
    }
}

template<class T> bool matrix<T>::operator==(const matrix<T>& B) const
{
    if (GetNumRows() != B.GetNumRows() || GetNumColumns() != B.GetNumColumns())
    {
        return false;
    }

    for (int i = 0; i < GetNumRows() * GetNumColumns(); i++)
    {
        if (elements[i] != B.elements[i])
        {
            return false;
        }
    }

    return true;
}

template<class T> matrix<T>& matrix<T>::operator+=(const matrix<T>& B)
{
    for (int i = 1; i <= GetNumRows(); i++)
    {
        for (int j = 1; j <= GetNumColumns(); j++)
        {
            SetElement(i, j, GetElement(i, j) + B.GetElement(i, j));
        }
    }
    return *this;
}

template<class T> matrix<T>& matrix<T>::operator-=(const matrix<T>& B)
{
    for (int i = 1; i <= GetNumRows(); i++)
    {
        for (int j = 1; j <= GetNumColumns(); j++)
        {
            SetElement(i, j, GetElement(i, j) - B.GetElement(i, j));
        }
    }
    return *this;
}

template<class T> matrix<T> matrix<T>::operator-() const
{
    for (int i = 1; i <= GetNumRows(); i++)
    {
        for (int j = 1; j <= GetNumColumns(); j++)
        {
            SetElement(i, j, -GetElement(i, j));
        }
    }
    return *this;
}

template<class T> matrix<T> matrix<T>::operator~() const
{
    matrix<T> Transposed(GetNumRows(), GetNumColumns());

    for (int i = 1; i <= GetNumRows(); i++)
    {
        for (int j = 1; j <= GetNumColumns(); j++)
        {
            Transposed(j, i) = GetElement(i, j);
        }
    }

    bool rvector = false;
    bool cvector = false;

    if (isRowVector)
    {
        rvector = false;
        cvector = true;
    }
    else if (isColumnVector)
    {
        rvector = true;
        cvector = false;
    }

    isRowVector = rvector;
    isColumnVector = cvector;

    return Transposed;
}

template<class T> matrix<T> matrix<T>::operator|(const matrix<T>& B) const
{
    if (GetNumRows() != B.GetNumRows())
    {
        std::cout << "Error: attempting to augment two matrices with different numbers of rows." << std::endl;
        return matrix<T>();
    }

    matrix<T> A(GetNumRows(), GetNumColumns() + B.GetNumColumns());

    for (int i = 1; i <= GetNumRows(); i++)
    {
        for (int j = 1; j <= GetNumColumns(); j++)
        {
            A.SetElement(i, j, GetElement(i, j));
        }
    }
    for (int i = 1; i <= B.numRows; i++)
    {
        for (int j = 1; j <= B.numColumns; j++)
        {
            A.SetElement(i, j + GetNumColumns(), B.GetElement(i, j));
        }
    }
    return A;
}

template<class T> matrix<T>& matrix<T>::operator*=(const T k)
{

    for (int i = 1; i <= GetNumRows(); i++)
    {
        for (int j = 1; j <= GetNumColumns(); j++)
        {
            SetElement(i, j, GetElement(i, j) * k);
        }
    }

    return *this;
}

template<class T> matrix<T>& matrix<T>::operator/=(const T k)
{
    for (int i = 1; i <= GetNumRows(); i++)
    {
        for (int j = 1; j <= GetNumColumns(); j++)
        {
            SetElement(i, j, GetElement(i, j) / k);
        }
    }

    return *this;
}

template<class T> matrix<T> matrix<T>::operator*(const matrix<T>& B) const
{
    matrix<T> C(GetNumRows(), B.GetNumColumns());

    for (int i = 1; i <= GetNumRows(); i++)
    {
        for (int j = 1; j <= GetNumColumns(); j++)
        {

            for (int k = 1; k <= B.GetNumRows(); k++)
            {
                C(i, j) += GetElement(i, k) * B(k, j);
            }
        }
    }

    return C;
}

template<class T> columnVector<T> matrix<T>::operator*(const columnVector<T>& B) const
{
    columnVector<T> C(B.GetNumRows());

    for (int i = 1; i <= GetNumRows(); i++)
    {
        for (int j = 1; j <= GetNumColumns(); j++)
        {
            C(i) += GetElement(i, j) * B(j);
        }
    }

    return C;
}

template<class T> void matrix<T>::InterchangeRows(int row1, int row2)
{
    T swap = 0;

    for (int i = 1; i <= GetNumColumns(); i++)
    {
        swap = GetElement(row1, i);
        SetElement(row1, i, GetElement(row2, i));
        SetElement(row2, i, swap);
    }
}

template<class T> void matrix<T>::SetRow(int row, T* values)
{
    for (int i = 1; i <= GetNumColumns(); i++)
    {
        SetElement(row, i, values[i - 1]);
    }
}

template<class T> void matrix<T>::ReplaceColumn(int column, const columnVector<T>& C)
{
    for (int i = 1; i <= numRows; i++)
    {
        SetElement(i, column, C.GetElement(i, 1));
    }
}

template<class T> void matrix<T>::ReplaceRow(int row, const rowVector<T>& R)
{
    if (R.numColumns != numColumns)
    {
        throw std::runtime_error("Cannot replace matrix row with row that has different number of columns.");
    }
    for (int i = 1; i <= numColumns; i++)
    {
        SetElement(row, i, R.GetElement(1, i));
    }
}

template<class T> matrix<T> matrix<T>::Partition(int afterWhichColumn)
{
    matrix<T> LHS(GetNumRows(), afterWhichColumn);

    matrix<T> RHS(GetNumRows(), GetNumColumns() - afterWhichColumn);

    for (int r = 1; r <= GetNumRows(); r++)
    {
        for (int c = 1; c <= GetNumColumns(); c++)
        {
            if (c <= afterWhichColumn)
            {
                LHS.SetElement(r, c, GetElement(r, c));
            }

            else
            {
                RHS.SetElement(r, (c - afterWhichColumn), GetElement(r, c));
            }
        }
    }

    elements = LHS.elements;
    numColumns = afterWhichColumn;

    return RHS;
}

template<class T> rowVector<T> matrix<T>::ExtractRow(int row) const
{
    rowVector<T> R(GetNumColumns());

    for (int j = 1; j <= GetNumColumns(); j++)
    {
        R(j) = GetElement(row, j);
    }
    return R;
}

template<class T> columnVector<T> matrix<T>::ExtractColumn(int column) const
{
    columnVector<T> C(GetNumRows());

    for (int i = 1; i <= GetNumRows(); i++)
    {
        C(i) = GetElement(i, column);
    }
    return C;
}

template<class T> matrix<T> matrix<T>::GetInverse(const matrix<T>& A, const LU<T>& LUFactor)
{
    matrix<T> identity(A.GetNumRows(), A.GetNumColumns());

    identity.SetAsEye();

    matrix<T> inverse(A.GetNumRows(), A.GetNumColumns());

    for (int col = 1; col <= A.GetNumColumns(); col++)
    {
        inverse.ReplaceColumn(col, matrix::Solve(A, identity.ExtractColumn(col), LUFactor));
    }
    return inverse;
}

template<class T> matrix<T> matrix<T>::GetInverse(const LU<T>& LUFactor) const
{
    matrix<T> identity(GetNumRows(), GetNumColumns());

    identity.SetAsEye();

    matrix<T> inverse(GetNumRows(), GetNumColumns());

    for (int col = 1; col <= GetNumColumns(); col++)
    {
        inverse.ReplaceColumn(col, Solve(identity.ExtractColumn(col), LUFactor));
    }
    return inverse;
}

template<class T> matrix<T> matrix<T>::GetInverse() const
{
    matrix<T> identity(GetNumRows(), GetNumColumns());

    identity.SetAsEye();

    matrix<T> inverse(GetNumRows(), GetNumColumns());
    
    LU<T> LUFactor = GetLU();

    for (int col = 1; col <= GetNumColumns(); col++)
    {
        inverse.ReplaceColumn(col, Solve(identity.ExtractColumn(col), LUFactor));
    }
    return inverse;
}

template<class T> LU<T> matrix<T>::GetLU() const
{
    LU<T> LUFactor;

    matrix<T> Eye(numRows, numColumns);

    Eye.SetAsEye();

    matrix<T> A = *this | Eye;

    T det = 1;

    for (int j = 1; j <= numRows; j++)
    {
        for (int k = j; k <= numRows; k++)
        {
            T prodsum = 0;
            for (int i = 1; i <= j - 1; i++)
            {
                prodsum += A(k, i) * A(i, j);
            }
            A(k, j) = A(k, j) - prodsum;
        }

        int rowPivot = j;

        T currMax = std::abs(A(j, j));

        for (int i = j; i <= numRows; i++)
        {
            if (std::abs(A(i, j)) > currMax)
            {
                currMax = std::abs(A(i, j));
                rowPivot = i;
            }
        }

        if (currMax == 0.0)
        {
            std::cout << "Singular matrix - cannot compute LU factorization." << std::endl;
        }

        if (rowPivot > j)
        {
            A.InterchangeRows(rowPivot, j);
            det *= -1.0;
        }

        for (int k = j + 1; k <= numRows; k++)
        {
            T prodsum = 0;
            for (int i = 1; i <= j - 1; i++)
            {
                prodsum += A(j, i) * A(i, k);
            }
            T newVal = (A(j, k) - prodsum) / A(j, j);
            A(j, k) = newVal;
        }
        det *= A(j, j);
    }

    LUFactor.P = A.Partition(numRows);
    LUFactor.Q = A;
    LUFactor.determinant = det;

    return LUFactor;
}

template<class T> matrix<T> matrix<T>::operator^(const T e) const
{
    if ((double)e == -1.0)
    {
        LU<T> Factors = GetLU();
        return GetInverse(Factors);
    }
    else
    {
        throw std::runtime_error("^ operator only defined for -1 exponent");
    }
}

template<class T> matrix<T> matrix<T>::Solve(const matrix<T>& A, const matrix<T>& B, const LU<T>& LUFactor) const
{
    columnVector<T> D = LUFactor.P * B;

    columnVector<T> Y(A.GetNumRows());

    columnVector<T> X(A.GetNumRows());

    for (int k = 1; k <= A.GetNumRows(); k++)
    {
        T prodsum = 0;

        for (int i = 1; i <= k - 1; i++)
        {
            prodsum += LUFactor.Q(k, i) * Y(i);
        }

        Y(k) = (D(k) - prodsum) / LUFactor.Q(k, k);
    }

    for (int k = A.GetNumRows(); k >= 1; k--)
    {
        T prodsum = 0;

        for (int i = k + 1; i <= A.GetNumRows(); i++)
        {
            prodsum += LUFactor.Q(k, i) * X(i);
        }

        X(k) = Y(k) - prodsum;
    }

    return X;
}

template<class T> columnVector<T> matrix<T>::Solve(const columnVector<T>& B, const LU<T>& LUFactor) const
{
    columnVector<T> D = LUFactor.P * B;

    columnVector<T> Y(GetNumRows());

    columnVector<T> X(GetNumRows());

    for (int k = 1; k <= GetNumRows(); k++)
    {
        T prodsum = 0;

        for (int i = 1; i <= k - 1; i++)
        {
            prodsum += LUFactor.Q(k, i) * Y(i);
        }

        Y(k) = (D(k) - prodsum) / LUFactor.Q(k, k);
    }

    for (int k = GetNumRows(); k >= 1; k--)
    {
        T prodsum = 0;

        for (int i = k + 1; i <= GetNumRows(); i++)
        {
            prodsum += LUFactor.Q(k, i) * X(i);
        }

        X(k) = Y(k) - prodsum;
    }

    return X;
}

template<class T> columnVector<T> matrix<T>::LeftDivide(const columnVector<T>& B) const
{
    LU<T> LUFactor = GetLU();

    columnVector<T> D = LUFactor.P * B;

    columnVector<T> Y(GetNumRows());
    
    columnVector<T> X(GetNumRows());

    for (int k = 1; k <= GetNumRows(); k++)
    {
        T prodsum = 0;

        for (int i = 1; i <= k - 1; i++)
        {
            prodsum += LUFactor.Q(k, i) * Y(i);
        }

        Y(k) = (D(k) - prodsum) / LUFactor.Q(k, k);
    }

    for (int k = GetNumRows(); k >= 1; k--)
    {
        T prodsum = 0;

        for (int i = k + 1; i <= GetNumRows(); i++)
        {
            prodsum += LUFactor.Q(k, i) * X(i);
        }

        X(k) = Y(k) - prodsum;
    }

    return X;
}

template<class T> T& matrix<T>::operator()(int row, int column)
{
    if (row > 0 && row <= numRows && column > 0 && column <= numColumns)
    {
        return elements[numColumns * (row - 1) + (column - 1)];
    }
    else
    {
        dummy = 0;
        return dummy;
    }
}

template<class T> const T& matrix<T>::operator()(int row, int column) const
{
    if (row > 0 && row <= numRows && column > 0 && column <= numColumns)
    {
        return elements[numColumns * (row - 1) + (column - 1)];
    }
    else
    {
        return dummy;
    }
}

template<class T> T& matrix<T>::operator()(int index)
{
    if (index == 0)
    {
        dummy = 0;
        return dummy;
    }
    else
    {
        return elements[index - 1];
    }
}

template<class T> const T& matrix<T>::operator()(int index) const
{
    if (index == 0)
    {
        return dummy;
    }
    else
    {
        return elements[index - 1];
    }
}

template<class T> void matrix<T>::Print() const
{
    for (int i = 1; i <= numRows; i++)
    {
        printf("[");
        for (int j = 1; j <= numColumns; j++)
        {
            printf("%10.4g ", GetElement(i, j));
        }
        printf("]\n");
    }
    printf("\n");
}

template<class T> matrix<T>& matrix<T>::operator=(const matrix<T>& B)
{
    elements = B.elements;
    numRows = B.numRows;
    numColumns = B.numColumns;
    isColumnVector = B.isColumnVector;
    isRowVector = B.isRowVector;
    return *this;
}

template<class T> columnVector<T>::columnVector()
{
    this->numRows = 0;
    this->numColumns = 0;
    this->elements.assign(0, 0);
    this->isColumnVector = true;
    this->isRowVector = false;
    this->dummy = 0;
}

template<class T> columnVector<T>::columnVector(const columnVector<T>& base)
{
    this->numRows = base.GetNumRows();
    this->numColumns = 1;
    this->elements = base.elements;
    this->isColumnVector = true;
    this->isRowVector = false;
    this->dummy = 0;
}

template<class T> columnVector<T>::columnVector(const matrix<T>& base)
{
    this->numRows = base.GetNumRows();
    this->numColumns = 1;
    this->elements.assign(this->numRows, 0);
    for (int i = 1; i <= this->numRows; i++)
    {
        this->SetElement(i, 1, this->GetElement(i, 1));
    }
    this->isColumnVector = true;
    this->isRowVector = false;
    this->dummy = 0;
}

template<class T> columnVector<T>::columnVector(int size)
{
    this->numRows = size;
    this->numColumns = 1;
    this->elements.assign(size, 0);
    this->isColumnVector = true;
    this->isRowVector = false;
    this->dummy = 0;
}

template<class T> columnVector<T>& columnVector<T>::operator+=(const columnVector<T>& B)
{

    for (int i = 1; i <= this->GetNumRows(); i++)
    {
        this->SetElement(i, 1, this->GetElement(i, 1) + B.GetElement(i, 1));
    }

    return *this;
}

template<class T> columnVector<T>& columnVector<T>::operator-=(const columnVector<T>& B)
{
    for (int i = 1; i <= this->GetNumRows(); i++)
    {
        this->SetElement(i, 1, this->GetElement(i, 1) - B.GetElement(i, 1));
    }

    return *this;
}

template<class T> columnVector<T>& columnVector<T>::operator*=(const double k)
{
    for (int i = 1; i <= this->GetNumRows(); i++)
    {
        this->SetElement(i, 1, this->GetElement(i, 1) * k);
    }

    return *this;
}

template<class T> columnVector<T>& columnVector<T>::operator/=(const T k)
{
    for (int i = 1; i <= this->GetNumRows(); i++)
    {
        this->SetElement(i, 1, this->GetElement(i, 1) / k);
    }

    return *this;
}

template<class T> columnVector<T>& columnVector<T>::operator=(const matrix<T>& B)
{
    columnVector<T> C(B.GetNumRows());

    for (int i = 1; i <= C.GetNumRows(); i++)
    {
        C.SetElement(i, 1, B.GetElement(i, 1));
    }

    return C;
}

template<class T> columnVector<T> columnVector<T>::operator*(const matrix<T>& B) const
{
    for (int i = 1; i <= this->GetNumRows(); i++)
    {
        this->SetElement(i, 1, this->GetElement(i, 1) * B.GetElement(1, 1));
    }

    return *this;
}

template<class T> columnVector<T> columnVector<T>::operator*(const columnVector<T>& B)
{
    for (int i = 1; i <= this->GetNumRows(); i++)
    {
        this->SetElement(i, 1, this->GetElement(i, 1) * B.GetElement(1, 1));
    }

    return *this;
}

template<class T> rowVector<T>::rowVector()
{
    this->numRows = 0;
    this->numColumns = 0;
    this->elements.assign(0, 0);
    this->isColumnVector = false;
    this->isRowVector = true;
    this->dummy = 0;
}

template<class T> rowVector<T>::rowVector(const rowVector<T>& base)
{
    this->numRows = 1;
    this->numColumns = base.GetNumColumns();
    this->elements = base.elements;
    this->isColumnVector = false;
    this->isRowVector = true;
    this->dummy = 0;
}

template<class T> rowVector<T>::rowVector(const matrix<T>& base)
{
    this->numRows = 1;
    this->numColumns = base.GetNumColumns();
    this->elements.assign(this->numColumns, 0);
    for (int i = 1; i <= this->numColumns; i++)
    {
        this->SetElement(1, i, this->GetElement(1, i));
    }
    this->isColumnVector = false;
    this->isRowVector = true;
    this->dummy = 0;
}

template<class T> rowVector<T>::rowVector(int size)
{
    this->numRows = 1;
    this->numColumns = size;
    this->elements.assign(size, 0);
    this->isColumnVector = false;
    this->isRowVector = true;
    this->dummy = 0;
}

template<class T> rowVector<T>& rowVector<T>::operator+=(const rowVector<T>& B)
{

    for (int i = 1; i <= this->GetNumColumns(); i++)
    {
        this->SetElement(1, i, this->GetElement(1, i) + B.GetElement(1, i));
    }

    return *this;
}

template<class T> rowVector<T>& rowVector<T>::operator-=(const rowVector<T>& B)
{
    for (int i = 1; i <= this->GetNumColumns(); i++)
    {
        this->SetElement(1, i, this->GetElement(1, i) - B.GetElement(1, i));
    }

    return *this;
}

template<class T> rowVector<T>& rowVector<T>::operator*=(const T k)
{
    for (int i = 1; i <= this->GetNumColumns(); i++)
    {
        this->SetElement(1, i, this->GetElement(1, i) * k);
    }

    return *this;
}

template<class T> rowVector<T>& rowVector<T>::operator/=(const T k)
{
    for (int i = 1; i <= this->GetNumColumns(); i++)
    {
        this->SetElement(1, i, this->GetElement(1, i) / k);
    }

    return *this;
}

template<class T> T rowVector<T>::operator*(const columnVector<T>& B) const
{
    T result = 0;

    for (int i = 1; i <= this->GetNumColumns(); i++)
    {
        result += B.GetElement(i, 1) * this->GetElement(1, i);
    }

    return result;
}

template<class T> columnVector<T> rowVector<T>::operator*(const matrix<T>& B) const
{

    columnVector<T> C(B.GetNumColumns());

    for (int i = 1; i <= this->GetNumColumns(); i++)
    {
        for (int j = 1; j <= B.GetNumRows(); j++)
        {
            C.SetElement(i, 1, B.GetElement(i, j) * this->GetElement(1, j));
        }
    }

    return C;
}

template<class T> rowVector<T>& rowVector<T>::operator=(const matrix<T>& B)
{
    rowVector<T> R(B.GetNumColumns());

    for (int i = 1; i <= R.GetNumColumns(); i++)
    {
        R.SetElement(1, i, B.GetElement(1, i));
    }

    return R;
}

template<class T> matrix<T> LU<T>::U() const
{
    matrix<T> Upper(Q.GetNumRows(), Q.GetNumColumns());
    Upper.SetAsEye();
    for (int row = 1; row <= Q.GetNumRows(); row++)
    {
        for (int column = row + 1; column <= Q.GetNumColumns(); column++)
        {
            Upper.SetElement(row, column, Q.GetElement(row, column));
        }
    }
    return Upper;
}

template<class T> matrix<T> LU<T>::L() const
{
    matrix<T> Lower(Q.GetNumRows(), Q.GetNumColumns());
    Lower.SetAsEye();
    for (int column = 1; column <= Q.GetNumColumns(); column++)
    {
        for (int row = column; row <= Q.GetNumRows(); row++)
        {
            Lower.SetElement(row, column, Q.GetElement(row, column));
        }
    }
    return Lower;
}

template<class T> columnVector<T> LU<T>::Solve(const columnVector<T>& B) const
{
    int numRows = B.GetNumRows();

    columnVector<T> D = matrix<T>::Dot(P, B);

    matrix<T> Y(numRows, 1);

    for (int k = 1; k <= numRows; k++)
    {
        T prodsum = 0;

        for (int i = 1; i <= k - 1; i++)
        {
            prodsum += Q.GetElement(k, i) * Y.GetElement(i, 1);
        }

        Y.SetElement(k, 1, (D.GetElement(k, 1) - prodsum) / Q.GetElement(k, k));
    }

    columnVector<T> X(numRows);

    for (int k = numRows; k >= 1; k--)
    {
        T prodsum = 0;

        for (int i = k + 1; i <= numRows; i++)
        {
            prodsum += Q.GetElement(k, i) * X.GetElement(i, 1);
        }

        X.SetElement(k, 1, Y.GetElement(k, 1) - prodsum);
    }
}

// Global functions:


template<class T> matrix<T> operator*(const T k, const matrix<T>& A)
{
    return A * k;
}

template<class T> columnVector<T> operator*(const T k, const columnVector<T>& A)
{
    return A * k;
}

#endif
