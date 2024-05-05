#ifndef PARSER_H
#define PARSER_H

#include "stdafx.h"
#include "integer.h"
#include "rational.h"
#include "real.h"
#include "complex.h"
#include "ast.h"
#include "script.h"
#include "solver.h"
#include "error_handler.h"
#include "utils.h"

using namespace std;

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace fusion = boost::fusion;

namespace yutovo_calculator
{

//Big numbers parser
template<typename Number>
struct Parser
{
    Parser(const int precision, const Language _language, uint64_t _max_time);
    
    Number Parse(ElementId id, std::u32string expression, Dependencies* dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
        Notation default_notation, const int precision = -1)
    {
        if (expression.empty() || expression == U";")
            throw SyntaxException(id, ExpressionExpected, 0, 0);
        
        std::u32string::iterator iter = expression.begin();
        std::u32string::iterator end = expression.end();
        unicode::space_type space;

        solver.SetDefaultNotation(default_notation);

        Script<Number> script(id, expression, &solver);
        ScriptNode<Number> script_node;

        phrase_parse(iter, end, script, space, script_node);
        return solver(script_node, id, default_angle_measure, result_angle_measure, precision, dependencies);
    }

    Number Parse(ElementId id, std::u32string expression, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, const int precision = -1)
    {
        Dependencies dependencies;
        return Parse(id, expression, &dependencies, default_angle_measure, result_angle_measure, Notation::Decimal, precision);
    }

    Number Parse(ElementId id, std::string expression, Dependencies* dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
        const int precision = -1)
    {
        return Parse(id, ToUtfString(expression), dependencies, default_angle_measure, result_angle_measure, Notation::Decimal, precision);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies* dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
        const int precision = -1)
    {
        return Parse(id, expression, dependencies, default_angle_measure, result_angle_measure, Notation::Decimal, precision);
    }

    Number Parse(ElementId id, std::u32string expression)
    {
        Dependencies dependencies;
        return Parse(id, expression, &dependencies, AngleMeasure::Radian, AngleMeasure::None, Notation::Decimal);
    }

