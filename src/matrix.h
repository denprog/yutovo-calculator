/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "real.h"
#include "rational.h"
#include "complex.h"
#include <algorithm>
#include <vector>

namespace yutovo_calculator
{

//A rectangular matrix over one of the number types; the elements are stored row-major.
template<class Number>
class Matrix
{
public:
    Matrix()
    {
    }

    Matrix(int precision)
    {
        rows = 1;
        columns = 1;
        numbers.emplace_back(precision);
    }

    Matrix(int precision, int num)
    {
        rows = 1;
        columns = 1;
        numbers.emplace_back(precision, num);
    }

    Matrix(const std::u32string& num)
    {
        rows = 1;
        columns = 1;
        numbers.emplace_back(num);
    }

    Matrix(const Number& num)
    {
        rows = 1;
        columns = 1;
        numbers.push_back(num);
    }

    Matrix(const Matrix& source) :
        numbers(source.numbers),
        rows(source.rows),
        columns(source.columns)
    {
    }

public:
    typedef Number value_type;
    typedef Matrix<Number> MatrixType;

    Matrix<Number>& operator=(const Matrix<Number>& num)
    {
        numbers = num.numbers;
        rows = num.rows;
        columns = num.columns;
        return *this;
    }

    Matrix<Number>& operator=(const Number& num)
    {
        numbers.clear();
        numbers.push_back(num);
        rows = 1;
        columns = 1;
        return *this;
    }

    Matrix<Number>& operator=(const int num)
    {
        numbers.clear();
        numbers.emplace_back(num);
        rows = 1;
        columns = 1;
        return *this;
    }

    Matrix<Number>& operator=(const std::u32string& num)
    {
        numbers.clear();
        numbers.emplace_back(num);
        rows = 1;
        columns = 1;
        return *this;
    }

    Matrix<Number> operator+() const
    {
        return *this;
    }

    Matrix<Number> operator-() const
    {
        Matrix<Number> res(*this);
        for (auto& element : res.numbers)
            element = -element;
        return res;
    }

    friend Matrix<Number> operator+(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        if (num1.rows == 1 && num1.columns == 1 && num1.numbers[0] == 0)
            return num2;
        if (num2.rows == 1 && num2.columns == 1 && num2.numbers[0] == 0)
            return num1;
        if (num1.rows != num2.rows || num1.columns != num2.columns)
            throw MathException(IncorrectOperation);
        Matrix<Number> res(num1);
        for (size_t i = 0; i < num1.numbers.size(); ++i)
            res.numbers[i] += num2.numbers[i];
        return res;
    }

    friend Matrix<Number> operator-(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        if (num2.rows == 1 && num2.columns == 1 && num2.numbers[0] == 0)
            return num1;
        if (num1.rows != num2.rows || num1.columns != num2.columns)
            throw MathException(IncorrectOperation);
        Matrix<Number> res(num1);
        for (size_t i = 0; i < num1.numbers.size(); ++i)
            res.numbers[i] -= num2.numbers[i];
        return res;
    }

    friend Matrix<Number> operator*(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        if (num1.rows == 1 && num1.columns == 1)
            return MatrixScalarProduct(num1.numbers[0], num2);
        if (num2.rows == 1 && num2.columns == 1)
            return MatrixScalarProduct(num2.numbers[0], num1);
        return MatrixProduct(num1, num2);
    }

    friend Matrix<Number> operator/(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        if (num2.rows == 1 && num2.columns == 1)
        {
            Matrix<Number> res(num1);
            for (auto& element : res.numbers)
                element = element / num2.numbers[0];
            return res;
        }
        //A/B is A multiplied by the inverse of B from the right
        return MatrixProduct(num1, inverse(num2));
    }

    void operator+=(const Matrix<Number>& num)
    {
        *this = *this + num;
    }

    void operator-=(const Matrix<Number>& num)
    {
        *this = *this - num;
    }

    void operator*=(const Matrix<Number>& num)
    {
        *this = *this * num;
    }

    void operator/=(const Matrix<Number>& num)
    {
        *this = *this / num;
    }

public:
    friend bool operator==(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        if (num1.rows != num2.rows || num1.columns != num2.columns)
            throw MathException(IncorrectOperation);
        for (size_t i = 0; i < num1.numbers.size(); ++i)
        {
            if (num1.numbers[i] != num2.numbers[i])
                return false;
        }
        return true;
    }

