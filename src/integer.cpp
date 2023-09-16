#include "stdafx.h"
#include "integer.h"
#include "utils.h"

namespace yutovo_calculator
{

//Integer

Integer::Integer()
{
	mpz_init(number);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Integer::Integer(const int precision, const int num)
{
	mpz_init(number);
	mpz_set_si(number, num);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Integer::Integer(const int precision, const double num)
{
	mpz_init(number);
	mpz_set_d(number, num);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Integer::Integer(const int num)
{
	mpz_init(number);
	mpz_set_si(number, num);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Integer::Integer(const Integer& source)
{
	mpz_init(number);
	mpz_set(number, source.number);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Integer::Integer(const std::u32string& num, const int base)
{
	mpz_init(number);
	if (mpz_set_str(number, (ToBasicString(num)).c_str(), base) == -1)
		throw MathException(ArgumentIsOver);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Integer::~Integer()
{
	mpz_clear(number);
}

Integer& Integer::operator=(const Integer& source)
{
	if (this == &source)
		return *this;

	mpz_clear(number);
	mpz_init(number);

	mpz_set(number, source.number);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif

	return *this;
}

Integer& Integer::operator=(const std::u32string& num)
{
	mpz_init(number);
	mpz_set_str(number, (ToBasicString(num)).c_str(), 10);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif

	return *this;
}

Integer& Integer::operator=(const mpz_t& source)
{
	mpz_set(number, source);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif

	return *this;
}

Integer Integer::operator+()
{
	Integer res(*this);

	return res;
}

Integer Integer::operator-()
{
	Integer res(*this);

	mpz_neg(res.number, number);

	return res;
}

Integer operator+(const Integer& num1, const Integer& num2)
{
	Integer res;

	mpz_add(res.number, num1.number, num2.number);

	return res;
}

Integer operator+(const Integer& num1, const int num2)
{
	Integer res;
	Integer _num2(num2);

	mpz_add(res.number, num1.number, _num2.number);

	return res;
}

Integer operator+(const int num1, const Integer& num2)
{
	Integer res;
	Integer _num1(num1);

	mpz_add(res.number, _num1.number, num2.number);

	return res;
}

Integer operator-(const Integer& num1, const Integer& num2)
{
	Integer res;

	mpz_sub(res.number, num1.number, num2.number);

	return res;
}

Integer operator-(const Integer& num1, const int num2)
{
	Integer res;
	Integer _num2(num2);

	mpz_sub(res.number, num1.number, _num2.number);

	return res;
}

Integer operator-(const int num1, const Integer& num2)
{
	Integer res;
	Integer _num1(num1);

	mpz_sub(res.number, _num1.number, num2.number);

	return res;
}

Integer operator*(const Integer& num1, const Integer& num2)
{
	Integer res;

	mpz_mul(res.number, num1.number, num2.number);

	return res;
}

Integer operator*(const Integer& num1, const int num2)
{
	Integer res;

	mpz_mul_si(res.number, num1.number, num2);

	return res;
}

Integer operator*(const int num1, const Integer& num2)
{
	Integer res;

	mpz_mul_si(res.number, num2.number, num1);

	return res;
}

Integer operator/(const Integer& num1, const Integer& num2)
{
	Integer res;

	if (num2 == 0)
		throw MathException(DivisionByZero);

	mpz_tdiv_q(res.number, num1.number, num2.number);

	return res;
}

Integer operator/(const Integer& num1, const int num2)
{
	Integer res;
	Integer _num2(num2);

	if (num2 == 0)
		throw MathException(DivisionByZero);

	//mpz_div(res.number, num1.number, _num2.number);
	mpz_divexact(res.number, num1.number, _num2.number);

	return res;
}

Integer operator/(const int num1, const Integer& num2)
{
	Integer res;
	Integer _num1(num1);

	if (num2 == 0)
		throw MathException(DivisionByZero);

	//mpz_div(res.number, _num1.number, num2.number);
	mpz_divexact(res.number, _num1.number, num2.number);

	return res;
}

Integer operator%(const Integer& num1, const Integer& num2)
{
	Integer res;

	if (num2 == 0)
		throw MathException(DivisionByZero);

	mpz_mod(res.number, num1.number, num2.number);

	return res;
}

Integer operator%(const Integer& num1, const int num2)
{
	Integer res;
	Integer _num2(num2);

	if (num2 == 0)
		throw MathException(DivisionByZero);

	mpz_mod(res.number, num1.number, _num2.number);

	return res;
}

Integer operator%(const int num1, const Integer& num2)
{
	Integer res;
	Integer _num1(num1);

	if (num2 == 0)
		throw MathException(DivisionByZero);

	mpz_mod(res.number, _num1.number, num2.number);

	return res;
}

Integer operator&(const Integer& num1, const Integer& num2)
{
	Integer res;

	mpz_and(res.number, num1.number, num2.number);

	return res;
}

Integer operator|(const Integer& num1, const Integer& num2)
{
	Integer res;

	mpz_ior(res.number, num1.number, num2.number);

	return res;
}

Integer operator^(const Integer& num1, const Integer& num2)
{
	Integer res;

	mpz_xor(res.number, num1.number, num2.number);

	return res;
}

Integer operator!(const Integer& num)
{
	auto s = num.ToString(2);
	std::u32string r;
	for (auto ch : s)
	{
		if (ch == U'1')
			r += U'0';
		else
			r += U'1';
	}
	return Integer::FromString(r, 2);
}

void Integer::operator+=(const Integer& num)
{
	*this = *this + num;
}

void Integer::operator-=(const Integer& num)
{
	*this = *this - num;
}

void Integer::operator*=(const Integer& num)
{
	*this = *this * num;
}

void Integer::operator/=(const Integer& num)
{
	*this = *this / num;
}

void Integer::operator%=(const Integer& num)
{
	*this = *this % num;
}

bool operator==(const Integer& num1, const Integer& num2)
{
	return mpz_cmp(num1.number, num2.number) == 0;
}

bool operator==(const Integer& num1, const int num2)
{
	return mpz_cmp_si(num1.number, num2) == 0;
}

bool operator==(const int num1, const Integer& num2)
{
	return mpz_cmp_si(num2.number, num1) == 0;
}

bool operator!=(const Integer& num1, const Integer& num2)
{
	return mpz_cmp(num1.number, num2.number) != 0;
}

bool operator!=(const Integer& num1, const int num2)
{
	return mpz_cmp_si(num1.number, num2) != 0;
}

bool operator!=(const int num1, const Integer& num2)
{
	return mpz_cmp_si(num2.number, num1) == 0;
}

bool operator>(const Integer& num1, const Integer& num2)
{
	return mpz_cmp(num1.number, num2.number) > 0;
}

bool operator>(const Integer& num1, const int num2)
{
	return mpz_cmp_si(num1.number, num2) > 0;
}

bool operator>(const int num1, const Integer& num2)
{
	return mpz_cmp_si(num2.number, num1) < 0;
}

bool operator>=(const Integer& num1, const Integer& num2)
{
	return mpz_cmp(num1.number, num2.number) >= 0;
}

bool operator>=(const Integer& num1, const int num2)
{
	return mpz_cmp_si(num1.number, num2) >= 0;
}

bool operator>=(const int num1, const Integer& num2)
{
	return mpz_cmp_si(num2.number, num1) <= 0;
}

bool operator<(const Integer& num1, const Integer& num2)
{
	return mpz_cmp(num1.number, num2.number) < 0;
}

bool operator<(const Integer& num1, const int num2)
{
	return mpz_cmp_si(num1.number, num2) < 0;
}

bool operator<(const int num1, const Integer& num2)
{
	return mpz_cmp_si(num2.number, num1) > 0;
}

bool operator<=(const Integer& num1, const Integer& num2)
{
	return mpz_cmp(num1.number, num2.number) <= 0;
}

bool operator<=(const Integer& num1, const int num2)
{
	return mpz_cmp_si(num1.number, num2) <= 0;
}

bool operator<=(const int num1, const Integer& num2)
{
	return mpz_cmp_si(num2.number, num1) >= 0;
}

void Integer::operator=(const int num)
{
	mpz_set_si(number, num);
}

Integer::operator int() const
{
	if (mpz_fits_slong_p(number) == 0)
		throw MathException(ConversionDoesNotFit);

	return mpz_get_si(number);
}

Integer pow(const Integer& num1, const Integer& num2)
{
	Integer res;

	mpz_pow_ui(res.number, num1.number, (int)num2);

	return res;
}

Integer abs(const Integer& num)
{
	Integer res;

	mpz_abs(res.number, num.number);

	return res;
}

Integer fact(const Integer& num)
{
	Integer res;

	mpz_fac_ui(res.number, (int)num);

	return res;
}

Integer bin(const std::u32string& str)
{
	return Integer::FromString(str, 2);
}

Integer oct(const std::u32string& str)
{
	return Integer::FromString(str, 8);
}

Integer dec(const std::u32string& str)
{
	return Integer::FromString(str, 10);
}

Integer hex(const std::u32string& str)
{
	return Integer::FromString(str, 16);
}

int Integer::GetPrecision() const
{
	return 0;
}

void Integer::SetPrecision(int precision)
{
}

bool Integer::IsNan()
{
	return false;
}

std::u32string Integer::ToString(const int base) const
{
	int len = mpz_sizeinbase(number, base) + 1;
	char *tmp = (char*)malloc(len + 1);

	char* str = mpz_get_str(tmp, base, number);
	std::u32string res(ToUtfString(str));
	free(tmp);

	return res;
}

std::u32string Integer::ToString(int exp, int accuracy) const
{
	return ToString(10);
}

std::string Integer::ToStdString(const int base) const
{
	return ToBasicString(ToString(base));
}

Integer Integer::FromString(const std::u32string& str, const int base)
{
	return Integer(str, base);
}

#ifdef TRACE_OUTPUT
void Integer::UpdateNumberStr()
{
	number_str = ToString(10);
}
#endif

}
