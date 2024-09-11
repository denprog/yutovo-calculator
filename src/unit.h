#ifndef __MEASURE_H__
#define __MEASURE_H__

#include <vector>
#include <string>
#include <algorithm>

namespace yutovo_calculator
{

class Real;

class Unit
{
public:
    Unit() = default;

    Unit(const std::u32string& name)
    {
        unit.push_back(std::make_pair(name, 1));
    }

    Unit(const std::u32string& name, const int power)
    {
        unit.push_back(std::make_pair(name, power));
    }

    Unit(const std::u32string& name, const std::u32string& _description, const int power) :
        description(_description)
    {
        unit.push_back(std::make_pair(name, power));
    }

    Unit(const std::u32string& numerator, const std::u32string& denomerator)
    {
        unit.push_back(std::make_pair(numerator, 1));
        unit.push_back(std::make_pair(denomerator, -1));
    }

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

    friend Unit sqrt(const Unit& unit);
    friend Unit root(const Unit& unit, const Real& val);

    bool operator==(const std::u32string& name) const;

    bool operator==(const Unit& other) const
    {
        std::u32string s1 = system, s2 = other.system;
        if (s1 == U"")
            s1 = U"SI";
        if (s2 == U"")
            s2 = U"SI";
        if (s1 != s2)
            return false;
        if (unit.size() != other.unit.size())
            return false;
        for (auto& u : other.unit)
        {
            auto it = std::find_if(unit.begin(), unit.end(), 
                [u](auto& p)
                {
                    return p.first == u.first && p.second == u.second;
                });
            if (it == unit.end())
                return false;
        }
        return true;
    }

    bool operator!=(const Unit& other) const
    {
        return !operator==(other);
    }

    int GetPower() const;

    bool IsEmpty() const
    {
        return unit.empty();
    }

    std::u32string ToString(bool f = true) const;

public:
    std::vector<std::pair<std::u32string, int>> unit; //unit names with their powers
    std::u32string system;
    std::u32string description;
};

}

#endif