    friend bool operator==(const Matrix<Number>& num1, const int num2)
    {
        if (num1.rows != 1 || num1.columns != 1)
            throw MathException(IncorrectOperation);
        return num1.numbers[0] == num2;
    }

    friend bool operator!=(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        return !(operator==(num1, num2));
    }

    friend bool operator!=(const Matrix<Number>& num1, const int num2)
    {
        return !(operator==(num1, num2));
    }

    friend bool operator>(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        throw MathException(IncorrectComparison);
    }

    friend bool operator>=(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        throw MathException(IncorrectComparison);
    }

    friend bool operator<(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        throw MathException(IncorrectComparison);
    }

    friend bool operator<=(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        throw MathException(IncorrectComparison);
    }

public:
    static int MatrixElementToInt(const Number& value)
    {
        if constexpr (std::is_same_v<Number, Complex>)
            return (int)module(value);
        else
            return (int)value;
    }

private:
    static auto MatrixElementMagnitude(const Number& value)
    {
        if constexpr (std::is_same_v<Number, Complex>)
            return module(value);
        else
            return abs(value);
    }

    static bool MatrixIsVector(const Matrix<Number>& matrix)
    {
        return matrix.rows == 1 || matrix.columns == 1;
    }

    static Number MatrixElementFromInt(const Number& sample, int value)
    {
        Number res = sample;
        res = value;
        return res;
    }

    static Matrix<Number> MatrixFromScalar(const Number& value)
    {
        Matrix<Number> res;
        res.Resize(1, 1, value);
        return res;
    }

    friend Matrix<Number> MatrixScalarProduct(const Number& scalar, const Matrix<Number>& matrix)
    {
        Matrix<Number> res(matrix);
        for (int row = 0; row < res.Rows(); ++row)
        {
            for (int column = 0; column < res.Columns(); ++column)
                res.Element(row, column) = scalar * res.Get(row, column);
        }
        return res;
    }

    friend Matrix<Number> MatrixProduct(const Matrix<Number>& left, const Matrix<Number>& right)
    {
        if (left.Columns() != right.Rows())
            throw MathException(IncorrectOperation);
        Number sum = MatrixElementFromInt(left.Get(0, 0), 0);
        Matrix<Number> res;
        res.Resize(left.Rows(), right.Columns(), sum);
        for (int row = 0; row < left.Rows(); ++row)
        {
            for (int column = 0; column < right.Columns(); ++column)
            {
                Number value = sum;
                for (int inner = 0; inner < left.Columns(); ++inner)
                    value = value + left.Get(row, inner) * right.Get(inner, column);
                res.Element(row, column) = value;
            }
        }
        return res;
    }

public:
    friend Matrix<Number> det(const Matrix<Number>& matrix)
    {
        if (matrix.Rows() == 0 || matrix.Rows() != matrix.Columns())
            throw MathException(IncorrectOperation);
        int size = matrix.Rows();
        std::vector<Number> elements = matrix.Elements();
        int sign = 1;
        for (int column = 0; column < size; ++column)
        {
            //pick the row with the largest-magnitude element in the column as the pivot row
            int pivot_row = column;
            auto best_magnitude = MatrixElementMagnitude(elements[(size_t)column * size + column]);
            for (int row = column + 1; row < size; ++row)
            {
                auto magnitude = MatrixElementMagnitude(elements[(size_t)row * size + column]);
                if (magnitude > best_magnitude)
                {
                    best_magnitude = magnitude;
                    pivot_row = row;
                }
            }
            if (elements[(size_t)pivot_row * size + column] == 0)
            {
                //a zero column below the diagonal makes the determinant zero
                return MatrixFromScalar(MatrixElementFromInt(elements[0], 0));
            }
            if (pivot_row != column)
            {
                for (int j = 0; j < size; ++j)
                    std::swap(elements[(size_t)pivot_row * size + j], elements[(size_t)column * size + j]);
                sign = -sign;
            }
            for (int row = column + 1; row < size; ++row)
            {
                Number factor = elements[(size_t)row * size + column] / elements[(size_t)column * size + column];
                if (factor == 0)
                    continue;
                for (int j = column; j < size; ++j)
                    elements[(size_t)row * size + j] = elements[(size_t)row * size + j] - factor * elements[(size_t)column * size + j];
            }
        }
        Number result = MatrixElementFromInt(elements[0], 1);
        for (int i = 0; i < size; ++i)
            result = result * elements[(size_t)i * size + i];
        if (sign < 0)
            result = -result;
        return MatrixFromScalar(result);
    }

