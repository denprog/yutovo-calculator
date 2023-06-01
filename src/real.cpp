#include "stdafx.h"
#include "real.h"
#include "utils.h"

namespace yutovo_calculator
{

//Real

Real pi(const int precision);

Real::Real()
{
	mpfr_init2(number, mpfr_get_default_prec());
	mpfr_set_si(number, 0, GMP_RNDN);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Real::Real(int precision)
{
	mpfr_init2(number, max((int)mpfr_get_default_prec(), precision));
	//mpfr_init2(number, precision);
	mpfr_set_si(number, 0, GMP_RNDN);
	//addPrecision = 0;

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Real::Real(int precision, const char* num)
{
	mpfr_init2(number, max((int)mpfr_get_default_prec(), precision));
	//mpfr_init2(number, strlen(num) + 1);
	mpfr_set_str(number, num, DEFAULT_BASE, MPFR_RNDZ);
	stringNumber = ToUtfString(num);
	//mpfr_prec_round(number, precision / 2, GMP_RNDN);
	//addPrecision = (int)strchr(num, '.');
	//if (!addPrecision)
	//	addPrecision = strlen(num);
	//else
	//	addPrecision -= (int)num;

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Real::Real(int precision, int num)
{
	mpfr_init2(number, max((int)mpfr_get_default_prec(), precision));
	//mpfr_init2(number, precision);
	mpfr_set_si(number, num, GMP_RNDN);
	//addPrecision = 1;

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Real::Real(int precision, float num)
{
	mpfr_init2(number, max((int)mpfr_get_default_prec(), precision));
	//mpfr_init2(number, precision);
	mpfr_set_d(number, num, GMP_RNDN);
	//addPrecision = 1;

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Real::Real(const std::u32string& num)
{
	stringNumber = num;
	mpfr_init2(number, max((int)mpfr_get_default_prec(), MathHelper::ToBitPrecision(num.length() * 2)) + 1);
	mpfr_set_str(number, ToBasicString(num).c_str(), DEFAULT_BASE, MPFR_RNDA);
	SetPrecision(GetPrecision() + GetExp());

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Real::Real(const Real& source)
{
	mpfr_init2(number, source.GetBitPrecision());
	mpfr_set(number, source.number, GMP_RNDN);
	//addPrecision = source.addPrecision;
	stringNumber = source.stringNumber;
	
#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

Real::~Real()
{
	mpfr_clear(number);
}

Real& Real::operator=(const Real& source)
{
	if (this == &source)
		return *this;

	mpfr_clear(number);
	mpfr_init2(number, source.GetBitPrecision());
	mpfr_set(number, source.number, GMP_RNDN);
	//addPrecision = source.addPrecision;
	stringNumber = source.stringNumber;

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif

	return *this;
}

Real& Real::operator=(const int num)
{
	mpfr_set_si(number, num, GMP_RNDN);
	//addPrecision = 1;

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif

	return *this;
}

Real& Real::operator=(const double num)
{
	mpfr_set_d(number, num, GMP_RNDN);
	//addPrecision = 1;

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif

	return *this;
}

Real& Real::operator=(const char* num)
{
	mpfr_t t;
	mpfr_init2(t, GetBitPrecision());

	if (mpfr_set_str(t, num, DEFAULT_BASE, GMP_RNDN) == 0)
	{
		mpfr_set(number, t, GMP_RNDN);
		mpfr_clear(t);
	}
	else
	{
		mpfr_clear(t);
	}

	stringNumber = ToUtfString(num);

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif

	return *this;
}

Real& Real::operator=(const std::u32string& num)
{
	mpfr_t t;

	if ((int)num.length() + 1 > GetPrecision())
	{
		SetBitPrecision(MathHelper::ToBitPrecision(num.length() * 2));
		mpfr_init2(t, GetBitPrecision());
	}
	else
		mpfr_init2(t, GetBitPrecision());

	if (mpfr_set_str(t, ToBasicString(num).c_str(), DEFAULT_BASE, GMP_RNDN) == 0)
	{
		mpfr_set(number, t, GMP_RNDN);
		mpfr_clear(t);
	}
	else
	{
		mpfr_clear(t);
	}

	stringNumber = num;

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif

	return *this;
}

Real Real::operator+()
{
	return *this;
}

Real Real::operator-()
{
	Real res(GetBitPrecision());

	mpfr_neg(res.number, number, GMP_RNDN);

	return res;
}

Real Real::operator++()
{
	*this += 1;

	return *this;
}

Real Real::operator--()
{
	*this -= 1;

	return *this;
}

Real operator+(const Real& num1, const Real& num2)
{
	Real res(max(num1.GetBitPrecision() + 2, num2.GetBitPrecision()) + 2);

	while (mpfr_add(res.number, num1.number, num2.number, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator+(const Real& num1, const int num2)
{
	Real res(num1.GetBitPrecision());

	while (mpfr_add_si(res.number, num1.number, num2, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator+(const int num1, const Real& num2)
{
	Real res(num2.GetBitPrecision());

	while (mpfr_add_si(res.number, num2.number, num1, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator+(const Real& num1, const float num2)
{
	Real res(num1.GetBitPrecision());

	while (mpfr_add_d(res.number, num1.number, num2, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator+(const float num1, const Real& num2)
{
	Real res(num2.GetBitPrecision());

	while (mpfr_add_d(res.number, num2.number, num1, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator-(const Real& num1, const Real& num2)
{
	Real res(max(num1.GetBitPrecision(), num2.GetBitPrecision()) + 1);

	mpfr_sub(res.number, num1.number, num2.number, GMP_RNDN);

#ifdef TRACE_OUTPUT
	res.UpdateNumberStr();
#endif

	return res;
}

Real operator-(const Real& num1, const int num2)
{
	Real res(num1.GetBitPrecision());

	mpfr_sub_si(res.number, num1.number, num2, DEFAULT_RND);

	return res;
}

Real operator-(const int num1, const Real& num2)
{
	Real res(num2.GetBitPrecision());

	mpfr_si_sub(res.number, num1, num2.number, DEFAULT_RND);

	return res;
}

Real operator-(const Real& num1, const float num2)
{
	Real res(num1.GetBitPrecision());

	mpfr_sub_d(res.number, num1.number, num2, DEFAULT_RND);

	return res;
}

Real operator-(const float num1, const Real& num2)
{
	Real res(num2.GetBitPrecision());

	mpfr_d_sub(res.number, num1, num2.number, DEFAULT_RND);

	return res;
}

Real operator*(const Real& num1, const Real& num2)
{
	Real res(max(num1.GetBitPrecision() * 2, num2.GetBitPrecision()) * 2);

	while (mpfr_mul(res.number, num1.number, num2.number, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator*(const Real& num1, const int num2)
{
	Real res(num1.GetBitPrecision());

	while (mpfr_mul_si(res.number, num1.number, num2, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator*(const int num1, const Real& num2)
{
	Real res(num2.GetBitPrecision());

	while (mpfr_mul_si(res.number, num2.number, num1, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator*(const Real& num1, const float num2)
{
	Real res(num1.GetBitPrecision());

	while (mpfr_mul_d(res.number, num1.number, num2, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator*(const float num1, const Real& num2)
{
	Real res(num2.GetBitPrecision());

	while (mpfr_mul_d(res.number, num2.number, num1, DEFAULT_RND) != 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real operator/(const Real& num1, const Real& num2)
{
	Real res(max(num1.GetBitPrecision() + 2, num2.GetBitPrecision() + 2));

	mpfr_div(res.number, num1.number, num2.number, GMP_RNDN);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(DivisionByZero);

#ifdef TRACE_OUTPUT
	res.UpdateNumberStr();
#endif

	return res;
}

Real operator/(const Real& num1, const int num2)
{
	Real res(num1.GetBitPrecision());

	mpfr_div_si(res.number, num1.number, num2, DEFAULT_RND);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	return res;
}

Real operator/(const int num1, const Real& num2)
{
	Real res(num2.GetBitPrecision());

	mpfr_si_div(res.number, num1, num2.number, DEFAULT_RND);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	return res;
}

Real operator/(const Real& num1, const float num2)
{
	Real res(num1.GetBitPrecision());

	mpfr_div_d(res.number, num1.number, num2, DEFAULT_RND);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	return res;
}

Real operator/(const float num1, const Real& num2)
{
	Real res(num2.GetBitPrecision());

	mpfr_d_div(res.number, num1, num2.number, DEFAULT_RND);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	return res;
}

void Real::operator+=(const Real& num)
{
	*this = *this + num;
}

void Real::operator+=(const int num)
{
	*this = *this + num;
}

void Real::operator+=(const float num)
{
	*this = *this + num;
}

void Real::operator-=(const Real& num)
{
	*this = *this - num;
}

void Real::operator-=(const int num)
{
	*this = *this - num;
}

void Real::operator-=(const float num)
{
	*this = *this - num;
}

void Real::operator*=(const Real& num)
{
	*this = *this * num;
}

void Real::operator*=(const int num)
{
	*this = *this * num;
}

void Real::operator*=(const float num)
{
	*this = *this * num;
}

void Real::operator/=(const Real& num)
{
	*this = *this / num;
}

void Real::operator/=(const int num)
{
	*this = *this / num;
}

void Real::operator/=(const float num)
{
	*this = *this / num;
}

Real::operator int() const
{
	if (IsInteger())
		return mpfr_get_si(number, DEFAULT_RND);
	else
		throw MathException(ConversionDoesNotFit);
}

bool operator==(const Real& num1, const Real& num2)
{
	if (num1.IsNaN() || num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_equal_p(num1.number, num2.number) != 0;
}

bool operator==(const Real& num1, const int num2)
{
	if (num1.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_si(num1.number, num2) == 0;
}

bool operator==(const int num1, const Real& num2)
{
	if (num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_si(num2.number, num1) == 0;
}

bool operator==(const Real& num1, const float num2)
{
	if (num1.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_d(num1.number, num2) == 0;
}

bool operator==(const float num1, const Real& num2)
{
	if (num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_d(num2.number, num1) == 0;
}

bool operator==(const Real& num1, const double num2)
{
	if (num1.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_d(num1.number, num2) == 0;
}

bool operator==(const double num1, const Real& num2)
{
	if (num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_d(num2.number, num1) == 0;
}

bool operator==(const Real& num1, const char* num2)
{
	return num1 == Real(num1.GetBitPrecision(), num2);
}

bool operator==(const char* num1, const Real& num2)
{
	return Real(num2.GetBitPrecision(), num1) == num2;
}

bool operator!=(const Real& num1, const Real& num2)
{
	if (num1.IsNaN() || num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_equal_p(num1.number, num2.number) == 0;
}

bool operator!=(const Real& num1, const int num2)
{
	if (num1.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_si(num1.number, num2) != 0;
}

bool operator!=(const int num1, const Real& num2)
{
	if (num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_si(num2.number, num1) != 0;
}

bool operator!=(const Real& num1, const float num2)
{
	if (num1.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_d(num1.number, num2) != 0;
}

bool operator!=(const float num1, const Real& num2)
{
	if (num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_d(num2.number, num1) != 0;
}

bool operator>(const Real& num1, const Real& num2)
{
	if (num1.IsNaN() || num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_greater_p(num1.number, num2.number) != 0;
}

bool operator>(const Real& num1, const int num2)
{
	if (num1.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_si(num1.number, num2) > 0;
}

bool operator>(const int num1, const Real& num2)
{
	Real _num1(num2.GetBitPrecision(), num1);

	return _num1 > num2;
}

bool operator>(const Real& num1, const float num2)
{
	if (num1.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_d(num1.number, num2) > 0;
}

bool operator>(const float num1, const Real& num2)
{
	Real _num1(num2.GetBitPrecision(), num1);

	return _num1 > num2;
}

bool operator>=(const Real& num1, const Real& num2)
{
	if (num1.IsNaN() || num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_greaterequal_p(num1.number, num2.number) != 0;
}

bool operator>=(const Real& num1, const int num2)
{
	return (num1 > num2) || (num1 == num2);
}

bool operator>=(const int num1, const Real& num2)
{
	return (num1 > num2) || (num1 == num2);
}

bool operator>=(const Real& num1, const float num2)
{
	return (num1 > num2) || (num1 == num2);
}

bool operator>=(const float num1, const Real& num2)
{
	return (num1 > num2) || (num1 == num2);
}

bool operator<(const Real& num1, const Real& num2)
{
	if (num1.IsNaN() || num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_less_p(num1.number, num2.number) != 0;
}

bool operator<(const Real& num1, const int num2)
{
	if (num1.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_si(num1.number, num2) < 0;
}

bool operator<(const int num1, const Real& num2)
{
	Real _num1(num2.GetBitPrecision(), num1);

	return _num1 < num2;
}

bool operator<(const Real& num1, const float num2)
{
	if (num1.IsNaN())
		throw MathException(Overflow);

	return mpfr_cmp_d(num1.number, num2) < 0;
}

bool operator<(const float num1, const Real& num2)
{
	Real _num1(num2.GetBitPrecision(), num1);

	return _num1 < num2;
}

bool operator<=(const Real& num1, const Real& num2)
{
	if (num1.IsNaN() || num2.IsNaN())
		throw MathException(Overflow);

	return mpfr_lessequal_p(num1.number, num2.number) != 0;
}

bool operator<=(const Real& num1, const int num2)
{
	return (num1 < num2) || (num1 == num2);
}

bool operator<=(const int num1, const Real& num2)
{
	return (num1 < num2) || (num1 == num2);
}

bool operator<=(const Real& num1, const float num2)
{
	return (num1 < num2) || (num1 == num2);
}

bool operator<=(const float num1, const Real& num2)
{
	return (num1 < num2) || (num1 == num2);
}

Real exp(const Real& num)
{
	Real res(num.GetBitPrecision());

	while (mpfr_exp(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	//mpfr_exp(res.number, num.number, DEFAULT_RND);

	return res;
}

Real ln(const Real& num)
{
	Real res(num.GetBitPrecision());

	while (mpfr_log(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	//mpfr_log(res.number, num.number, DEFAULT_RND);

	return res;
}

Real lg(const Real& num)
{
	Real res = ln(num) / ln(Real(num.GetBitPrecision(), 10));

	return res;
}

Real log(const Real& num1, const Real& num2)
{
	Real res = ln(num2) / ln(num1);

	return res;
}

Real sin(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_sin(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		return res;
	}

	Real misc = MathHelper::GetMisc<Real>(num);
	Real _pi = pi(num.GetBitPrecision());
	Real pi2 = pi(num.GetBitPrecision()) * 2;

	Real _num = num;
	Real mul = round(_num / pi2);

	_num -= mul * pi2;
	_num = abs(_num);

	if (_num <= misc)
	{
		res = 0;
	}
	else if (abs(_num - _pi) <= misc)
	{
		res = 0;
	}
	else
	{
		while (mpfr_sin(res.number, num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}
	}

	return res;
}

Real cos(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_cos(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		return res;
	}

	Real _pi = pi(num.GetBitPrecision());
	Real pi2 = pi(num.GetBitPrecision()) * 2;

	Real mul = round(num / pi2);
	Real _num = num;

	_num -= mul * pi2;
	_num = abs(_num);

	if (_num == 0)
	{
		res = 1;
	}
	else if (_num == _pi)
	{
		res = -1;
	}
	else if (_num == _pi / 2)
	{
		res = 0;
	}
	else if (_num == 3 * _pi / 2)
	{
		res = 0;
	}
	else
	{
		while (mpfr_cos(res.number, num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}
	}

	return res;
}

Real tg(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_tan(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		return res;
	}

	while (mpfr_tan(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	return res;
}

Real ctg(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_cot(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		return res;
	}

	while (mpfr_cot(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	return res;
}

Real sec(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_sec(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		return res;
	}

	while (mpfr_sec(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real cosec(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_csc(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		return res;
	}

	while (mpfr_csc(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real arcsin(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (abs(num) > 1)
		throw MathException(Overflow);

	while (mpfr_asin(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	if (angle_measure == DEGREE)
		res = res.RadianToDegree();
	else if (angle_measure == GRAD)
		res = res.RadianToGrad();

	return res;
}

Real arccos(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (abs(num) > 1)
		throw MathException(Overflow);

	while (mpfr_acos(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	if (angle_measure == DEGREE)
		res = res.RadianToDegree();
	else if (angle_measure == GRAD)
		res = res.RadianToGrad();

	return res;
}

Real arctg(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	while (mpfr_atan(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	if (angle_measure == DEGREE)
		res = res.RadianToDegree();
	else if (angle_measure == GRAD)
		res = res.RadianToGrad();

	return res;
}

Real arcctg(const Real& num, AngleMeasure angle_measure)
{
	Real _pi = pi(num.GetBitPrecision());

	if (angle_measure == DEGREE)
		_pi = _pi.RadianToDegree();
	else if (angle_measure == GRAD)
		_pi = _pi.RadianToGrad();

	Real res(_pi / 2 - arctg(num, angle_measure));

	return res;
}

Real arcsec(const Real& num, AngleMeasure angle_measure)
{
	Real res(arccos(1 / num, angle_measure));

	return res;
}

Real arccosec(const Real& num, AngleMeasure angle_measure)
{
	Real res(arcsin(1 / num, angle_measure));

	return res;
}

Real sh(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_sinh(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		//mpfr_sinh(res.number, _num.number, DEFAULT_RND);

		return res;
	}

	while (mpfr_sinh(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	//mpfr_sinh(res.number, num.number, DEFAULT_RND);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	return res;
}

Real ch(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_cosh(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		//mpfr_cosh(res.number, _num.number, DEFAULT_RND);

		return res;
	}

	while (mpfr_cosh(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	//mpfr_cosh(res.number, num.number, DEFAULT_RND);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	return res;
}

Real th(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_tanh(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		//mpfr_tanh(res.number, _num.number, DEFAULT_RND);

		return res;
	}

	while (mpfr_tanh(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	//mpfr_tanh(res.number, num.number, DEFAULT_RND);

	return res;
}

Real cth(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		while (mpfr_coth(res.number, _num.number, DEFAULT_RND) < 0)
		{
			if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
				throw MathException(Overflow);

			res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
		}

		//mpfr_coth(res.number, _num.number, DEFAULT_RND);

		return res;
	}

	while (mpfr_coth(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	//mpfr_coth(res.number, num.number, DEFAULT_RND);

	return res;
}

Real sch(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		mpfr_sech(res.number, _num.number, DEFAULT_RND);

		return res;
	}

	mpfr_sech(res.number, num.number, DEFAULT_RND);

	return res;
}

Real csch(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	if (angle_measure != RADIAN)
	{
		Real _num(num.GetBitPrecision());

		if (angle_measure == DEGREE)
			_num = num.DegreeToRadian();
		else if (angle_measure == GRAD)
			_num = num.GradToRadian();

		mpfr_csch(res.number, _num.number, DEFAULT_RND);

		return res;
	}

	mpfr_csch(res.number, num.number, DEFAULT_RND);

	return res;
}

Real arsh(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	mpfr_asinh(res.number, num.number, DEFAULT_RND);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	if (angle_measure == DEGREE)
		res = res.RadianToDegree();
	else if (angle_measure == GRAD)
		res = res.RadianToGrad();

	return res;
}

Real arch(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	mpfr_acosh(res.number, num.number, DEFAULT_RND);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	if (angle_measure == DEGREE)
		res = res.RadianToDegree();
	else if (angle_measure == GRAD)
		res = res.RadianToGrad();

	return res;
}

Real arth(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	mpfr_atanh(res.number, num.number, DEFAULT_RND);

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	if (angle_measure == DEGREE)
		res = res.RadianToDegree();
	else if (angle_measure == GRAD)
		res = res.RadianToGrad();

	return res;
}

Real arcth(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	//res = (ln((num + 1) / (num - 1))) / 2;
	res = arth(1 / num, angle_measure);

	if (angle_measure == DEGREE)
		res = res.RadianToDegree();
	else if (angle_measure == GRAD)
		res = res.RadianToGrad();

	return res;
}

Real arsch(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	//res = ln(sqrt((1 / num) - 1) * sqrt((1 / num) + 1) + (1 / num));
	res = arch(1 / num, angle_measure);

	if (angle_measure == DEGREE)
		res = res.RadianToDegree();
	else if (angle_measure == GRAD)
		res = res.RadianToGrad();

	return res;
}

Real arcsch(const Real& num, AngleMeasure angle_measure)
{
	Real res(num.GetBitPrecision());

	//res = ln(sqrt(1 + 1 / sqr(num)) + 1 / num);
	res = arsh(1 / num, angle_measure);

	if (angle_measure == DEGREE)
		res = res.RadianToDegree();
	else if (angle_measure == GRAD)
		res = res.RadianToGrad();

	return res;
}

Real pow(const Real& num1, const Real& num2)
{
	Real res(max(num1.GetBitPrecision() + 2, num2.GetBitPrecision() + 2));

	while (mpfr_pow(res.number, num1.number, num2.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	if (res.IsInfinity() || res.IsNaN())
		throw MathException(Overflow);

	res.Round(max(num1.GetBitPrecision(), num2.GetBitPrecision()));

	return res;
}

Real pow(const Real& num1, const int num2)
{
	Real res(num1.GetBitPrecision());

	while (mpfr_pow_si(res.number, num1.number, num2, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real sqr(const Real& num)
{
	Real res(num.GetBitPrecision());

	while (mpfr_sqr(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real sqrt(const Real& num)
{
	Real res(num.GetBitPrecision());

	while (mpfr_sqrt(res.number, num.number, DEFAULT_RND) < 0)
	{
		if (res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION >= MPFR_PREC_MAX)
			throw MathException(Overflow);

		res.SetBitPrecision(res.GetBitPrecision() + DEFAULT_INCREASE_PRECISION);
	}

	return res;
}

Real root(const Real& num1, const Real& num2)
{
	Real res(pow(num1, 1 / num2));

	return res;
}

Real abs(const Real& num)
{
	Real res(num.GetBitPrecision());

	mpfr_abs(res.number, num.number, DEFAULT_RND);

	return res;
}

Real floor(const Real& num)
{
	Real res(num.GetBitPrecision());

	mpfr_floor(res.number, num.number);

	return res;
}

Real fract(const Real& num)
{
	Real res(num.GetBitPrecision());

	mpfr_frac(res.number, num.number, DEFAULT_RND);

	return res;
}

Real trunc(const Real& num)
{
	Real res(num.GetBitPrecision());

	mpfr_trunc(res.number, num.number);

	return res;
}

Real ceil(const Real& num)
{
	Real res(num.GetBitPrecision());

	mpfr_ceil(res.number, num.number);

	return res;
}

Real round(const Real& num)
{
	Real res(num.GetBitPrecision());

	mpfr_round(res.number, num.number);

	return res;
}

Real integer(const Real& num)
{
	return trunc(num);
}

Real fraction(const Real& num)
{
	return fract(num);
}

Real fact(const Real& num)
{
	Real res(num.GetBitPrecision());

	if (!num.IsInteger())
		throw MathException(ArgumentIsOver);

	mpfr_fac_ui(res.number, (int)num, DEFAULT_RND);

	return res;
}

Real roundoff(const Real& num)
{
	Real res(num.GetBitPrecision());

	mpfr_round(res.number, num.number);

	return res;
}

Real exp(const int precision)
{
	return exp(Real(precision, 1));
}

Real pi(const int precision)
{
	Real res(precision);

	mpfr_const_pi(res.number, DEFAULT_RND);

	return res;
}

Real Real::DegreeToRadian() const
{
	Real res = pi(GetBitPrecision()) / 180 * *this;

	return res;
}

Real Real::RadianToDegree() const
{
	Real res = 180 / pi(GetBitPrecision()) * *this;

	return res;
}

Real Real::GradToRadian() const
{
	Real res = pi(GetBitPrecision()) / 180 * *this * (float)0.9;

	return res;
}

Real Real::RadianToGrad() const
{
	Real res = 180 / pi(GetBitPrecision()) * *this * (float)0.9;

	return res;
}

Real Real::DegreeToGrad() const
{
	Real res = *this / (float)0.9;

	return res;
}

Real Real::GradToDegree() const
{
	Real res = *this * (float)0.9;

	return res;
}

Real rad(const Real& num, AngleMeasure angle_measure)
{
	Real res(num);

	switch (angle_measure)
	{
	case DEGREE:
		res = res.RadianToDegree();
		break;
	case GRAD:
		res = res.RadianToGrad();
		break;
	}

	return res;
}

Real degree(const Real& num, AngleMeasure angle_measure)
{
	Real res = num;

	switch (angle_measure)
	{
	case RADIAN:
		res = res.DegreeToRadian();
		break;
	case GRAD:
		res = res.DegreeToGrad();
		break;
	}

	return res;
}

Real minute(const Real& num, AngleMeasure angle_measure)
{
	Real res = num;

	switch (angle_measure)
	{
	case RADIAN:
		res /= 60;
		res = res.DegreeToRadian();
		break;
	case DEGREE:
		res /=60;
		break;
	case GRAD:
		res /= 60;
		res = res.DegreeToGrad();
		break;
	}

	return res;
}

Real second(const Real& num, AngleMeasure angle_measure)
{
	Real res = num;

	switch (angle_measure)
	{
	case RADIAN:
		res /= 3600;
		res = res.DegreeToRadian();
		break;
	case DEGREE:
		res /= 3600;
		break;
	case GRAD:
		res /= 3600;
		res = res.DegreeToGrad();
		break;
	}

	return res;
}

Real grad(const Real& num, AngleMeasure angle_measure)
{
	Real res = num;

	switch (angle_measure)
	{
	case RADIAN:
		res = res.GradToRadian();
		break;
	case DEGREE:
		res = res.GradToDegree();
		break;
	}

	return res;
}

int Real::GetPrecision() const
{
	return MathHelper::ToDecimalPrecision(GetBitPrecision());
}

void Real::SetPrecision(int precision)
{
	if (stringNumber.empty())
		SetBitPrecision(max((int)mpfr_get_default_prec(), MathHelper::ToBitPrecision(precision + 2)));
	else
	{
		SetBitPrecision(max((int)mpfr_get_default_prec(), MathHelper::ToBitPrecision(precision + 2)));
		//renew the number because of not being precios getting by std::u32string
		mpfr_set_str(number, ToBasicString(stringNumber).c_str(), DEFAULT_BASE, MPFR_RNDA);
	}
	//SetBitPrecision(MathHelper::ToBitPrecision(precision + addPrecision + 1));

#ifdef TRACE_OUTPUT
	UpdateNumberStr();
#endif
}

void Real::RaisePrecision()
{
	SetPrecision(GetPrecision() + 1);
}

void Real::LowerPrecision()
{
	SetPrecision(GetPrecision() - 1);
}

void Real::LowerPrecision(int prec)
{
	if (GetBitPrecision() <= MathHelper::ToBitPrecision(prec))
		return;

	SetPrecision(prec);
}

std::u32string Real::ToString() const
{
	std::ostringstream s;
	mp_exp_t exp;

	char* c_str = mpfr_get_str(NULL, &exp, DEFAULT_BASE, 0, number, DEFAULT_RND);

	--exp;

	if (c_str == NULL)
		return U"";

	if (mpfr_number_p(number))
	{
		if (c_str[0] != '-')
			s << std::string(c_str).insert(1, 1, '.');
		else
			s << std::string(c_str).insert(2, 1, '.');
	}
	else
		s << std::string(c_str);

	mpfr_free_str(c_str);

	if (IsNumber() && !IsZero())
	{
		s << 'e';

		if (exp < 0)
			s << '-';
		else
			s << '+';

		if (std::abs(exp) < 10)
			s << '0';

		s << std::abs(exp);
	}

	return ToUtfString(s.str().c_str());
}

void Real::ToString(int exp, int accuracy, bool& mantissaSign, std::u32string& mantissa, bool& exponentSign, std::u32string& exponent) const
{
	Real res(abs(*this));
	char* numStr;
	mp_exp_t numExp;
	char buf[20];

	mantissaSign = GetSign();
	exponentSign = false;

	assert(accuracy >= 1);

	if (IsNaN() || IsInfinity())
		return;

	numStr = mpfr_get_str(NULL, &numExp, 10, accuracy + 1, number, DEFAULT_RND);

	if (numExp >= 0 && exp >= numExp)
	{
		mpfr_free_str(numStr);
		numStr = mpfr_get_str(NULL, &numExp, 10, numExp + accuracy, number, DEFAULT_RND);
	}
	else if (exp > ::abs(numExp))
	{
		mpfr_free_str(numStr);
		numStr = mpfr_get_str(NULL, &numExp, 10, accuracy, number, DEFAULT_RND);
	}

	mantissa = ToUtfString(numStr);
	mpfr_free_str(numStr);

	if (mantissaSign)
		mantissa.erase(0, 1);

	if (res >= 1)
	{
		if (numExp > exp)
		{
			mantissa.insert(1, 1, '.');

			--numExp;
#ifdef _WIN32
			_itoa_s(numExp, buf, 10);
#else
			sprintf(buf, "%d", (int)numExp);
#endif
			exponent = ToUtfString(buf);
		}
		else
		{
			mantissa.insert(numExp, 1, '.');
			exponent = U"";
		}
	}
	else
	{
		if (IsZero())
		{
			mantissa = U"0.";
			exponent = U"";
			return;
		}

		int i = ::abs(numExp);

		if (numExp <= 0)
		{
			while (i < (int)mantissa.size() - 1 && mantissa[i] == '0')
				++i;

			if (i >= exp)
			{
				mantissa.insert(1, 1, '.');

				--numExp;
				if (numExp < 0)
					exponentSign = true;

				if (numExp != 0)
				{
#ifdef _WIN32
				  _itoa_s(::abs(numExp), buf, 10);
#else
				  sprintf(buf, "%d", ::abs(numExp));
#endif
					exponent = ToUtfString(buf);
				}
				else
					exponent = U"";
			}
			else
			{
				mantissa.insert(0, 1, '0');
				mantissa.insert(1, 1, '.');

				if (numExp < 0)
					mantissa.insert(2, ::abs(numExp), '0');

				exponent = U"";
			}
		}
		else
		{
			mantissa.insert(1, 1, '.');

			--numExp;
			if (numExp < 0)
				exponentSign = true;

			if (numExp != 0)
			{
#ifdef _WIN32
				_itoa_s(::abs(numExp), buf, 10);
#else
				sprintf(buf, "%d", ::abs(numExp));
#endif
				exponent = ToUtfString(buf);
			}
			else
				exponent = U"";
		}
	}

	for (int i = mantissa.size() - 1; i > 0; --i)
	{
		if (mantissa[i] == '.')
			break;
		if (mantissa[i] == '0')
			mantissa.erase(i, 1);
		else
			break;
	}
}

void Real::ToString(int exp, int accuracy, bool& mantissaSign, std::string& mantissa, bool& exponentSign, std::string& exponent) const
{
	std::u32string m, e;
	ToString(exp, accuracy, mantissaSign, m, exponentSign, e);
	mantissa = ToBasicString(m);
	exponent = ToBasicString(e);
}

std::u32string Real::ToString(int exp, int accuracy) const
{
	bool mantissaSign;
	std::u32string mantissa;
	bool exponentSign;
	std::u32string exponent;
	
	ToString(exp, accuracy, mantissaSign, mantissa, exponentSign, exponent);
	
	std::u32string res;
	res += mantissaSign ? U"-" : U"";
	res += mantissa;
	res += U"E";
	res += exponentSign ? U"-" : U"+";
	res += exponent.empty() ? U"0" : exponent;
	
	return res;
}

std::string Real::ToStdString(int exp, int accuracy) const
{
	return ToBasicString(ToString(exp, accuracy));
}

Real Real::GetNumber()
{
	Real res(*this);

	return res;
}

//int Real::GetPrecision() const
//{
//	return MathHelper::ToDecimalPrecision(GetBitPrecision());
//}

//void Real::SetPrecision(int precision)
//{
//	//'precision' in this case means digits count after the point, so we need to add the leading digit
//	SetBitPrecision(MathHelper::ToBitPrecision(precision + 1));

//#ifdef TRACE_OUTPUT
//	UpdateNumberStr();
//#endif
//}

//std::u32string Real::ToString() const
//{
//	char buf[100];
//	
//	gmp_sprintf(buf, "%.Fe", number);
//	int p = mpf_get_prec(number);
//	
//	return std::u32string(buf);
//	
//	//std::ostringstream s;
//	//mp_exp_t exp;

//	//char* c_str = mpf_get_str(NULL, &exp, DEFAULT_BASE, 0, number);

//	//--exp;

//	//if (c_str == NULL)
//	//	return "";

//	////if (mpf_number_p(number))
//	////{
//	////	if (c_str[0] != '-')
//	////		s << std::u32string(c_str).insert(1, 1, '.');
//	////	else
//	////		s << std::u32string(c_str).insert(2, 1, '.');
//	////}
//	////else
//	//	s << std::u32string(c_str);

//	////mpf_free_str(c_str);
//	//free_function(c_str);

//	////if (IsNumber() && !IsZero())
//	//{
//	//	s << 'e';

//	//	if (exp < 0)
//	//		s << '-';
//	//	else
//	//		s << '+';

//	//	if (std::abs(exp) < 10)
//	//		s << '0';

//	//	s << std::abs(exp);
//	//}

//	//std::u32string res = s.str();

//	//return res;
//}

#ifdef TRACE_OUTPUT
void Real::UpdateNumberStr()
{
	number_str = ToString();
}
#endif

}
