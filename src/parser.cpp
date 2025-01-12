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
Complex pow(const Complex& num1, const Complex& num2, int& res_pos);

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

Complex sin(const Complex& num, int& res_pos);
Complex cos(const Complex& num, int& res_pos);
Complex tg(const Complex& num, int& res_pos);
Complex ctg(const Complex& num, int& res_pos);
Complex sec(const Complex& num, int& res_pos);
Complex cosec(const Complex& num, int& res_pos);
Complex arcsin(const Complex& num, int& res_pos);
Complex arccos(const Complex& num, int& res_pos);
Complex arctg(const Complex& num, int& res_pos);
Complex arcctg(const Complex& num, int& res_pos);
Complex arcsec(const Complex& num, int& res_pos);
Complex arccosec(const Complex& num, int& res_pos);
Complex sh(const Complex& num, int& res_pos);
Complex ch(const Complex& num, int& res_pos);
Complex th(const Complex& num, int& res_pos);
Complex cth(const Complex& num, int& res_pos);
Complex sch(const Complex& num, int& res_pos);
Complex csch(const Complex& num, int& res_pos);
Complex arsh(const Complex& num, int& res_pos);
Complex arch(const Complex& num, int& res_pos);
Complex arth(const Complex& num, int& res_pos);
Complex arcth(const Complex& num, int& res_pos);
Complex arsch(const Complex& num, int& res_pos);
Complex arcsch(const Complex& num, int& res_pos);

Complex pi_complex(const int precision);
Complex exp_complex(const int precision);

Complex exp(const Complex& num, int& res_pos);

Complex ln(const Complex& num, int& res_pos);
Complex lg(const Complex& num, int& res_pos);
Complex log(const Complex& num1, const Complex& num2, int& res_pos);
Complex sqrt(const Complex& num, int& res_pos);
Complex root(const Complex& num1, const Complex& num2, int& res_pos);

Complex rad(const Complex& num, int& res_pos);
Complex deg(const Complex& num, int& res_pos);
Complex minute(const Complex& num, int& res_pos);
Complex second(const Complex& num, int& res_pos);
Complex grad(const Complex& num, int& res_pos);

template<>
Parser<yutovo_calculator::Integer>::Parser(const int precision, const Language _language) : 
    solver(precision, AngleMeasure::Radian),
    language(_language)
{
    pthread_getcpuclockid(pthread_self(), &thread_clock_id);
    
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
Parser<yutovo_calculator::Real>::Parser(const int precision, const Language _language) : 
    solver(precision, AngleMeasure::Radian),
    language(_language)
{
    pthread_getcpuclockid(pthread_self(), &thread_clock_id);
    
    RealPrecisionVariable var;
    var = &pi;
    solver.AddBuiltinVariable("pi", var);
    solver.AddBuiltinVariable("π", var);
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

    InitUnits();
    InitPhisicalConstants();
}

template<>
Parser<yutovo_calculator::Rational>::Parser(const int precision, const Language _language) : 
    solver(precision, AngleMeasure::Radian),
    language(_language)
{
    pthread_getcpuclockid(pthread_self(), &thread_clock_id);

    RationalBinaryFunc binary_func = &pow;
    solver.AddBuiltinFunction("pow", binary_func);

    InitUnits();
}

template<>
Parser<yutovo_calculator::Complex>::Parser(const int precision, const Language _language) : 
    solver(precision, AngleMeasure::Radian),
    language(_language)
{
    pthread_getcpuclockid(pthread_self(), &thread_clock_id);
    
    ComplexPrecisionVariable var;
    var = &pi_complex;
    solver.AddBuiltinVariable("pi", var);
    var = &exp_complex;
    solver.AddBuiltinVariable("e", var);
    
    ComplexUnaryFunc unary_func;
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

    ComplexTrigonometricFunc trigonometric_func;
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

    ComplexBinaryFunc binary_func = &pow;
    solver.AddBuiltinFunction("pow", binary_func);
    binary_func = &log;
    solver.AddBuiltinFunction("log", binary_func);
    binary_func = &root;
    solver.AddBuiltinFunction("root", binary_func);

    switch (language)
    {
    case Language::Russian:
        solver.im = U"j";
        break;
    default:
        solver.im = U"i";
        break;
    }
}

template<>
void Parser<yutovo_calculator::Integer>::SetLocale(Language _language)
{
    language = _language;
    last_language = language;
}

template<>
void Parser<yutovo_calculator::Real>::SetLocale(Language _language)
{
    language = _language;
    InitUnits();
    InitPhisicalConstants();
    last_language = language;
}

template<>
void Parser<yutovo_calculator::Rational>::SetLocale(Language _language)
{
    language = _language;
    InitUnits();
    last_language = language;
}

template<>
void Parser<yutovo_calculator::Complex>::SetLocale(Language _language)
{
    language = _language;
    switch (language)
    {
    case Language::Russian:
        solver.im = U"j";
        break;
    default:
        solver.im = U"i";
        break;
    }
    last_language = language;
}

template<>
void Parser<yutovo_calculator::Real>::InitAngleUnits()
{
    for (int i = 1; i <= 5; ++i)
        solver.RemoveIdentifier(LogicalId{0, -1, 3, i});
    switch (language)
    {
    case Language::English:
        Parse(LogicalId{0, -1, 3, 1}, U"°~(pi)/(180)rad;");
        Parse(LogicalId{0, -1, 3, 2}, U"deg~°;");
        Parse(LogicalId{0, -1, 3, 3}, U"'~°/60;");
        Parse(LogicalId{0, -1, 3, 4}, U"''~(')/(60);");
        Parse(LogicalId{0, -1, 3, 5}, U"grad~(9)/(10)°;");
        break;
    case Language::Russian:
        Parse(LogicalId{0, -1, 3, 1}, U"°~(pi)/(180)рад;");
        Parse(LogicalId{0, -1, 3, 2}, U"deg~°;");
        Parse(LogicalId{0, -1, 3, 3}, U"'~°/60;");
        Parse(LogicalId{0, -1, 3, 4}, U"''~(')/(60);");
        Parse(LogicalId{0, -1, 3, 5}, U"град~(9)/(10)°;");
        break;
    default:
        break;
    }
}

};