    friend Matrix<Number> inverse(const Matrix<Number>& matrix)
    {
        if (matrix.Rows() == 0 || matrix.Rows() != matrix.Columns())
            throw MathException(IncorrectOperation);
        int size = matrix.Rows();
        Number zero = MatrixElementFromInt(matrix.Get(0, 0), 0);
        Number one = MatrixElementFromInt(matrix.Get(0, 0), 1);
        std::vector<Number> elements = matrix.Elements();
        Matrix<Number> augmented;
        augmented.Resize(size, 2 * size, zero);
        for (int row = 0; row < size; ++row)
        {
            for (int column = 0; column < size; ++column)
                augmented.Element(row, column) = elements[(size_t)row * size + column];
            augmented.Element(row, size + row) = one;
        }
        for (int column = 0; column < size; ++column)
        {
            int pivot_row = column;
            auto best_magnitude = MatrixElementMagnitude(augmented.Get(column, column));
            for (int row = column + 1; row < size; ++row)
            {
                auto magnitude = MatrixElementMagnitude(augmented.Get(row, column));
                if (magnitude > best_magnitude)
                {
                    best_magnitude = magnitude;
                    pivot_row = row;
                }
            }
            if (augmented.Get(pivot_row, column) == 0)
                throw MathException(DivisionByZero);
            if (pivot_row != column)
            {
                for (int j = 0; j < 2 * size; ++j)
                    std::swap(augmented.Element(pivot_row, j), augmented.Element(column, j));
            }
            Number pivot = augmented.Get(column, column);
            for (int j = 0; j < 2 * size; ++j)
                augmented.Element(column, j) = augmented.Get(column, j) / pivot;
            for (int row = 0; row < size; ++row)
            {
                if (row == column)
                    continue;
                Number factor = augmented.Get(row, column);
                if (factor == 0)
                    continue;
                for (int j = 0; j < 2 * size; ++j)
                    augmented.Element(row, j) = augmented.Get(row, j) - factor * augmented.Get(column, j);
            }
        }
        Matrix<Number> res;
        res.Resize(size, size, zero);
        for (int row = 0; row < size; ++row)
        {
            for (int column = 0; column < size; ++column)
                res.Element(row, column) = augmented.Get(row, size + column);
        }
        return res;
    }

    friend Matrix<Number> transpose(const Matrix<Number>& matrix)
    {
        Number fill = MatrixElementFromInt(matrix.Get(0, 0), 0);
        Matrix<Number> res;
        res.Resize(matrix.Columns(), matrix.Rows(), fill);
        for (int row = 0; row < matrix.Rows(); ++row)
        {
            for (int column = 0; column < matrix.Columns(); ++column)
                res.Element(column, row) = matrix.Get(row, column);
        }
        return res;
    }

    friend Matrix<Number> trace(const Matrix<Number>& matrix)
    {
        if (matrix.Rows() == 0 || matrix.Rows() != matrix.Columns())
            throw MathException(IncorrectOperation);
        Number result = matrix.Get(0, 0);
        result = 0;
        for (int i = 0; i < matrix.Rows(); ++i)
            result = result + matrix.Get(i, i);
        return MatrixFromScalar(result);
    }

    friend Matrix<Number> rank(const Matrix<Number>& matrix)
    {
        if (matrix.Rows() == 0)
            throw MathException(IncorrectOperation);
        Matrix<Number> reduced = rref(matrix);
        int count = 0;
        for (int row = 0; row < reduced.Rows(); ++row)
        {
            bool zero_row = true;
            for (int column = 0; column < reduced.Columns(); ++column)
            {
                if (reduced.Get(row, column) != 0)
                {
                    zero_row = false;
                    break;
                }
            }
            if (!zero_row)
                ++count;
        }
        return MatrixFromScalar(MatrixElementFromInt(matrix.Get(0, 0), count));
    }

