#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <string>
#include <map>
#include <algorithm>
#include <mpfr.h>

using namespace std;

#define DEFAULT_BASE 10
#define DEFAULT_INCREASE_PRECISION 5
#define DEFAULT_RND MPFR_RNDN
#define DEFAULT_INCREASE_PRECISION 5

#ifdef DEBUG
#define TRACE_OUTPUT
#endif

namespace yutovo_calculator
{

enum class AngleMeasure
{
	None = -1, 
	Radian = 0, 
	Degree, 
	Grad
};

enum class Notation
{
	None = -1,
	Binary = 0,
	Octal,
	Decimal,
	Hexadecimal
};

enum class FractionForm
{
	None = -1,
    Proper = 0,
    Improper
};

enum class Language
{
	English = 1,
	Russian
};

class Real;

class MathHelper
{
public:
	static int ToBitPrecision(const int precision);
	static int ToDecimalPrecision(const int precision);

	template<class TNumber>
	static TNumber GetMisc(const TNumber& num);
	
private:
	static map<int, int> bit_precisions; //the bit precisions cache
	static map<int, int> decimal_precisions; //the decimal precisions cache
	static map<int, Real> real_miscs;	//the real miscs cache
};

}

#endif
