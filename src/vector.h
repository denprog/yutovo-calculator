/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "real.h"
#include "rational.h"
#include "complex.h"
#include <algorithm>
#include <vector>

namespace yutovo_calculator
{

//A column vector over one of the number types
template<class Number>
class Vector
{
public:
    Vector()
    {
    }

    Vector(int precision)
    {
        numbers.emplace_back(precision);
    }

    Vector(int precision, int num)
    {
        numbers.emplace_back(precision, num);
    }

    Vector(const std::u32string& num)
    {
        numbers.emplace_back(num);
    }

    Vector(const Number& num)
    {
        numbers.push_back(num);
    }

    Vector(const Vector& source) :
        numbers(source.numbers)
    {
    }

public:
    typedef Number value_type;
    typedef Vector<Number> VectorType;

    Vector<Number>& operator=(const Vector<Number>& num)
    {
        numbers = num.numbers;
        return *this;
    }

    Vector<Number>& operator=(const Number& num)
    {
        numbers.clear();
        numbers.push_back(num);
        return *this;
    }

    Vector<Number>& operator=(const int num)
    {
        numbers.clear();
        numbers.emplace_back(num);
        return *this;
    }

    Vector<Number>& operator=(const std::u32string& num)
    {
        numbers.clear();
        numbers.emplace_back(num);
        return *this;
    }

    Vector<Number> operator+() const
    {
        return *this;
    }

    Vector<Number> operator-() const
    {
        Vector<Number> res(*this);
        for (auto& element : res.numbers)
            element = -element;
        return res;
    }

    friend Vector<Number> operator+(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        if (num1.numbers.size() == 1 && num1.numbers[0] == 0)
            return num2;
        if (num2.numbers.size() == 1 && num2.numbers[0] == 0)
            return num1;
        if (num1.numbers.size() != num2.numbers.size())
            throw MathException(IncorrectOperation);
        Vector<Number> res(num1);
        for (size_t i = 0; i < num1.numbers.size(); ++i)
            res.numbers[i] += num2.numbers[i];
        return res;
    }

    friend Vector<Number> operator-(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        if (num2.numbers.size() == 1 && num2.numbers[0] == 0)
            return num1;
        if (num1.numbers.size() != num2.numbers.size())
            throw MathException(IncorrectOperation);
        Vector<Number> res(num1);
        for (size_t i = 0; i < num1.numbers.size(); ++i)
            res.numbers[i] -= num2.numbers[i];
        return res;
    }

    friend Vector<Number> operator*(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        if (num1.numbers.size() == 1)
            return VectorScalarProduct(num1.numbers[0], num2);
        if (num2.numbers.size() == 1)
            return VectorScalarProduct(num2.numbers[0], num1);
        return dot(num1, num2);
    }

    friend Vector<Number> operator/(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        if (num2.numbers.size() != 1)
            throw MathException(IncorrectOperation);
        Vector<Number> res(num1);
        for (auto& element : res.numbers)
            element = element / num2.numbers[0];
        return res;
    }

    void operator+=(const Vector<Number>& num)
    {
        *this = *this + num;
    }

    void operator-=(const Vector<Number>& num)
    {
        *this = *this - num;
    }

    void operator*=(const Vector<Number>& num)
    {
        *this = *this * num;
    }

    void operator/=(const Vector<Number>& num)
    {
        *this = *this / num;
    }

public:
    friend bool operator==(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        if (num1.numbers.size() != num2.numbers.size())
            throw MathException(IncorrectOperation);
        for (size_t i = 0; i < num1.numbers.size(); ++i)
        {
            if (num1.numbers[i] != num2.numbers[i])
                return false;
        }
        return true;
    }

    friend bool operator==(const Vector<Number>& num1, const int num2)
    {
        if (num1.numbers.size() != 1)
            throw MathException(IncorrectOperation);
        return num1.numbers[0] == num2;
    }

    friend bool operator!=(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        return !(operator==(num1, num2));
    }

    friend bool operator!=(const Vector<Number>& num1, const int num2)
    {
        return !(operator==(num1, num2));
    }

    friend bool operator>(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        throw MathException(IncorrectComparison);
    }

    friend bool operator>=(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        throw MathException(IncorrectComparison);
    }

    friend bool operator<(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        throw MathException(IncorrectComparison);
    }

    friend bool operator<=(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        throw MathException(IncorrectComparison);
    }

public:
    static int VectorElementToInt(const Number& value)
    {
        if constexpr (std::is_same_v<Number, Complex>)
            return (int)module(value);
        else
            return (int)value;
    }

private:
    static auto VectorElementMagnitude(const Number& value)
    {
        if constexpr (std::is_same_v<Number, Complex>)
            return module(value);
        else
            return abs(value);
    }

    static Number VectorElementFromInt(const Number& sample, int value)
    {
        Number res = sample;
        res = value;
        return res;
    }

    static Vector<Number> VectorFromScalar(const Number& value)
    {
        Vector<Number> res;
        res.numbers.push_back(value);
        return res;
    }

    static Number VectorSumOfSquaredMagnitudes(const Vector<Number>& num)
    {
        Number sum = num.numbers[0] * num.numbers[0];
        for (size_t i = 1; i < num.numbers.size(); ++i)
            sum = sum + num.numbers[i] * num.numbers[i];
        return sum;
    }

    friend Vector<Number> VectorScalarProduct(const Number& scalar, const Vector<Number>& vector)
    {
        Vector<Number> res(vector);
        for (auto& element : res.numbers)
            element = scalar * element;
        return res;
    }

public:
    friend Vector<Number> dot(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        if (num1.numbers.empty() || num1.numbers.size() != num2.numbers.size())
            throw MathException(IncorrectOperation);
        Number sum = VectorElementFromInt(num1.numbers[0], 0);
        for (size_t i = 0; i < num1.numbers.size(); ++i)
            sum = sum + num1.numbers[i] * num2.numbers[i];
        return VectorFromScalar(sum);
    }