    friend Matrix<Number> rref(const Matrix<Number>& matrix)
    {
        if (matrix.Rows() == 0)
            throw MathException(IncorrectOperation);
        int rows = matrix.Rows();
        int columns = matrix.Columns();
        Number zero = MatrixElementFromInt(matrix.Get(0, 0), 0);
        Number one = MatrixElementFromInt(matrix.Get(0, 0), 1);
        Matrix<Number> res(matrix);
        int row = 0;
        for (int column = 0; column < columns && row < rows; ++column)
        {
            int pivot_row = row;
            auto best_magnitude = MatrixElementMagnitude(res.Get(row, column));
            for (int candidate = row + 1; candidate < rows; ++candidate)
            {
                auto magnitude = MatrixElementMagnitude(res.Get(candidate, column));
                if (magnitude > best_magnitude)
                {
                    best_magnitude = magnitude;
                    pivot_row = candidate;
                }
            }
            if (res.Get(pivot_row, column) == 0)
                continue;
            if (pivot_row != row)
            {
                for (int j = 0; j < columns; ++j)
                    std::swap(res.Element(pivot_row, j), res.Element(row, j));
            }
            Number pivot = res.Get(row, column);
            for (int j = 0; j < columns; ++j)
                res.Element(row, j) = res.Get(row, j) / pivot;
            for (int other = 0; other < rows; ++other)
            {
                if (other == row)
                    continue;
                Number factor = res.Get(other, column);
                if (factor == 0)
                    continue;
                for (int j = 0; j < columns; ++j)
                    res.Element(other, j) = res.Get(other, j) - factor * res.Get(row, j);
            }
            ++row;
        }
        return res;
    }

    friend Matrix<Number> idn(const Matrix<Number>& order)
    {
        const Number& value = order.GetScalar();
        Number zero = value;
        zero = 0;
        Number one = value;
        one = 1;
        Matrix<Number> res;
        int size = MatrixElementToInt(value);
        if (size <= 0)
            throw MathException(IncorrectOperation);
        res.Resize(size, size, zero);
        for (int i = 0; i < size; ++i)
            res.Element(i, i) = one;
        return res;
    }

    friend Matrix<Number> rows(const Matrix<Number>& matrix)
    {
        if (matrix.Rows() == 0)
            throw MathException(IncorrectOperation);
        return MatrixFromScalar(MatrixElementFromInt(matrix.Get(0, 0), matrix.Rows()));
    }

    friend Matrix<Number> cols(const Matrix<Number>& matrix)
    {
        if (matrix.Rows() == 0)
            throw MathException(IncorrectOperation);
        return MatrixFromScalar(MatrixElementFromInt(matrix.Get(0, 0), matrix.Columns()));
    }

    //The size as a 1x2 matrix [[rows,columns]]
    friend Matrix<Number> size(const Matrix<Number>& matrix)
    {
        if (matrix.Rows() == 0)
            throw MathException(IncorrectOperation);
        Matrix<Number> res;
        res.Resize(1, 2, matrix.Get(0, 0));
        res.Element(0, 0) = MatrixElementFromInt(matrix.Get(0, 0), matrix.Rows());
        res.Element(0, 1) = MatrixElementFromInt(matrix.Get(0, 0), matrix.Columns());
        return res;
    }

    //Solves the linear system A*x=b by Gaussian elimination with back substitution; b must be a column vector
    friend Matrix<Number> linsolve(const Matrix<Number>& left, const Matrix<Number>& right)
    {
        if (left.Rows() == 0 || left.Rows() != left.Columns() || right.Columns() != 1 || right.Rows() != left.Rows())
            throw MathException(IncorrectOperation);
        int size = left.Rows();
        Number zero = MatrixElementFromInt(left.Get(0, 0), 0);
        Matrix<Number> augmented;
        augmented.Resize(size, size + 1, zero);
        for (int row = 0; row < size; ++row)
        {
            for (int column = 0; column < size; ++column)
                augmented.Element(row, column) = left.Get(row, column);
            augmented.Element(row, size) = right.Get(row, 0);
        }
        for (int column = 0; column < size; ++column)
        {
            int pivot_row = column;
            auto best_magnitude = MatrixElementMagnitude(augmented.Get(column, column));
            for (int row = column + 1; row < size; ++row)
            {
                auto magnitude = MatrixElementMagnitude(augmented.Get(row, column));
                if (magnitude > best_magnitude)
                {
                    best_magnitude = magnitude;
                    pivot_row = row;
                }
            }
            if (augmented.Get(pivot_row, column) == 0)
                throw MathException(DivisionByZero);
            if (pivot_row != column)
            {
                for (int j = 0; j <= size; ++j)
                    std::swap(augmented.Element(pivot_row, j), augmented.Element(column, j));
            }
            for (int row = column + 1; row < size; ++row)
            {
                Number factor = augmented.Get(row, column) / augmented.Get(column, column);
                if (factor == 0)
                    continue;
                for (int j = column; j <= size; ++j)
                    augmented.Element(row, j) = augmented.Get(row, j) - factor * augmented.Get(column, j);
            }
        }
        Matrix<Number> solution;
        solution.Resize(size, 1, zero);
        for (int row = size - 1; row >= 0; --row)
        {
            Number value = augmented.Get(row, size);
            for (int column = row + 1; column < size; ++column)
                value = value - augmented.Get(row, column) * solution.Get(column, 0);
            solution.Element(row, 0) = value / augmented.Get(row, row);
        }
        return solution;
    }

