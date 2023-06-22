#include "unit.h"
#include "utils.h"
#include "real.h"
#include "parser_exception.h"

namespace yutovo_calculator
{

//Unit

Unit::Unit(const std::u32string& name)
{
    unit.push_back(std::make_pair(name, 1));
}

Unit::Unit(const std::u32string& name, const int power)
{
    unit.push_back(std::make_pair(name, power));
}

Unit operator+(const Unit& unit1, const Unit& unit2)
{
    if (unit1 != unit2)
        throw MathException(UnitsAreIncompatible);
    return unit1;
}

Unit operator+(const Unit& unit1, const int)
{
    if (!unit1.IsEmpty())
        throw MathException(UnitsAreIncompatible);
    return Unit();
}

Unit operator+(const int, const Unit& unit2)
{
    if (!unit2.IsEmpty())
        throw MathException(UnitsAreIncompatible);
    return Unit();
}

Unit operator+(const Unit& unit1, const float)
{
    if (!unit1.IsEmpty())
        throw MathException(UnitsAreIncompatible);
    return Unit();
}

Unit operator+(const float, const Unit& unit2)
{
    if (!unit2.IsEmpty())
        throw MathException(UnitsAreIncompatible);
    return Unit();
}

Unit operator-(const Unit& unit1, const Unit& unit2)
{
    if (unit1 != unit2)
        throw MathException(UnitsAreIncompatible);
    return unit1;
}

Unit operator-(const Unit& unit1, const int)
{
    if (!unit1.IsEmpty())
        throw MathException(UnitsAreIncompatible);
    return Unit();
}

Unit operator-(const int, const Unit& unit2)
{
    if (!unit2.IsEmpty())
        throw MathException(UnitsAreIncompatible);
    return Unit();
}

Unit operator-(const Unit& unit1, const float)
{
    if (!unit1.IsEmpty())
        throw MathException(UnitsAreIncompatible);
    return Unit();
}

Unit operator-(const float, const Unit& unit2)
{
    if (!unit2.IsEmpty())
        throw MathException(UnitsAreIncompatible);
    return Unit();
}

Unit operator*(const Unit& unit1, const Unit& unit2)
{
    Unit res(unit1);
    for (size_t i = 0, j = 0; i < unit2.unit.size(); ++i)
    {
        auto& p1 = unit2.unit[i];
        for (; j < res.unit.size(); ++j)
        {
            auto& p2 = res.unit[j];
            if (p1.first == p2.first)
            {
                p2.second += p1.second;
                if (p2.second == 0)
                    res.unit.erase(res.unit.begin() + j--);
                break;
            }
        }
        if (j == res.unit.size())
            res.unit.push_back(p1);
    }
    return res;
}

Unit operator*(const Unit& unit1, const int)
{
    return unit1;
}

Unit operator*(const int, const Unit& unit2)
{
    return unit2;
}

Unit operator*(const Unit& unit1, const float)
{
    return unit1;
}

Unit operator*(const float, const Unit& unit2)
{
    return unit2;
}

Unit operator/(const Unit& unit1, const Unit& unit2)
{
    Unit res(unit1);
    for (size_t i = 0, j = 0; i < unit2.unit.size(); ++i)
    {
        auto& p1 = unit2.unit[i];
        for (; j < res.unit.size(); ++j)
        {
            auto& p2 = res.unit[j];
            if (p1.first == p2.first)
            {
                p2.second -= p1.second;
                if (p2.second == 0)
                    res.unit.erase(res.unit.begin() + j--);
                break;
            }
        }
        if (j == res.unit.size())
            res.unit.push_back({p1.first, -p1.second});
    }
    return res;
}

Unit operator/(const int, const Unit& unit2)
{
    Unit res(unit2);
    for (auto& u : res.unit)
    {
        u.second *= -1;
    }
    return res;
}

Unit operator/(const Unit& unit1, const int)
{
    return unit1;
}

Unit operator/(const float, const Unit& unit2)
{
    Unit res(unit2);
    for (auto& u : res.unit)
    {
        u.second *= -1;
    }
    return res;
}

Unit operator/(const Unit& unit1, const float)
{
    return unit1;
}

Unit pow(const Unit& unit1, const Real& val)
{
    if (!val.unit.IsEmpty() || !val.IsInteger())
        throw MathException(UnitsAreIncompatible);
    int p = (int)val;
    Unit res(unit1);
    for (auto& u : res.unit)
    {
        u.second *= p;
    }
    return res;
}

Unit pow(const Unit& unit1, const int val)
{
    Unit res(unit1);
    for (auto& u : res.unit)
    {
        u.second *= val;
    }
    return res;
}

Unit pow(const Unit& unit1, const float)
{
    throw MathException(UnitsAreIncompatible);
}

bool Unit::operator==(const std::u32string& name) const
{
    return unit.size() == 1 && unit[0].first == name && unit[0].second == 1;
}

bool Unit::operator==(const Unit& other) const
{
    return unit == other.unit;
}

bool Unit::operator!=(const Unit& other) const
{
    return unit != other.unit;
}

int Unit::GetPower() const
{
    if (unit.empty())
        return 0;
    
    int p = unit[0].second;
    int j, k, l = 0;
	for (int i = 0; i < (int)unit.size() - 1; ++i)
	{
		j = abs(p);
		k = abs(unit[i + 1].second);

		while (k > 0)
		{
			l = j % k;
			j = k;
			k = l;
		}

		p = abs(j);
	}

	return p;
}

bool Unit::IsEmpty() const
{
    return unit.empty();
}

std::u32string Unit::ToString(const std::u32string& system) const
{
    if (unit.empty())
        return U"";
    
    std::u32string upper, lower;
    for (auto p : unit)
    {
        auto s = p.first;
        if (abs(p.second) > 1)
            s += U"^" + ToUtfString(std::to_string(abs(p.second)));
        if (p.second > 0)
        {
            if (upper.empty())
                upper = s;
            else
                upper += U"*" + s;
        }
        else
        {
            if (lower.empty())
                lower = s;
            else
                lower += U"*" + s;
        }
    }

    if (upper.empty())
        upper = U"1";
    else if (!lower.empty())
        upper = U"(" + upper + U")";
    if (!lower.empty())
    {
        upper += U"/";
        lower = U"(" + lower + U")";
    }

    auto r = U"(" + upper + lower + U")";
    if (system.empty() || system == U"SI")
        return r;
    return r + U"{" + system + U"}";
}

}