    //The cross product of two 3-dimensional vectors
    friend Vector<Number> cross(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        if (num1.numbers.size() != 3 || num2.numbers.size() != 3)
            throw MathException(IncorrectOperation);
        Vector<Number> res;
        res.Add(num1.numbers[1] * num2.numbers[2] - num1.numbers[2] * num2.numbers[1]);
        res.Add(num1.numbers[2] * num2.numbers[0] - num1.numbers[0] * num2.numbers[2]);
        res.Add(num1.numbers[0] * num2.numbers[1] - num1.numbers[1] * num2.numbers[0]);
        return res;
    }

    //The Euclidean norm (the length) of a vector; for rational elements the result must stay exact
    friend Vector<Number> norm(const Vector<Number>& num)
    {
        if (num.numbers.empty())
            throw MathException(IncorrectOperation);
        if constexpr (std::is_same_v<Number, Complex>)
        {
            //the sum of the squared modules is a real value, so the norm is real as well
            Real squares = module(num.numbers[0]) * module(num.numbers[0]);
            for (size_t i = 1; i < num.numbers.size(); ++i)
                squares = squares + module(num.numbers[i]) * module(num.numbers[i]);
            return VectorFromScalar(Number(sqrt(squares)));
        }
        else
        {
            Number squares = VectorSumOfSquaredMagnitudes(num);
            return VectorFromScalar(sqrt(squares));
        }
    }

    //The vector of length 1 in the same direction; the zero vector has no direction
    friend Vector<Number> normalize(const Vector<Number>& num)
    {
        Vector<Number> length = norm(num);
        if (length.numbers[0] == 0)
            throw MathException(DivisionByZero);
        Vector<Number> res(num);
        for (auto& element : res.numbers)
            element = element / length.numbers[0];
        return res;
    }

    friend Vector<Number> angle(const Vector<Number>& num1, const Vector<Number>& num2)
    {
        if (num1.numbers.empty() || num1.numbers.size() != num2.numbers.size())
            throw MathException(IncorrectOperation);
        Vector<Number> length1 = norm(num1);
        Vector<Number> length2 = norm(num2);
        if (length1.numbers[0] == 0 || length2.numbers[0] == 0)
            throw MathException(DivisionByZero);
        Number cosine = dot(num1, num2).numbers[0] / (length1.numbers[0] * length2.numbers[0]);
        if constexpr (std::is_same_v<Number, Complex>)
        {
            int result_position = 0;
            return VectorFromScalar(arccos(cosine, result_position));
        }
        else
            return VectorFromScalar(arccos(cosine));
    }

    friend Vector<Number> abs(const Vector<Number>& num)
    {
        return norm(num);
    }

    friend Vector<Number> size(const Vector<Number>& num)
    {
        if (num.numbers.empty())
            throw MathException(IncorrectOperation);
        return VectorFromScalar(VectorElementFromInt(num.numbers[0], (int)num.numbers.size()));
    }

public:
    int Size() const
    {
        return (int)numbers.size();
    }

    bool IsSingleValue() const
    {
        return numbers.size() == 1;
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

    const Number& Get(int index) const
    {
        if (index < 0 || index >= (int)numbers.size())
            throw SyntaxException(ArgumentIsOver);
        return numbers[(size_t)index];
    }

    Number& Element(int index)
    {
        if (index < 0 || index >= (int)numbers.size())
            throw SyntaxException(ArgumentIsOver);
        return numbers[(size_t)index];
    }

    void Add(const Number& value)
    {
        numbers.push_back(value);
    }

    void Append(const Vector<Number>& value)
    {
        if (value.numbers.empty())
            throw MathException(IncorrectOperation);
        if (value.numbers.size() == 1)
            numbers.push_back(value.numbers[0]);
        else
            numbers.insert(numbers.end(), value.numbers.begin(), value.numbers.end());
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

    Vector<Number> ToRadian() const
    {
        Vector<Number> res;
        res.numbers.resize(numbers.size());
        for (size_t i = 0; i < numbers.size(); ++i)
            res.numbers[i] = numbers[i].ToRadian();
        return res;
    }

    Vector<Number> ToDegree() const
    {
        Vector<Number> res;
        res.numbers.resize(numbers.size());
        for (size_t i = 0; i < numbers.size(); ++i)
            res.numbers[i] = numbers[i].ToDegree();
        return res;
    }

    Vector<Number> ToGrad() const
    {
        Vector<Number> res;
        res.numbers.resize(numbers.size());
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
        for (size_t i = 0; i < numbers.size(); ++i)
        {
            res += U"[";
            res += numbers[i].ToString();
            res += U"]";
            if (i < numbers.size() - 1)
                res += U",";
        }
        res += U"]";
        return res;
    }

    std::string ToStdString(int exp, int accuracy) const
    {
        std::string res = "[";
        for (size_t i = 0; i < numbers.size(); ++i)
        {
            res += "[";
            const Number& element = numbers[i];
            if constexpr (std::is_same_v<Number, Rational>)
                res += element.ToStdString();
            else
                res += element.ToStdString(exp, accuracy);
            res += "]";
            if (i < numbers.size() - 1)
                res += ",";
        }
        res += "]";
        return res;
    }

private:
    std::vector<Number> numbers;
};

template<typename Number>
struct is_vector : std::false_type
{
};

template<typename Number>
struct is_vector<Vector<Number>> : std::true_type
{
};

template<typename Number>
inline constexpr bool is_vector_v = is_vector<Number>::value;

}

#endif