    //The dot product of two vectors given as matrices with a single row or a single column
    friend Matrix<Number> dot(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        if (num1.rows == 0 || num2.rows == 0 || !MatrixIsVector(num1) || !MatrixIsVector(num2) ||
            num1.numbers.size() != num2.numbers.size())
        {
            throw MathException(IncorrectOperation);
        }
        Number sum = MatrixElementFromInt(num1.numbers[0], 0);
        for (size_t i = 0; i < num1.numbers.size(); ++i)
            sum = sum + num1.numbers[i] * num2.numbers[i];
        return MatrixFromScalar(sum);
    }

    //The cross product of two 3-dimensional vectors; the result is a column vector
    friend Matrix<Number> cross(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        if (!MatrixIsVector(num1) || !MatrixIsVector(num2) || num1.numbers.size() != 3 || num2.numbers.size() != 3)
            throw MathException(IncorrectOperation);
        Matrix<Number> res;
        res.Resize(3, 1, num1.numbers[0]);
        res.Element(0, 0) = num1.numbers[1] * num2.numbers[2] - num1.numbers[2] * num2.numbers[1];
        res.Element(1, 0) = num1.numbers[2] * num2.numbers[0] - num1.numbers[0] * num2.numbers[2];
        res.Element(2, 0) = num1.numbers[0] * num2.numbers[1] - num1.numbers[1] * num2.numbers[0];
        return res;
    }

    //The Euclidean norm (the length) of a vector; for rational elements the result must stay exact
    friend Matrix<Number> norm(const Matrix<Number>& matrix)
    {
        if (matrix.rows == 0 || !MatrixIsVector(matrix))
            throw MathException(IncorrectOperation);
        if constexpr (std::is_same_v<Number, Complex>)
        {
            //the sum of the squared modules is a real value, so the norm is real as well
            Real squares = module(matrix.numbers[0]) * module(matrix.numbers[0]);
            for (size_t i = 1; i < matrix.numbers.size(); ++i)
                squares = squares + module(matrix.numbers[i]) * module(matrix.numbers[i]);
            return MatrixFromScalar(Number(sqrt(squares)));
        }
        else
        {
            Number squares = matrix.numbers[0] * matrix.numbers[0];
            for (size_t i = 1; i < matrix.numbers.size(); ++i)
                squares = squares + matrix.numbers[i] * matrix.numbers[i];
            return MatrixFromScalar(sqrt(squares));
        }
    }

    //The vector of length 1 in the same direction, keeping the orientation of the argument
    friend Matrix<Number> normalize(const Matrix<Number>& matrix)
    {
        Matrix<Number> length = norm(matrix);
        if (length.numbers[0] == 0)
            throw MathException(DivisionByZero);
        Matrix<Number> res(matrix);
        for (auto& element : res.numbers)
            element = element / length.numbers[0];
        return res;
    }

    //The angle between two vectors; the result carries the radian measure
    friend Matrix<Number> angle(const Matrix<Number>& num1, const Matrix<Number>& num2)
    {
        if (num1.rows == 0 || !MatrixIsVector(num1) || !MatrixIsVector(num2) || num1.numbers.size() != num2.numbers.size())
            throw MathException(IncorrectOperation);
        Matrix<Number> length1 = norm(num1);
        Matrix<Number> length2 = norm(num2);
        if (length1.numbers[0] == 0 || length2.numbers[0] == 0)
            throw MathException(DivisionByZero);
        Number cosine = dot(num1, num2).numbers[0] / (length1.numbers[0] * length2.numbers[0]);
        if constexpr (std::is_same_v<Number, Complex>)
        {
            int result_position = 0;
            return MatrixFromScalar(arccos(cosine, result_position));
        }
        else
            return MatrixFromScalar(arccos(cosine));
    }

public:
    int Rows() const
    {
        return rows;
    }

    int Columns() const
    {
        return columns;
    }

    int Size() const
    {
        return (int)numbers.size();
    }

    bool IsSingleValue() const
    {
        return rows == 1 && columns == 1;
    }

    bool IsEmpty() const
    {
        return numbers.empty();
    }

