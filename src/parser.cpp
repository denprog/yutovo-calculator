#include "stdafx.h"
#include "parser.h"

namespace yutovo_calculator
{
  Integer pow(const Integer& num1, const Integer& num2);

  Real pow(const Real& num1, const Real& num2);

  Real sin(const Real& num, AngleMeasure angle_measure);
  Real cos(const Real& num, AngleMeasure angle_measure);
  Real tg(const Real& num, AngleMeasure angle_measure);
  Real ctg(const Real& num, AngleMeasure angle_measure);
  Real sec(const Real& num, AngleMeasure angle_measure);
  Real cosec(const Real& num, AngleMeasure angle_measure);
  Real arcsin(const Real& num, AngleMeasure angle_measure);
  Real arccos(const Real& num, AngleMeasure angle_measure);
  Real arctg(const Real& num, AngleMeasure angle_measure);
  Real arcctg(const Real& num, AngleMeasure angle_measure);
  Real arcsec(const Real& num, AngleMeasure angle_measure);
  Real arccosec(const Real& num, AngleMeasure angle_measure);
  Real sh(const Real& num, AngleMeasure angle_measure);
  Real ch(const Real& num, AngleMeasure angle_measure);
  Real th(const Real& num, AngleMeasure angle_measure);
  Real cth(const Real& num, AngleMeasure angle_measure);
  Real sch(const Real& num, AngleMeasure angle_measure);
  Real csch(const Real& num, AngleMeasure angle_measure);
  Real arsh(const Real& num, AngleMeasure angle_measure);
  Real arch(const Real& num, AngleMeasure angle_measure);
  Real arth(const Real& num, AngleMeasure angle_measure);
  Real arcth(const Real& num, AngleMeasure angle_measure);
  Real arsch(const Real& num, AngleMeasure angle_measure);
  Real arcsch(const Real& num, AngleMeasure angle_measure);

  Real pi(const int precision);
  Real exp(const int precision);

  Real exp(const Real& num);

  Real ln(const Real& num);
  Real lg(const Real& num);
  Real log(const Real& num1, const Real& num2);
  Real sqrt(const Real& num);
  Real root(const Real& num1, const Real& num2);

	/**
	 * Constructor.
	 * @tparam yutovo_calculator::Integer Integer type of the big numbers parser.
	 * @param precision The precision.
	 */
	template<>
	Parser<yutovo_calculator::Integer>::Parser(const int precision) : solver(precision)
	{
		IntegerBinaryFunc binary_func = &pow;
		solver.AddBuildinFunction("pow", binary_func);
	}

	/**
	 * Constructor.
	 * @tparam yutovo_calculator::Real Real type of the big numbers parser.
	 * @param precision The precision.
	 */
	template<>
	Parser<yutovo_calculator::Real>::Parser(const int precision) : solver(precision)
	{
		RealPrecisionVariable var;
		var = &pi;
		solver.AddBuildinVariable("pi", var);
		var = &exp;
		solver.AddBuildinVariable("e", var);
		
		RealUnaryFunc unary_func;
		unary_func = &exp;
		solver.AddBuildinFunction("exp", unary_func);
		unary_func = &ln;
		solver.AddBuildinFunction("ln", unary_func);
		unary_func = &lg;
		solver.AddBuildinFunction("lg", unary_func);
		unary_func = &sqrt;
		solver.AddBuildinFunction("sqrt", unary_func);
		
		RealTrigonometricFunc trigonometric_func;
		trigonometric_func = &sin;
		solver.AddBuildinFunction("sin", trigonometric_func);
		trigonometric_func = &cos;
		solver.AddBuildinFunction("cos", trigonometric_func);
		trigonometric_func = &tg;
		solver.AddBuildinFunction("tg", trigonometric_func);
		trigonometric_func = &ctg;
		solver.AddBuildinFunction("ctg", trigonometric_func);
		trigonometric_func = &sec;
		solver.AddBuildinFunction("sec", trigonometric_func);
		trigonometric_func = &cosec;
		solver.AddBuildinFunction("cosec", trigonometric_func);
		trigonometric_func = &arcsin;
		solver.AddBuildinFunction("arcsin", trigonometric_func);
		trigonometric_func = &arccos;
		solver.AddBuildinFunction("arccos", trigonometric_func);
		trigonometric_func = &arctg;
		solver.AddBuildinFunction("arctg", trigonometric_func);
		trigonometric_func = &arcctg;
		solver.AddBuildinFunction("arcctg", trigonometric_func);
		trigonometric_func = &arcsec;
		solver.AddBuildinFunction("arcsec", trigonometric_func);
		trigonometric_func = &arccosec;
		solver.AddBuildinFunction("arccosec", trigonometric_func);
		trigonometric_func = &sh;
		solver.AddBuildinFunction("sh", trigonometric_func);
		trigonometric_func = &ch;
		solver.AddBuildinFunction("ch", trigonometric_func);
		trigonometric_func = &th;
		solver.AddBuildinFunction("th", trigonometric_func);
		trigonometric_func = &cth;
		solver.AddBuildinFunction("cth", trigonometric_func);
		trigonometric_func = &sch;
		solver.AddBuildinFunction("sch", trigonometric_func);
		trigonometric_func = &csch;
		solver.AddBuildinFunction("csch", trigonometric_func);
		trigonometric_func = &arsh;
		solver.AddBuildinFunction("arsh", trigonometric_func);
		trigonometric_func = &arch;
		solver.AddBuildinFunction("arch", trigonometric_func);
		trigonometric_func = &arth;
		solver.AddBuildinFunction("arth", trigonometric_func);
		trigonometric_func = &arcth;
		solver.AddBuildinFunction("arcth", trigonometric_func);
		trigonometric_func = &arsch;
		solver.AddBuildinFunction("arsch", trigonometric_func);
		trigonometric_func = &arcsch;
		solver.AddBuildinFunction("arcsch", trigonometric_func);
	
		RealBinaryFunc binary_func = &pow;
		solver.AddBuildinFunction("pow", binary_func);
		binary_func = &log;
		solver.AddBuildinFunction("log", binary_func);
		binary_func = &root;
		solver.AddBuildinFunction("root", binary_func);
	}

	/**
	 * Constructor.
	 * @tparam yutovo_calculator::Rational Rational type of the big numbers parser.
	 * @param precision The precision.
	 */
	template<>
	Parser<yutovo_calculator::Rational>::Parser(const int precision) : solver(precision)
	{
	}
};
