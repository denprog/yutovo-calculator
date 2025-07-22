#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "real.h"
#include <algorithm>

namespace yutovo_calculator
{

template<class Number>
class Array
{
public:
    Array()
    {
    }

    Array(int precision)
    {
        numbers.emplace_back(precision);
    }

    Array(int precision, Unit& _unit)
    {
    }

    Array(int precision, AngleMeasure _angle_measure)
    {
    }

    Array(int precision, const char* num)
    {
    }

    Array(int precision, int num)
    {
    }

    Array(int precision, float num)
    {
    }

    Array(const std::u32string& num)
    {
    }

    Array(const Array& source)
    {
    }

    ~Array()
    {
    }

public:
    std::u32string ToString() const
    {
        std::u32string res;
        return res;
    }

    std::string ToStdString(int exp, int accuracy) const
    {
        std::string res;
        return res;
    }

private:
    std::vector<Number> numbers;
};

}

#endif