    const Number& GetScalar() const
    {
        if (!IsSingleValue())
            throw MathException(IncorrectOperation);
        return numbers[0];
    }

    const Number& Get(int row, int column) const
    {
        if (row < 0 || row >= rows || column < 0 || column >= columns)
            throw SyntaxException(ArgumentIsOver);
        return numbers[(size_t)row * columns + column];
    }

    Number& Element(int row, int column)
    {
        if (row < 0 || row >= rows || column < 0 || column >= columns)
            throw SyntaxException(ArgumentIsOver);
        return numbers[(size_t)row * columns + column];
    }

    void Resize(int _rows, int _columns, const Number& fill_value)
    {
        if (_rows < 0 || _columns < 0)
            throw MathException(IncorrectOperation);
        rows = _rows;
        columns = _columns;
        numbers.assign((size_t)_rows * _columns, fill_value);
    }

    void AppendRows(const Matrix<Number>& source)
    {
        if (source.columns != columns)
            throw MathException(IncorrectOperation);
        numbers.insert(numbers.end(), source.numbers.begin(), source.numbers.end());
        rows += source.rows;
    }

    const std::vector<Number>& Elements() const
    {
        return numbers;
    }

    void SetPrecision(int precision)
    {
        for (auto& element : numbers)
            element.SetPrecision(precision);
    }

    int GetPrecision() const
    {
        if (numbers.empty())
            return 0;
        return numbers[0].GetPrecision();
    }

    Matrix<Number> ToRadian() const
    {
        Matrix<Number> res;
        res.Resize(rows, columns, Number());
        for (size_t i = 0; i < numbers.size(); ++i)
            res.numbers[i] = numbers[i].ToRadian();
        return res;
    }

    Matrix<Number> ToDegree() const
    {
        Matrix<Number> res;
        res.Resize(rows, columns, Number());
        for (size_t i = 0; i < numbers.size(); ++i)
            res.numbers[i] = numbers[i].ToDegree();
        return res;
    }

    Matrix<Number> ToGrad() const
    {
        Matrix<Number> res;
        res.Resize(rows, columns, Number());
        for (size_t i = 0; i < numbers.size(); ++i)
            res.numbers[i] = numbers[i].ToGrad();
        return res;
    }

    AngleMeasure GetAngleMeasure() const
    {
        if constexpr (std::is_same_v<Number, Rational>)
        {
            return AngleMeasure::None;
        }
        else
        {
            if (numbers.empty())
                return AngleMeasure::None;
            Number first = numbers[0];
            return first.GetAngleMeasure();
        }
    }

    void SetAngleMeasure(AngleMeasure angle_measure)
    {
        if constexpr (!std::is_same_v<Number, Rational>)
        {
            for (auto& element : numbers)
                element.SetAngleMeasure(angle_measure);
        }
    }

    bool IsNaN() const
    {
        if constexpr (std::is_same_v<Number, Real> || std::is_same_v<Number, Complex>)
        {
            for (auto& element : numbers)
            {
                if (element.IsNaN())
                    return true;
            }
        }
        return false;
    }

public:
    std::u32string ToString() const
    {
        std::u32string res = U"[";
        for (int row = 0; row < rows; ++row)
        {
            res += U"[";
            for (int column = 0; column < columns; ++column)
            {
                res += numbers[(size_t)row * columns + column].ToString();
                if (column < columns - 1)
                    res += U",";
            }
            res += U"]";
            if (row < rows - 1)
                res += U",";
        }
        res += U"]";
        return res;
    }

    std::string ToStdString(int exp, int accuracy) const
    {
        std::string res = "[";
        for (int row = 0; row < rows; ++row)
        {
            res += "[";
            for (int column = 0; column < columns; ++column)
            {
                const Number& element = numbers[(size_t)row * columns + column];
                if constexpr (std::is_same_v<Number, Rational>)
                    res += element.ToStdString();
                else
                    res += element.ToStdString(exp, accuracy);
                if (column < columns - 1)
                    res += ",";
            }
            res += "]";
            if (row < rows - 1)
                res += ",";
        }
        res += "]";
        return res;
    }

private:
    std::vector<Number> numbers;
    int rows = 0;
    int columns = 0;
};

template<typename Number>
struct is_matrix : std::false_type
{
};

template<typename Number>
struct is_matrix<Matrix<Number>> : std::true_type
{
};

template<typename Number>
inline constexpr bool is_matrix_v = is_matrix<Number>::value;

}

#endif
