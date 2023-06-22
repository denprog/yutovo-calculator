#ifndef __MEASURE_H__
#define __MEASURE_H__

#include <vector>
#include <string>

namespace yutovo_calculator
{

class Real;

class Unit
{
public:
    Unit() = default;
    Unit(const std::u32string& name);
    Unit(const std::u32string& name, const int power);

    friend Unit operator+(const Unit& unit1, const Unit& unit2);
    friend Unit operator+(const Unit& unit1, const int);
    friend Unit operator+(const int, const Unit& unit2);
    friend Unit operator+(const Unit& unit1, const float);
    friend Unit operator+(const float, const Unit& unit2);

    friend Unit operator-(const Unit& unit1, const Unit& unit2);
    friend Unit operator-(const Unit& unit1, const int);
    friend Unit operator-(const int, const Unit& unit2);
    friend Unit operator-(const Unit& unit1, const float);
    friend Unit operator-(const float, const Unit& unit2);

    friend Unit operator*(const Unit& unit1, const Unit& unit2);
    friend Unit operator*(const Unit& unit1, const int);
    friend Unit operator*(const int, const Unit& unit2);
    friend Unit operator*(const Unit& unit1, const float);
    friend Unit operator*(const float, const Unit& unit2);

    friend Unit operator/(const Unit& unit1, const Unit& unit2);
    friend Unit operator/(const int, const Unit& unit2);
    friend Unit operator/(const Unit& unit1, const int);
    friend Unit operator/(const float, const Unit& unit2);
    friend Unit operator/(const Unit& unit1, const float);

    friend Unit pow(const Unit& unit1, const Real& val);
    friend Unit pow(const Unit& unit1, const int val);
    friend Unit pow(const Unit& unit1, const float);

    bool operator==(const std::u32string& name) const;
    bool operator==(const Unit& other) const;
    bool operator!=(const Unit& other) const;

    int GetPower() const;

    bool IsEmpty() const;

    std::u32string ToString(const std::u32string& system) const;

public:
    std::vector<std::pair<std::u32string, int>> unit; //unit names with their powers
};

}

#endif