    Number Parse(ElementId id, std::u32string expression, const int precision)
    {
        Dependencies dependencies;
        return Parse(id, expression, &dependencies, AngleMeasure::Radian, AngleMeasure::None, Notation::Decimal, precision);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies* dependencies)
    {
        return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None, Notation::Decimal);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies* dependencies, Notation default_notation)
    {
        return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None, default_notation);
    }

    Number Parse(ElementId id, std::string expression, Dependencies* dependencies, Notation default_notation)
    {
        return Parse(id, ToUtfString(expression), dependencies, AngleMeasure::Radian, AngleMeasure::None, default_notation);
    }

    Number Parse(ElementId id, std::string expression, Dependencies* dependencies)
    {
        return Parse(id, ToUtfString(expression), dependencies, AngleMeasure::Radian, AngleMeasure::None);
    }

    Number Parse(ElementId id, std::u32string expression, Notation default_notation)
    {
        Dependencies dependencies;
        return Parse(id, expression, &dependencies, AngleMeasure::Radian, AngleMeasure::None, default_notation);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies* dependencies, 
        AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, const int precision, const int res_count, 
        std::vector<Number>& results)
    {
        if (expression.empty() || expression == U";")
            throw SyntaxException(id, ExpressionExpected, 0, 0);
        
        std::u32string::iterator iter = expression.begin();
        std::u32string::iterator end = expression.end();
        unicode::space_type space;

        Script<Complex> script(id, expression, &solver);
        ScriptNode<Complex> script_node;

        phrase_parse(iter, end, script, space, script_node);

        for (int i = 0; i < res_count; ++i)
        {
            solver.res_pos = i;
            Number r = solver(script_node, id, default_angle_measure, result_angle_measure, precision, dependencies);
            if (std::find(results.begin(), results.end(), r) != results.end())
                break;
            results.push_back(r);
        }
        if (results.empty())
            return Number();
        return results[0];
    }

    Number Parse(ElementId id, std::string expression, Dependencies* dependencies, 
        AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, const int precision, const int res_count, 
        std::vector<Number>& results)
    {
        return Parse(id, ToUtfString(expression), dependencies, default_angle_measure, result_angle_measure, precision, res_count, results);
    }

    bool RemoveIdentifier(ElementId id, const std::u32string& name)
    {
        return solver.RemoveIdentifier(id, name);
    }

    bool RemoveIdentifier(ElementId id, const std::string& name)
    {
        return RemoveIdentifier(id, ToUtfString(name));
    }

    Number GetSuitableUnit(ElementId id, const Number& val)
    {
        return solver.GetSuitableUnit(id, val, solver.symbols->last_unit_system, solver.symbols->buildin_elements);
    }

    void GetCastUnits(const ElementId id, const Number& val, std::vector<Unit>& cast_units)
    {
        solver.GetCastUnits(id, val, cast_units);
    }

    Number CastToUnit(const ElementId id, const Number& val, const Unit& unit)
    {
        return solver.CastToUnit(id, val, unit);
    }

    void SetLocale(Language _language);

    void ListBuiltinVariables(std::vector<std::u32string>& variables)
    {
        solver.ListBuiltinVariables(variables);
    }

    void ListBuiltinFunctions(std::vector<std::u32string>& functions)
    {
        solver.ListBuiltinFunctions(functions);
    }

    void ListUserVariables(std::vector<std::pair<std::u32string, std::u32string>>& variables)
    {
        solver.ListUserVariables(variables);
    }

    void ListUserFunctions(std::vector<std::u32string>& functions)
    {
        solver.ListUserFunctions(functions);
    }

    void ListBuiltinUnits(std::vector<std::u32string>& units)
    {
        solver.ListBuiltinUnits(units);
    }

    void ListUserUnits(std::vector<CustomUnit<Number>>& units)
    {
        solver.ListUserUnits(units);
    }

    void SetMaxTime(uint64_t max_time)
    {
        solver.SetMaxTime(max_time);
    }

