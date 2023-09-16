#include "stdafx.h"
#include "parser.h"

namespace yutovo_calculator
{

Integer bin(const std::u32string& str);
Integer oct(const std::u32string& str);
Integer dec(const std::u32string& str);
Integer hex(const std::u32string& str);

Integer pow(const Integer& num1, const Integer& num2);

Real pow(const Real& num1, const Real& num2);

Rational pow(const Rational& num1, const Rational& num2);

Real sin(const Real& num);
Real cos(const Real& num);
Real tg(const Real& num);
Real ctg(const Real& num);
Real sec(const Real& num);
Real cosec(const Real& num);
Real arcsin(const Real& num);
Real arccos(const Real& num);
Real arctg(const Real& num);
Real arcctg(const Real& num);
Real arcsec(const Real& num);
Real arccosec(const Real& num);
Real sh(const Real& num);
Real ch(const Real& num);
Real th(const Real& num);
Real cth(const Real& num);
Real sch(const Real& num);
Real csch(const Real& num);
Real arsh(const Real& num);
Real arch(const Real& num);
Real arth(const Real& num);
Real arcth(const Real& num);
Real arsch(const Real& num);
Real arcsch(const Real& num);

Real pi(const int precision);
Real exp(const int precision);

Real exp(const Real& num);

Real ln(const Real& num);
Real lg(const Real& num);
Real log(const Real& num1, const Real& num2);
Real sqrt(const Real& num);
Real root(const Real& num1, const Real& num2);

Real rad(const Real& num);
Real deg(const Real& num);
Real minute(const Real& num);
Real second(const Real& num);
Real grad(const Real& num);

template<>
Parser<yutovo_calculator::Integer>::Parser(const int precision) : 
	solver(precision)
{
	IntegerBinaryFunc binary_func = &pow;
	solver.AddBuiltinFunction("pow", binary_func);

	IntegerStringFunc string_func = &bin;
	solver.AddBuiltinFunction("bin", string_func);
	string_func = &oct;
	solver.AddBuiltinFunction("oct", string_func);
	string_func = &dec;
	solver.AddBuiltinFunction("dec", string_func);
	string_func = &hex;
	solver.AddBuiltinFunction("hex", string_func);
}

template<>
Parser<yutovo_calculator::Real>::Parser(const int precision) : 
	solver(precision)
{
	RealPrecisionVariable var;
	var = &pi;
	solver.AddBuiltinVariable("pi", var);
	var = &exp;
	solver.AddBuiltinVariable("e", var);
	
	RealUnaryFunc unary_func;
	unary_func = &exp;
	solver.AddBuiltinFunction("exp", unary_func);
	unary_func = &ln;
	solver.AddBuiltinFunction("ln", unary_func);
	unary_func = &lg;
	solver.AddBuiltinFunction("lg", unary_func);
	unary_func = &sqrt;
	solver.AddBuiltinFunction("sqrt", unary_func);

	unary_func = &rad;
	solver.AddBuiltinFunction("rad", unary_func);
	unary_func = &deg;
	solver.AddBuiltinFunction("deg", unary_func);
	unary_func = &minute;
	solver.AddBuiltinFunction("minute", unary_func);
	unary_func = &second;
	solver.AddBuiltinFunction("second", unary_func);
	unary_func = &grad;
	solver.AddBuiltinFunction("grad", unary_func);

	RealTrigonometricFunc trigonometric_func;
	trigonometric_func = &sin;
	solver.AddTrigonometricFunction("sin", trigonometric_func);
	trigonometric_func = &cos;
	solver.AddTrigonometricFunction("cos", trigonometric_func);
	trigonometric_func = &tg;
	solver.AddTrigonometricFunction("tg", trigonometric_func);
	trigonometric_func = &ctg;
	solver.AddTrigonometricFunction("ctg", trigonometric_func);
	trigonometric_func = &sec;
	solver.AddTrigonometricFunction("sec", trigonometric_func);
	trigonometric_func = &cosec;
	solver.AddTrigonometricFunction("cosec", trigonometric_func);
	trigonometric_func = &arcsin;
	solver.AddBuiltinFunction("arcsin", trigonometric_func);
	trigonometric_func = &arccos;
	solver.AddBuiltinFunction("arccos", trigonometric_func);
	trigonometric_func = &arctg;
	solver.AddBuiltinFunction("arctg", trigonometric_func);
	trigonometric_func = &arcctg;
	solver.AddBuiltinFunction("arcctg", trigonometric_func);
	trigonometric_func = &arcsec;
	solver.AddBuiltinFunction("arcsec", trigonometric_func);
	trigonometric_func = &arccosec;
	solver.AddBuiltinFunction("arccosec", trigonometric_func);
	trigonometric_func = &sh;
	solver.AddTrigonometricFunction("sh", trigonometric_func);
	trigonometric_func = &ch;
	solver.AddTrigonometricFunction("ch", trigonometric_func);
	trigonometric_func = &th;
	solver.AddTrigonometricFunction("th", trigonometric_func);
	trigonometric_func = &cth;
	solver.AddTrigonometricFunction("cth", trigonometric_func);
	trigonometric_func = &sch;
	solver.AddTrigonometricFunction("sch", trigonometric_func);
	trigonometric_func = &csch;
	solver.AddTrigonometricFunction("csch", trigonometric_func);
	trigonometric_func = &arsh;
	solver.AddBuiltinFunction("arsh", trigonometric_func);
	trigonometric_func = &arch;
	solver.AddBuiltinFunction("arch", trigonometric_func);
	trigonometric_func = &arth;
	solver.AddBuiltinFunction("arth", trigonometric_func);
	trigonometric_func = &arcth;
	solver.AddBuiltinFunction("arcth", trigonometric_func);
	trigonometric_func = &arsch;
	solver.AddBuiltinFunction("arsch", trigonometric_func);
	trigonometric_func = &arcsch;
	solver.AddBuiltinFunction("arcsch", trigonometric_func);

	RealBinaryFunc binary_func = &pow;
	solver.AddBuiltinFunction("pow", binary_func);
	binary_func = &log;
	solver.AddBuiltinFunction("log", binary_func);
	binary_func = &root;
	solver.AddBuiltinFunction("root", binary_func);

	AddUnits();
}

template<>
Parser<yutovo_calculator::Rational>::Parser(const int precision) : 
	solver(precision)
{
	RationalBinaryFunc binary_func = &pow;
	solver.AddBuiltinFunction("pow", binary_func);

	AddUnits();
}

};
