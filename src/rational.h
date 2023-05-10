#ifndef RATIONAL_H
#define RATIONAL_H

#include <string>
#include <mpir.h>
#include "math_helper.h"
#include "integer.h"

using namespace std;

namespace yutovo_calculator
{

//Rational number
class Rational
{
public:
	Rational();
	Rational(const Rational& source);
	Rational(const int num);
	Rational(const int precision, const double num);
	Rational(const int precision, const int num);
	Rational(const std::u32string& num);
	~Rational();
	
public:
	typedef Rational value_type;

	Rational& operator=(const Rational& source);
	Rational& operator=(const std::u32string& source);
	
	Rational operator+();
	Rational operator-();

	friend Rational operator+(const Rational& num1, const Rational& num2);
	friend Rational operator+(const Rational& num1, const int num2);
	friend Rational operator+(const int num1, const Rational& num2);

	friend Rational operator-(const Rational& num1, const Rational& num2);
	friend Rational operator-(const Rational& num1, const int num2);
	friend Rational operator-(const int num1, const Rational& num2);

	friend Rational operator*(const Rational& num1, const Rational& num2);
	friend Rational operator*(const Rational& num1, const int num2);
	friend Rational operator*(const int num1, const Rational& num2);

	friend Rational operator/(const Rational& num1, const Rational& num2);
	friend Rational operator/(const Rational& num1, const int num2);
	friend Rational operator/(const int num1, const Rational& num2);

public:
	void operator+=(const Rational& num);

public:
	void operator=(const int num);
	
	operator int() const;

public:
	friend bool operator==(const Rational& num1, const Rational& num2);
	friend bool operator==(const Rational& num1, const int num2);
	friend bool operator==(const int num1, const Rational& num2);

	friend bool operator!=(const Rational& num1, const Rational& num2);
	friend bool operator!=(const Rational& num1, const int num2);
	friend bool operator!=(const int num1, const Rational& num2);

	friend bool operator>(const Rational& num1, const Rational& num2);
	friend bool operator>(const Rational& num1, const int num2);
	friend bool operator>(const int num1, const Rational& num2);

	friend bool operator>=(const Rational& num1, const Rational& num2);
	friend bool operator>=(const Rational& num1, const int num2);
	friend bool operator>=(const int num1, const Rational& num2);

	friend bool operator<(const Rational& num1, const Rational& num2);
	friend bool operator<(const Rational& num1, const int num2);
	friend bool operator<(const int num1, const Rational& num2);

	friend bool operator<=(const Rational& num1, const Rational& num2);
	friend bool operator<=(const Rational& num1, const int num2);
	friend bool operator<=(const int num1, const Rational& num2);

public:
	Integer GetNumerator();
	Integer GetDenomerator();

	std::u32string ToString() const;

	int GetPrecision() const
	{
		return 0;
	}

	void SetPrecision(int precision)
	{
	}

	void SetBitPrecision(const int precision)
	{
	}

	std::string ToStdString() const;

	private:
	#ifdef TRACE_OUTPUT
		void UpdateNumberStr();
	#endif
		
	private:
		mpq_t number;

	#ifdef TRACE_OUTPUT
		std::u32string number_str; //the std::u32string representation of the number for debug purposes
	#endif
};

}

#endif