private:
    void InitUnits()
    {
        solver.ResetBuildinUnits();

        switch (language)
        {
        case Language::English:
            solver.AddBuiltinUnit(Unit(U"m")); //meter (length)
            solver.AddBuiltinUnit(Unit(U"kg")); //kilogram (mass)
            solver.AddBuiltinUnit(Unit(U"s")); //second (time)
            solver.AddBuiltinUnit(Unit(U"mol")); //mole (amount of matter)
            solver.AddBuiltinUnit(Unit(U"A")); //ampere (electric current)
            solver.AddBuiltinUnit(Unit(U"cd")); //candella (luminosity)
            solver.AddBuiltinUnit(Unit(U"K")); //kelvin (temperature)
            solver.AddBuiltinUnit(Unit(U"rad")); //radian (angle)
            solver.AddBuiltinUnit(Unit(U"sr")); //steradian (solid angle)
            solver.AddBuiltinUnit(Unit(U"bit")); //bit (data)
            break;
        case Language::Russian:
            solver.AddBuiltinUnit(Unit(U"м")); //meter (length)
            solver.AddBuiltinUnit(Unit(U"кг")); //kilogram (mass)
            solver.AddBuiltinUnit(Unit(U"сек")); //second (time)
            solver.AddBuiltinUnit(Unit(U"моль")); //mole (amount of matter)
            solver.AddBuiltinUnit(Unit(U"А")); //ampere (electric current)
            solver.AddBuiltinUnit(Unit(U"Кд")); //candella (luminosity)
            solver.AddBuiltinUnit(Unit(U"К")); //kelvin (temperature)
            solver.AddBuiltinUnit(Unit(U"рад")); //radian (angle)
            solver.AddBuiltinUnit(Unit(U"ср")); //steradian (solid angle)
            solver.AddBuiltinUnit(Unit(U"бит")); //bit (data)
            break;
        default:
            throw ParserException({}, ParserExceptionCode::UnknownLanguage);
        }

        solver.symbols->buildin_elements = true;

        InitSIUnits();
        InitOtherUnits();
        InitRussianUnits();
        InitAngleUnits();

        solver.symbols->buildin_elements = false;
    }

    void InitSIUnits()
    {
        auto it = si_units.find(last_language);
        if (it != si_units.end())
        {
            int p = 0;
            for (auto& expr : it->second)
                solver.RemoveIdentifier(ElementId{0, -2, 0, 0, p++});
        }

        it = si_units.find(language);
        if (it == si_units.end())
            return;
        int p = 0;
        for (auto& expr : it->second)
            Parse(ElementId{0, -2, 0, 0, p++}, expr);
    }

    void InitOtherUnits()
    {
        auto it = other_units.find(last_language);
        if (it != other_units.end())
        {
            int p = 0;
            for (auto& expr : it->second)
                solver.RemoveIdentifier(ElementId{0, -2, 1, 0, p++});
        }

        it = other_units.find(language);
        if (it == other_units.end())
            return;
        int p = 0;
        for (auto& expr : it->second)
            Parse(ElementId{0, -2, 1, 0, p++}, expr);
    }

    void InitRussianUnits()
    {
        auto it = russian_units.find(last_language);
        if (it != russian_units.end())
        {
            int p = 0;
            for (auto& expr : it->second)
                solver.RemoveIdentifier(ElementId{0, -2, 2, 0, p++});
        }

        it = russian_units.find(language);
        if (it == russian_units.end())
            return;
        int p = 0;
        for (auto& expr : it->second)
            Parse(ElementId{0, -2, 2, 0, p++}, expr);
    }

    void InitAngleUnits()
    {
    }

    void InitPhisicalConstants()
    {
        auto it = phisical_constants.find(last_language);
        if (it != phisical_constants.end())
        {
            int p = 0;
            for (auto& expr : it->second)
                solver.RemoveIdentifier(ElementId{0, -1, 0, 0, p++});
        }

        it = phisical_constants.find(language);
        if (it == phisical_constants.end())
            return;
        int p = 0;
        for (auto& expr : it->second)
            Parse(ElementId{0, -1, 0, 0, p++}, expr);
    }

    Solver<Number> solver;

    Language language = Language::English;
    Language last_language = Language::None;

    uint64_t max_time = 0;

    std::map<Language, std::vector<std::u32string>> si_units = 
        {
            {
                Language::English,
                {
                    //distance
                    U"ang~0.000000000001m;",
                    U"nm~0.000000001m;",
                    U"mcm~0.000001m;",
                    U"mm~0.001m;",
                    U"cm~0.01m;",
                    U"dm~0.1m;",
                    U"km~1000m;",

                    //time
                    U"ps~0.000000000001s;",
                    U"ns~0.000000001s;",
                    U"mcs~0.000001s;",
                    U"ms~0.001s;",
                    U"min~60s;",
                    U"hour~60min;",
                    U"day~24hour;",
                    U"week~7day;",

                    //mass
                    U"g~0.001kg;",
                    U"mcg~0.000001kg;",
                    U"mg~0.001kg;",
                    U"ton~1000kg;",

                    //electric current
                    U"mcA~0.000001A;",
                    U"mA~0.001A;",
                    U"kA~1000A;",
                    U"MA~1000000A;",

                    //frequency
                    U"Hz~1/s;",
                    U"kHz~1000Hz;",
                    U"MHz~1000kHz;",
                    U"GHz~1000MHz;",

                    //force
                    U"N~(kg*m)/(pow(s,2));",
                    U"mcN~0.000001N;",
                    U"mN~0.001N;",
                    U"kN~1000N;",
                    U"MN~1000kN;",

                    //energy
                    U"J~N*m;",
                    U"mcJ~0.000001J;",
                    U"mJ~0.001J;",
                    U"kJ~1000J;",
                    U"MJ~1000000J;",

                    //power
                    U"W~(J)/(s);",
                    U"mcW~0.000001W;",
                    U"mW~0.001W;",
                    U"kW~1000W;",
                    U"MW~1000000W;",

                    //pressure
                    U"Pa~(N)/(pow(m,2));",
                    U"mcPa~0.000001Pa;",
                    U"mPa~0.001Pa;",
                    U"kPa~1000Pa;",
                    U"MPa~1000000Pa;",

                    //luminous intensity
                    U"mccd~0.000001cd;",
                    U"mcd~0.001cd;",
                    U"kcd~1000cd;",
                    U"Mcd~1000000cd;",

                    //luminous flux
                    U"lm~cd*sr;",
                    U"mclm~0.000001lm;",
                    U"mlm~0.001lm;",
                    U"klm~1000lm;",
                    U"Mlm~1000000lm;",

                    //illuminance
                    U"lx~lm/pow(m,2);",
                    U"mclx~0.000001lx;",
                    U"mlx~0.001lx;",
                    U"klx~1000lx;",
                    U"Mlx~1000000lx;",

                    //electrical charge
                    U"C~A*s;",
                    U"mcC~0.000001C;",
                    U"mC~0.001C;",
                    U"kC~1000C;",
                    U"MC~1000000C;",

                    //potential
                    U"V~(J)/(C);",
                    U"pV~0.000000000001V;",
                    U"nV~0.000000001V;",
                    U"mcV~0.000001V;",
                    U"mV~0.001V;",
                    U"kV~1000V;",
                    U"MV~1000000V;",

                    //electrical resistance
                    U"Ohm~V/A;",
                    U"mcOhm~0.000001Ohm;",
                    U"mOhm~0.001Ohm;",
                    U"kOhm~1000Ohm;",
                    U"MOhm~1000000Ohm;",

                    //electrical capacity
                    U"F~C/V;",
                    U"pF~0.000000000001F;",
                    U"nF~0.000000001F;",
                    U"mcF~0.000001F;",
                    U"mF~0.001F;",

                    //magnetic flux
                    U"Wb~(kg*pow(m,2)/(pow(s,2)*A));",
                    U"mcWb~0.000001Wb;",
                    U"mWb~0.001Wb;",
                    U"kWb~1000Wb;",
                    U"MWb~1000000Wb;",

                    //magnetic field
                    U"T~(Wb)/(pow(m,2));",
                    U"mcT~0.000001T;",
                    U"mT~0.001T;",
                    U"kT~1000T;",
                    U"MT~1000000T;",

                    //electrical inductance
                    U"H~(kg*pow(m,2))/(pow(s,2)*pow(A,2));",
                    U"pH~0.000000000001H;",
                    U"nH~0.000000001H;",
                    U"mcH~0.000001H;",
                    U"mH~0.001H;",

                    //electric conductance
                    U"S~(1)/(Ohm);",
                    U"mcS~0.000001Ohm;",
                    U"mS~0.001Ohm;",
                    U"kS~1000Ohm;",
                    U"MS~1000000Ohm;",

                    //ionizing radiation
                    U"Gy~(J)/(kg);",
                    U"mcGy~0.000001Gy;",
                    U"mGy~0.001Gy;",
                    U"kGy~1000Gy;",
                    U"MGy~1000000Gy;"
                }
            },
            {
                Language::Russian,
                {
                    //distance
                    U"анг~0.0000000001м;",
                    U"нм~0.000000001м;",
                    U"мкм~0.000001м;",
                    U"мм~0.001м;",
                    U"см~0.01м;",
                    U"дм~0.1м;",
                    U"км~1000м;",

                    //time
                    U"пс~0.000000000001сек;",
                    U"нс~0.000000001сек;",
                    U"мкс~0.000001сек;",
                    U"мс~0.001сек;",
                    U"мин~60сек;",
                    U"час~60мин;",
                    U"сутки~24час;",
                    U"неделя~7сутки;",

                    //mass
                    U"г~0.001кг;",
                    U"мкг~0.000001кг;",
                    U"мг~0.001кг;",
                    U"тонна~1000кг;",

                    //electric current
                    U"мкА~0.000001А;",
                    U"мА~0.001А;",
                    U"кА~1000А;",
                    U"МА~1000000А;",

                    //frequency
                    U"Гц~1/сек;",
                    U"кГц~1000Гц;",
                    U"МГц~1000кГц;",
                    U"ГГц~1000МГц;",

                    //force
                    U"Н~кг*м/pow(сек,2);",
                    U"мкН~0.000001Н;",
                    U"мН~0.001Н;",
                    U"кН~1000Н;",
                    U"МН~1000кН;",

                    //energy
                    U"Дж~Н*м;",
                    U"мкДж~0.000001Дж;",
                    U"мДж~0.001Дж;",
                    U"кДж~1000Дж;",
                    U"МДж~1000000Дж;",

                    //power
                    U"Вт~Дж/сек;",
                    U"мкВт~0.000001Вт;",
                    U"мВт~0.001Вт;",
                    U"кВт~1000Вт;",
                    U"МВт~1000000Вт;",

                    //pressure
                    U"Па~(Н)/pow(м,2);",
                    U"мкПа~0.000001Па;",
                    U"мПа~0.001Па;",
                    U"кПа~1000Па;",
                    U"МПа~1000000Па;",

                    //luminous intensity
                    U"мкКд~0.000001Кд;",
                    U"мКд~0.001Кд;",
                    U"кКд~1000Кд;",
                    U"МКд~1000000Кд;",

                    //luminous flux
                    U"лм~Кд*ср;",
                    U"мклм~0.000001лм;",
                    U"млм~0.001лм;",
                    U"клм~1000лм;",
                    U"Млм~1000000лм;",

                    //illuminance
                    U"лк~лм/pow(м,2);",
                    U"мклк~0.000001лк;",
                    U"млк~0.001лк;",
                    U"клк~1000лк;",
                    U"Млк~1000000лк;",

                    //electrical charge
                    U"Кл~А*сек;",
                    U"мкКл~0.000001Кл;",
                    U"мКл~0.001Кл;",
                    U"кКл~1000Кл;",
                    U"МКл~1000000Кл;",

                    //potential
                    U"В~Дж/Кл;",
                    U"пВ~0.000000000001В;",
                    U"нВ~0.000000001В;",
                    U"мкВ~0.000001В;",
                    U"мВ~0.001В;",
                    U"кВ~1000В;",
                    U"МВ~1000000В;",

                    //electrical resistance
                    U"Ом~В/А;",
                    U"мкОм~0.000001Ом;",
                    U"мОм~0.001Ом;",
                    U"кОм~1000Ом;",
                    U"МОм~1000000Ом;",

                    //electrical capacity
                    U"Ф~Кл/В;",
                    U"пФ~0.000000000001Ф;",
                    U"нФ~0.000000001Ф;",
                    U"мкФ~0.000001Ф;",
                    U"мФ~0.001Ф;",

                    //magnetic flux
                    U"Вб~(кг*pow(м,2)/(pow(сек,2)*А));",
                    U"мкВб~0.000001Вб;",
                    U"мВб~0.001Вб;",
                    U"кВб~1000Вб;",
                    U"МВб~1000000Вб;",

                    //magnetic field
                    U"Тл~(Вб)/(pow(м,2));",
                    U"мкТл~0.000001Тл;",
                    U"мТл~0.001Тл;",
                    U"кТл~1000Тл;",
                    U"МТл~1000000Тл;",

                    //electrical inductance
                    U"Гн~(кг*pow(м,2))/(pow(сек,2)*pow(А,2));",
                    U"пГн~0.000000000001Гн;",
                    U"нГн~0.000000001Гн;",
                    U"мкГн~0.000001Гн;",
                    U"мГн~0.001Гн;",

                    //electric conductance
                    U"См~(1)/(Ом);",
                    U"мкСм~0.000001См;",
                    U"мСм~0.001См;",
                    U"кСм~1000См;",
                    U"МСм~1000000См;",

                    //ionizing radiation
                    U"Гр~(Дж)/(кг);",
                    U"мкГр~0.000001Гр;",
                    U"мГр~0.001Гр;",
                    U"кГр~1000Гр;",
                    U"МГр~1000000Гр;",
                }
            }
        };

    std::map<Language, std::vector<std::u32string>> other_units = 
        {
            {
                Language::English,
                {
                    //temperature
                    U"°C~K;",
                    
                    //data
                    U"byte~8bit;",
                    U"kbyte~1024byte;",
                    U"Mbyte~1024kbyte;",
                    U"Gbyte~1024Mbyte;",
                    U"Tbyte~1024Gbyte;",

                    //square
                    U"a~100*pow(m,2);", //ar
                    U"ha~100a;", //hectar

                    //volume
                    U"l~pow(dm,3);", //litre
                    U"ml~0.001l;",
                    U"dl~0.1l;",
                    U"dal~10l;",
                    U"hl~100l;"
                }
            },
            {
                Language::Russian,
                {
                    //temperature
                    U"°C~К;",

                    //data
                    U"байт~8бит;",
                    U"кбайт~1024байт;",
                    U"Мбайт~1024кбайт;",
                    U"Гбайт~1024Мбайт;",
                    U"Тбайт~1024Гбайт;",

                    //square
                    U"ар~100*pow(м,2);", //ar
                    U"га~100ар;", //hectar

                    //volume
                    U"л~pow(дм,3);", //litre
                    U"мл~0.001л;",
                    U"дл~0.1л;",
                    U"дал~10л;",
                    U"гл~100л;"
                }
            }
        };

    std::map<Language, std::vector<std::u32string>> russian_units = 
        {
            {
                Language::English,
                {
                    //distance
                    U"tochka{rus}~(254)/(1000)mm;",
                    U"liniya{rus}~10tochka{rus};",
                    U"sotka{rus}~84tochka{rus};",
                    U"dyum{rus}~10liniya{rus};",
                    U"vershok{rus}~(7)/(4)dyum{rus};",
                    U"chetvert{rus}~7dyum{rus};",
                    U"fut{rus}~12dyum{rus};",
                    U"arshin{rus}~28dyum{rus};",
                    U"sazhen{rus}~7fut{rus};",
                    U"versta{rus}~500sazhen{rus};",

                    //mass
                    U"funt{rus}~0.4095124*kg;",
                    U"batman{rus}~10funt{rus};",
                    U"pud{rus}~40funt{rus};",
                    U"bezmen{rus}~(1)/(16pud{rus});",
                    U"kongar{rus}~40.95kg;",
                    U"pirog{rus}~43mg;",
                    U"zolotnik{rus}~4.266g;",
                    U"dolya{rus}~(1)/(96zolotnik{rus});",
                    U"pochka{rus}~(1)/(25zolotnik{rus});",
                    U"lot{rus}~zolotnik{rus};",

                    //time
                    U"s{rus}~s;",
                    U"min{rus}~min;",
                    U"hour{rus}~hour;",

                    //volume
                    U"stakan{rus}~0.273l;",
                    U"polygarnets{rus}~6stakan{rus};",
                    U"garnets{rus}~12stakan{rus};",
                    U"chetveric{rus}~8garnets{rus};",
                    U"osmina{rus}~4chetveric{rus};",
                    U"polosminy{rus}~52.48*l;",
                    U"chet{rus}~64garnets{rus};",
                    U"polovnik{rus}~419.84l;",
                    U"kadka{rus}~2polovnik{rus};",
                    U"shkalic{rus}~61.5ml;",
                    U"charka{rus}~2shkalic{rus};",
                    U"kosushka{rus}~5shkalic{rus};",
                    U"shtof{rus}~10charka{rus};",
                    U"vedro{rus}~4chet{rus};",
                    U"bochka{rus}~40vedro{rus};",
                }
            },
            {
                Language::Russian,
                {
                    //distance
                    U"точка{rus}~(254)/(1000)мм;",
                    U"линия{rus}~10точка{rus};",
                    U"сотка{rus}~84точка{rus};",
                    U"дюйм{rus}~10линия{rus};",
                    U"вершок{rus}~(7)/(4)дюйм{rus};",
                    U"четверть{rus}~7дюйм{rus};",
                    U"фут{rus}~12дюйм{rus};",
                    U"аршин{rus}~28дюйм{rus};",
                    U"сажень{rus}~7фут{rus};",
                    U"верста{rus}~500сажень{rus};",

                    //mass
                    U"фунт{rus}~0.4095124*кг;",
                    U"батман{rus}~10фунт{rus};",
                    U"пуд{rus}~40фунт{rus};",
                    U"безмен{rus}~(1)/(16пуд{rus});",
                    U"конгарь{rus}~40.95кг;",
                    U"пирог{rus}~43мг;",
                    U"золотник{rus}~4.266г;",
                    U"доля{rus}~(1)/(96золотник{rus});",
                    U"почка{rus}~(1)/(25золотник{rus});",
                    U"лот{rus}~золотник{rus};",

                    //time
                    U"сек{rus}~сек;",
                    U"мин{rus}~мин;",
                    U"час{rus}~час;",

                    //volume
                    U"стакан{rus}~0.273л;",
                    U"полигарнец{rus}~6стакан{rus};",
                    U"гарнец{rus}~12стакан{rus};",
                    U"четверик{rus}~8гарнец{rus};",
                    U"осмина{rus}~4четверик{rus};",
                    U"полосмины{rus}~52.48л;",
                    U"чет{rus}~64гарнец{rus};",
                    U"половник{rus}~419.84л;",
                    U"кадка{rus}~2половник{rus};",
                    U"шкалик{rus}~61.5мл;",
                    U"чарка{rus}~2шкалик{rus};",
                    U"косушка{rus}~5шкалик{rus};",
                    U"штоф{rus}~10чарка{rus};",
                    U"ведро{rus}~4чет{rus};",
                    U"бочка{rus}~40ведро{rus};",
                }
            }
        };

    std::map<Language, std::vector<std::u32string>> phisical_constants = 
        {
            {
                Language::English,
                {
                    U"c`speed of light`=299792458(m/s);",
                    U"G`gravitational constant`=6.674301515151515*pow(10,-11)*(pow(m,3)/(kg*pow(s,2)));",
                    U"h`Planck constant`=6.62607015*pow(10,-34)*(J*s);",
                    U"e_c`elementary charge`=1.602176634*pow(10,-19)*C;",
                    U"k`Boltzmann constant`=1.380649*pow(10,-23)*(J/K);"
                }
            },
            {
                Language::Russian,
                {
                    U"с`скорость света`=299792458(м/сек);",
                    U"G`гравитационная постоянная`=6.674301515151515*pow(10,-11)*(pow(м,3)/(кг*pow(сек,2)));"
                    U"h`постоянная Планка`=6.62607015*pow(10,-34)*(Дж*сек);",
                    U"e_c`элементарный заряд`=1.602176634*pow(10,-19)*Кл;",
                    U"k`постоянная Больцмана`=1.380649*pow(10,-23)*(Дж/К);"
                }
            }
        };
};

};

#endif
