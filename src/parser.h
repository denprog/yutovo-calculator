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
    Parser(const int precision, const Language _language, const char _decimal_point);
    
    Number Parse(ElementId id, std::u32string expression, Dependencies& dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
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
        solver.SetDependencies(&dependencies);
        return solver(script_node, id, default_angle_measure, result_angle_measure, precision);
    }

    Number Parse(ElementId id, std::u32string expression, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, const int precision = -1)
    {
        Dependencies dependencies;
        return Parse(id, expression, dependencies, default_angle_measure, result_angle_measure, Notation::Decimal, precision);
    }

    Number Parse(ElementId id, std::string expression, Dependencies& dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
        const int precision = -1)
    {
        return Parse(id, ToUtfString(expression), dependencies, default_angle_measure, result_angle_measure, Notation::Decimal, precision);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies& dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
        const int precision = -1)
    {
        return Parse(id, expression, dependencies, default_angle_measure, result_angle_measure, Notation::Decimal, precision);
    }

    Number Parse(ElementId id, std::u32string expression)
    {
        Dependencies dependencies;
        return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None, Notation::Decimal);
    }

    Number Parse(ElementId id, std::u32string expression, const int precision)
    {
        Dependencies dependencies;
        return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None, Notation::Decimal, precision);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies& dependencies)
    {
        return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None, Notation::Decimal);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies& dependencies, Notation default_notation)
    {
        return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None, default_notation);
    }

    Number Parse(ElementId id, std::string expression, Dependencies& dependencies, Notation default_notation)
    {
        return Parse(id, ToUtfString(expression), dependencies, AngleMeasure::Radian, AngleMeasure::None, default_notation);
    }

    Number Parse(ElementId id, std::string expression, Dependencies& dependencies)
    {
        return Parse(id, ToUtfString(expression), dependencies, AngleMeasure::Radian, AngleMeasure::None);
    }

    Number Parse(ElementId id, std::u32string expression, Notation default_notation)
    {
        Dependencies dependencies;
        return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None, default_notation);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies& dependencies, 
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
        solver.SetDependencies(&dependencies);

        for (int i = 0; i < res_count; ++i)
        {
            solver.res_pos = i;
            Number r = solver(script_node, id, default_angle_measure, result_angle_measure, precision);
            if (std::find(results.begin(), results.end(), r) != results.end())
                break;
            results.push_back(r);
        }
        if (results.empty())
            return Number();
        return results[0];
    }

    Number Parse(ElementId id, std::string expression, Dependencies& dependencies, 
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

    void SetLocale(Language _language, char _decimal_point);

    void ListBuiltinVariables(std::vector<std::u32string>& variables)
    {
        solver.ListBuiltinVariables(variables);
    }

    void ListBuiltinFunctions(std::vector<std::u32string>& functions)
    {
        solver.ListBuiltinFunctions(functions);
    }

    void ListUserVariables(std::vector<std::u32string>& variables)
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

private:
    void InitUnits()
    {
        solver.ResetUnits();

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
        switch (language)
        {
        case Language::English:
            //distance
            Parse(ElementId{0, 0, 0, 1}, U"ang~0.000000000001m;");
            Parse(ElementId{0, 0, 0, 2}, U"nm~0.000000001m;");
            Parse(ElementId{0, 0, 0, 3}, U"mcm~0.000001m;");
            Parse(ElementId{0, 0, 0, 4}, U"mm~0.001m;");
            Parse(ElementId{0, 0, 0, 5}, U"cm~0.01m;");
            Parse(ElementId{0, 0, 0, 6}, U"dm~0.1m;");
            Parse(ElementId{0, 0, 0, 7}, U"km~1000m;");

            //time
            Parse(ElementId{0, 0, 1, 1}, U"ps~0.000000000001s;");
            Parse(ElementId{0, 0, 1, 2}, U"ns~0.000000001s;");
            Parse(ElementId{0, 0, 1, 3}, U"mcs~0.000001s;");
            Parse(ElementId{0, 0, 1, 4}, U"ms~0.001s;");
            Parse(ElementId{0, 0, 1, 5}, U"min~60s;");
            Parse(ElementId{0, 0, 1, 6}, U"hour~60min;");
            Parse(ElementId{0, 0, 1, 7}, U"day~24hour;");
            Parse(ElementId{0, 0, 1, 8}, U"week~7day;");

            //mass
            Parse(ElementId{0, 0, 2, 1}, U"g~0.001kg;");
            Parse(ElementId{0, 0, 2, 2}, U"mcg~0.000001kg;");
            Parse(ElementId{0, 0, 2, 3}, U"mg~0.001kg;");
            Parse(ElementId{0, 0, 2, 4}, U"ton~1000kg;");

            //electric current
            Parse(ElementId{0, 0, 3, 1}, U"mcA~0.000001A;");
            Parse(ElementId{0, 0, 3, 2}, U"mA~0.001A;");
            Parse(ElementId{0, 0, 3, 3}, U"kA~1000A;");
            Parse(ElementId{0, 0, 3, 4}, U"MA~1000000A;");

            //frequency
            Parse(ElementId{0, 0, 4, 1}, U"Hz~1/s;");
            Parse(ElementId{0, 0, 4, 2}, U"kHz~1000Hz;");
            Parse(ElementId{0, 0, 4, 3}, U"MHz~1000kHz;");
            Parse(ElementId{0, 0, 4, 4}, U"GHz~1000MHz;");

            //force
            Parse(ElementId{0, 0, 5, 1}, U"N~(kg*m)/(pow(s,2));");
            Parse(ElementId{0, 0, 5, 2}, U"mcN~0.000001N;");
            Parse(ElementId{0, 0, 5, 3}, U"mN~0.001N;");
            Parse(ElementId{0, 0, 5, 4}, U"kN~1000N;");
            Parse(ElementId{0, 0, 5, 5}, U"MN~1000kN;");

            //energy
            Parse(ElementId{0, 0, 6, 1}, U"J~N*m;");
            Parse(ElementId{0, 0, 6, 2}, U"mcJ~0.000001J;");
            Parse(ElementId{0, 0, 6, 3}, U"mJ~0.001J;");
            Parse(ElementId{0, 0, 6, 4}, U"kJ~1000J;");
            Parse(ElementId{0, 0, 6, 5}, U"MJ~1000000J;");

            //power
            Parse(ElementId{0, 0, 7, 1}, U"W~(J)/(s);");
            Parse(ElementId{0, 0, 7, 2}, U"mcW~0.000001W;");
            Parse(ElementId{0, 0, 7, 3}, U"mW~0.001W;");
            Parse(ElementId{0, 0, 7, 4}, U"kW~1000W;");
            Parse(ElementId{0, 0, 7, 5}, U"MW~1000000W;");

            //pressure
            Parse(ElementId{0, 0, 8, 1}, U"Pa~(N)/(pow(m,2));");
            Parse(ElementId{0, 0, 8, 2}, U"mcPa~0.000001Pa;");
            Parse(ElementId{0, 0, 8, 3}, U"mPa~0.001Pa;");
            Parse(ElementId{0, 0, 8, 4}, U"kPa~1000Pa;");
            Parse(ElementId{0, 0, 8, 5}, U"MPa~1000000Pa;");

            //luminous intensity
            Parse(ElementId{0, 0, 9, 1}, U"mccd~0.000001cd;");
            Parse(ElementId{0, 0, 9, 2}, U"mcd~0.001cd;");
            Parse(ElementId{0, 0, 9, 3}, U"kcd~1000cd;");
            Parse(ElementId{0, 0, 9, 4}, U"Mcd~1000000cd;");

            //luminous flux
            Parse(ElementId{0, 0, 10, 1}, U"lm~cd*sr;");
            Parse(ElementId{0, 0, 10, 2}, U"mclm~0.000001lm;");
            Parse(ElementId{0, 0, 10, 3}, U"mlm~0.001lm;");
            Parse(ElementId{0, 0, 10, 4}, U"klm~1000lm;");
            Parse(ElementId{0, 0, 10, 5}, U"Mlm~1000000lm;");

            //illuminance
            Parse(ElementId{0, 0, 11, 1}, U"lx~lm/pow(m,2);");
            Parse(ElementId{0, 0, 11, 2}, U"mclx~0.000001lx;");
            Parse(ElementId{0, 0, 11, 3}, U"mlx~0.001lx;");
            Parse(ElementId{0, 0, 11, 4}, U"klx~1000lx;");
            Parse(ElementId{0, 0, 11, 5}, U"Mlx~1000000lx;");

            //electrical charge
            Parse(ElementId{0, 0, 12, 1}, U"C~A*s;");
            Parse(ElementId{0, 0, 12, 2}, U"mcC~0.000001C;");
            Parse(ElementId{0, 0, 12, 3}, U"mC~0.001C;");
            Parse(ElementId{0, 0, 12, 4}, U"kC~1000C;");
            Parse(ElementId{0, 0, 12, 5}, U"MC~1000000C;");

            //potential
            Parse(ElementId{0, 0, 13, 1}, U"V~(J)/(C);");
            Parse(ElementId{0, 0, 13, 2}, U"pV~0.000000000001V;");
            Parse(ElementId{0, 0, 13, 3}, U"nV~0.000000001V;");
            Parse(ElementId{0, 0, 13, 4}, U"mcV~0.000001V;");
            Parse(ElementId{0, 0, 13, 5}, U"mV~0.001V;");
            Parse(ElementId{0, 0, 13, 6}, U"kV~1000V;");
            Parse(ElementId{0, 0, 13, 7}, U"MV~1000000V;");

            //electrical resistance
            Parse(ElementId{0, 0, 14, 1}, U"Ohm~V/A;");
            Parse(ElementId{0, 0, 14, 2}, U"mcOhm~0.000001Ohm;");
            Parse(ElementId{0, 0, 14, 3}, U"mOhm~0.001Ohm;");
            Parse(ElementId{0, 0, 14, 4}, U"kOhm~1000Ohm;");
            Parse(ElementId{0, 0, 14, 5}, U"MOhm~1000000Ohm;");

            //electrical capacity
            Parse(ElementId{0, 0, 15, 1}, U"F~C/V;");
            Parse(ElementId{0, 0, 15, 2}, U"pF~0.000000000001F;");
            Parse(ElementId{0, 0, 15, 3}, U"nF~0.000000001F;");
            Parse(ElementId{0, 0, 15, 4}, U"mcF~0.000001F;");
            Parse(ElementId{0, 0, 15, 5}, U"mF~0.001F;");

            //magnetic flux
            Parse(ElementId{0, 0, 16, 1}, U"Wb~(kg*pow(m,2)/(pow(s,2)*A));");
            Parse(ElementId{0, 0, 16, 2}, U"mcWb~0.000001Wb;");
            Parse(ElementId{0, 0, 16, 3}, U"mWb~0.001Wb;");
            Parse(ElementId{0, 0, 16, 4}, U"kWb~1000Wb;");
            Parse(ElementId{0, 0, 16, 5}, U"MWb~1000000Wb;");

            //magnetic field
            Parse(ElementId{0, 0, 17, 1}, U"T~(Wb)/(pow(m,2));");
            Parse(ElementId{0, 0, 17, 2}, U"mcT~0.000001T;");
            Parse(ElementId{0, 0, 17, 3}, U"mT~0.001T;");
            Parse(ElementId{0, 0, 17, 4}, U"kT~1000T;");
            Parse(ElementId{0, 0, 17, 5}, U"MT~1000000T;");

            //electrical inductance
            Parse(ElementId{0, 0, 18, 1}, U"H~(kg*pow(m,2))/(pow(s,2)*pow(A,2));");
            Parse(ElementId{0, 0, 18, 2}, U"pH~0.000000000001H;");
            Parse(ElementId{0, 0, 18, 3}, U"nH~0.000000001H;");
            Parse(ElementId{0, 0, 18, 4}, U"mcH~0.000001H;");
            Parse(ElementId{0, 0, 18, 5}, U"mH~0.001H;");

            //electric conductance
            Parse(ElementId{0, 0, 19, 1}, U"S~(1)/(Ohm);");
            Parse(ElementId{0, 0, 19, 2}, U"mcS~0.000001Ohm;");
            Parse(ElementId{0, 0, 19, 3}, U"mS~0.001Ohm;");
            Parse(ElementId{0, 0, 19, 4}, U"kS~1000Ohm;");
            Parse(ElementId{0, 0, 19, 5}, U"MS~1000000Ohm;");

            //ionizing radiation
            Parse(ElementId{0, 0, 20, 1}, U"Gy~(J)/(kg);");
            Parse(ElementId{0, 0, 20, 2}, U"mcGy~0.000001Gy;");
            Parse(ElementId{0, 0, 20, 3}, U"mGy~0.001Gy;");
            Parse(ElementId{0, 0, 20, 4}, U"kGy~1000Gy;");
            Parse(ElementId{0, 0, 20, 5}, U"MGy~1000000Gy;");
            break;
        case Language::Russian:
            //distance
            Parse(ElementId{0, 0, 0, 1}, U"анг~0.0000000001м;");
            Parse(ElementId{0, 0, 0, 2}, U"нм~0.000000001м;");
            Parse(ElementId{0, 0, 0, 3}, U"мкм~0.000001м;");
            Parse(ElementId{0, 0, 0, 4}, U"мм~0.001м;");
            Parse(ElementId{0, 0, 0, 5}, U"см~0.01м;");
            Parse(ElementId{0, 0, 0, 6}, U"дм~0.1м;");
            Parse(ElementId{0, 0, 0, 7}, U"км~1000м;");

            //time
            Parse(ElementId{0, 0, 1, 1}, U"пс~0.000000000001сек;");
            Parse(ElementId{0, 0, 1, 2}, U"нс~0.000000001сек;");
            Parse(ElementId{0, 0, 1, 3}, U"мкс~0.000001сек;");
            Parse(ElementId{0, 0, 1, 4}, U"мс~0.001сек;");
            Parse(ElementId{0, 0, 1, 5}, U"мин~60сек;");
            Parse(ElementId{0, 0, 1, 6}, U"час~60мин;");
            Parse(ElementId{0, 0, 1, 7}, U"сутки~24час;");
            Parse(ElementId{0, 0, 1, 8}, U"неделя~7сутки;");

            //mass
            Parse(ElementId{0, 0, 2, 1}, U"г~0.001кг;");
            Parse(ElementId{0, 0, 2, 2}, U"мкг~0.000001кг;");
            Parse(ElementId{0, 0, 2, 3}, U"мг~0.001кг;");
            Parse(ElementId{0, 0, 2, 4}, U"тонна~1000кг;");

            //electric current
            Parse(ElementId{0, 0, 3, 1}, U"мкА~0.000001А;");
            Parse(ElementId{0, 0, 3, 2}, U"мА~0.001А;");
            Parse(ElementId{0, 0, 3, 3}, U"кА~1000А;");
            Parse(ElementId{0, 0, 3, 4}, U"МА~1000000А;");

            //frequency
            Parse(ElementId{0, 0, 4, 1}, U"Гц~1/сек;");
            Parse(ElementId{0, 0, 4, 2}, U"кГц~1000Гц;");
            Parse(ElementId{0, 0, 4, 3}, U"МГц~1000кГц;");
            Parse(ElementId{0, 0, 4, 4}, U"ГГц~1000МГц;");

            //force
            Parse(ElementId{0, 0, 5, 1}, U"Н~кг*м/pow(сек,2);");
            Parse(ElementId{0, 0, 5, 2}, U"мкН~0.000001Н;");
            Parse(ElementId{0, 0, 5, 3}, U"мН~0.001Н;");
            Parse(ElementId{0, 0, 5, 4}, U"кН~1000Н;");
            Parse(ElementId{0, 0, 5, 5}, U"МН~1000кН;");

            //energy
            Parse(ElementId{0, 0, 6, 1}, U"Дж~Н*м;");
            Parse(ElementId{0, 0, 6, 2}, U"мкДж~0.000001Дж;");
            Parse(ElementId{0, 0, 6, 3}, U"мДж~0.001Дж;");
            Parse(ElementId{0, 0, 6, 4}, U"кДж~1000Дж;");
            Parse(ElementId{0, 0, 6, 5}, U"МДж~1000000Дж;");

            //power
            Parse(ElementId{0, 0, 7, 1}, U"Вт~Дж/сек;");
            Parse(ElementId{0, 0, 7, 2}, U"мкВт~0.000001Вт;");
            Parse(ElementId{0, 0, 7, 3}, U"мВт~0.001Вт;");
            Parse(ElementId{0, 0, 7, 4}, U"кВт~1000Вт;");
            Parse(ElementId{0, 0, 7, 5}, U"МВт~1000000Вт;");

            //pressure
            Parse(ElementId{0, 0, 8, 1}, U"Па~(Н)/pow(м,2);");
            Parse(ElementId{0, 0, 8, 2}, U"мкПа~0.000001Па;");
            Parse(ElementId{0, 0, 8, 3}, U"мПа~0.001Па;");
            Parse(ElementId{0, 0, 8, 4}, U"кПа~1000Па;");
            Parse(ElementId{0, 0, 8, 5}, U"МПа~1000000Па;");

            //luminous intensity
            Parse(ElementId{0, 0, 9, 1}, U"мкКд~0.000001Кд;");
            Parse(ElementId{0, 0, 9, 2}, U"мКд~0.001Кд;");
            Parse(ElementId{0, 0, 9, 3}, U"кКд~1000Кд;");
            Parse(ElementId{0, 0, 9, 4}, U"МКд~1000000Кд;");

            //luminous flux
            Parse(ElementId{0, 0, 10, 1}, U"лм~Кд*ср;");
            Parse(ElementId{0, 0, 10, 2}, U"мклм~0.000001лм;");
            Parse(ElementId{0, 0, 10, 3}, U"млм~0.001лм;");
            Parse(ElementId{0, 0, 10, 4}, U"клм~1000лм;");
            Parse(ElementId{0, 0, 10, 5}, U"Млм~1000000лм;");

            //illuminance
            Parse(ElementId{0, 0, 11, 1}, U"лк~лм/pow(м,2);");
            Parse(ElementId{0, 0, 11, 2}, U"мклк~0.000001лк;");
            Parse(ElementId{0, 0, 11, 3}, U"млк~0.001лк;");
            Parse(ElementId{0, 0, 11, 4}, U"клк~1000лк;");
            Parse(ElementId{0, 0, 11, 5}, U"Млк~1000000лк;");

            //electrical charge
            Parse(ElementId{0, 0, 12, 1}, U"Кл~А*сек;");
            Parse(ElementId{0, 0, 12, 2}, U"мкКл~0.000001Кл;");
            Parse(ElementId{0, 0, 12, 3}, U"мКл~0.001Кл;");
            Parse(ElementId{0, 0, 12, 4}, U"кКл~1000Кл;");
            Parse(ElementId{0, 0, 12, 5}, U"МКл~1000000Кл;");

            //potential
            Parse(ElementId{0, 0, 13, 1}, U"В~Дж/Кл;");
            Parse(ElementId{0, 0, 13, 2}, U"пВ~0.000000000001В;");
            Parse(ElementId{0, 0, 13, 3}, U"нВ~0.000000001В;");
            Parse(ElementId{0, 0, 13, 4}, U"мкВ~0.000001В;");
            Parse(ElementId{0, 0, 13, 5}, U"мВ~0.001В;");
            Parse(ElementId{0, 0, 13, 6}, U"кВ~1000В;");
            Parse(ElementId{0, 0, 13, 7}, U"МВ~1000000В;");

            //electrical resistance
            Parse(ElementId{0, 0, 14, 1}, U"Ом~В/А;");
            Parse(ElementId{0, 0, 14, 2}, U"мкОм~0.000001Ом;");
            Parse(ElementId{0, 0, 14, 3}, U"мОм~0.001Ом;");
            Parse(ElementId{0, 0, 14, 4}, U"кОм~1000Ом;");
            Parse(ElementId{0, 0, 14, 5}, U"МОм~1000000Ом;");

            //electrical capacity
            Parse(ElementId{0, 0, 15, 1}, U"Ф~Кл/В;");
            Parse(ElementId{0, 0, 15, 2}, U"пФ~0.000000000001Ф;");
            Parse(ElementId{0, 0, 15, 3}, U"нФ~0.000000001Ф;");
            Parse(ElementId{0, 0, 15, 4}, U"мкФ~0.000001Ф;");
            Parse(ElementId{0, 0, 15, 5}, U"мФ~0.001Ф;");

            //magnetic flux
            Parse(ElementId{0, 0, 16, 1}, U"Вб~(кг*pow(м,2)/(pow(сек,2)*А));");
            Parse(ElementId{0, 0, 16, 2}, U"мкВб~0.000001Вб;");
            Parse(ElementId{0, 0, 16, 3}, U"мВб~0.001Вб;");
            Parse(ElementId{0, 0, 16, 4}, U"кВб~1000Вб;");
            Parse(ElementId{0, 0, 16, 5}, U"МВб~1000000Вб;");

            //magnetic field
            Parse(ElementId{0, 0, 17, 1}, U"Тл~(Вб)/(pow(м,2));");
            Parse(ElementId{0, 0, 17, 2}, U"мкТл~0.000001Тл;");
            Parse(ElementId{0, 0, 17, 3}, U"мТл~0.001Тл;");
            Parse(ElementId{0, 0, 17, 4}, U"кТл~1000Тл;");
            Parse(ElementId{0, 0, 17, 5}, U"МТл~1000000Тл;");

            //electrical inductance
            Parse(ElementId{0, 0, 18, 1}, U"Гн~(кг*pow(м,2))/(pow(сек,2)*pow(А,2));");
            Parse(ElementId{0, 0, 18, 2}, U"пГн~0.000000000001Гн;");
            Parse(ElementId{0, 0, 18, 3}, U"нГн~0.000000001Гн;");
            Parse(ElementId{0, 0, 18, 4}, U"мкГн~0.000001Гн;");
            Parse(ElementId{0, 0, 18, 5}, U"мГн~0.001Гн;");

            //electric conductance
            Parse(ElementId{0, 0, 19, 1}, U"См~(1)/(Ом);");
            Parse(ElementId{0, 0, 19, 2}, U"мкСм~0.000001См;");
            Parse(ElementId{0, 0, 19, 3}, U"мСм~0.001См;");
            Parse(ElementId{0, 0, 19, 4}, U"кСм~1000См;");
            Parse(ElementId{0, 0, 19, 5}, U"МСм~1000000См;");

            //ionizing radiation
            Parse(ElementId{0, 0, 20, 1}, U"Гр~(Дж)/(кг);");
            Parse(ElementId{0, 0, 20, 2}, U"мкГр~0.000001Гр;");
            Parse(ElementId{0, 0, 20, 3}, U"мГр~0.001Гр;");
            Parse(ElementId{0, 0, 20, 4}, U"кГр~1000Гр;");
            Parse(ElementId{0, 0, 20, 5}, U"МГр~1000000Гр;");
            break;
        }
    }

    void InitOtherUnits()
    {
        switch (language)
        {
        case Language::English:
            //temperature
            Parse(ElementId{0, 1, 0, 0, 1}, U"°C~K;");
            
            //data
            Parse(ElementId{0, 1, 0, 1}, U"byte~8bit;");
            Parse(ElementId{0, 1, 0, 2}, U"kbyte~1024byte;");
            Parse(ElementId{0, 1, 0, 3}, U"Mbyte~1024kbyte;");
            Parse(ElementId{0, 1, 0, 4}, U"Gbyte~1024Mbyte;");
            Parse(ElementId{0, 1, 0, 5}, U"Tbyte~1024Gbyte;");

            //square
            Parse(ElementId{0, 1, 1, 1}, U"a~100*pow(m,2);"); //ar
            Parse(ElementId{0, 1, 1, 2}, U"ha~100a;"); //hectar

            //volume
            Parse(ElementId{0, 1, 2, 1}, U"l~pow(dm,3);"); //litre
            Parse(ElementId{0, 1, 2, 2}, U"ml~0.001l;");
            Parse(ElementId{0, 1, 2, 3}, U"dl~0.1l;");
            Parse(ElementId{0, 1, 2, 4}, U"dal~10l;");
            Parse(ElementId{0, 1, 2, 5}, U"hl~100l;");
            break;
        case Language::Russian:
            //temperature
            Parse(ElementId{0, 1, 0, 0, 1}, U"°C~К;");

            //data
            Parse(ElementId{0, 1, 0, 1}, U"байт~8бит;");
            Parse(ElementId{0, 1, 0, 2}, U"кбайт~1024байт;");
            Parse(ElementId{0, 1, 0, 3}, U"Мбайт~1024кбайт;");
            Parse(ElementId{0, 1, 0, 4}, U"Гбайт~1024Мбайт;");
            Parse(ElementId{0, 1, 0, 5}, U"Тбайт~1024Гбайт;");

            //square
            Parse(ElementId{0, 1, 1, 1}, U"ар~100*pow(м,2);"); //ar
            Parse(ElementId{0, 1, 1, 2}, U"га~100ар;"); //hectar

            //volume
            Parse(ElementId{0, 1, 2, 1}, U"л~pow(дм,3);"); //litre
            Parse(ElementId{0, 1, 2, 2}, U"мл~0.001л;");
            Parse(ElementId{0, 1, 2, 3}, U"дл~0.1л;");
            Parse(ElementId{0, 1, 2, 4}, U"дал~10л;");
            Parse(ElementId{0, 1, 2, 5}, U"гл~100л;");
            break;
        }
    }

    void InitRussianUnits()
    {
        switch (language)
        {
        case Language::English:
            //distance
            Parse(ElementId{0, 2, 0, 0}, U"tochka{rus}~(254)/(1000)mm;");
            Parse(ElementId{0, 2, 0, 1}, U"liniya{rus}~10tochka{rus};");
            Parse(ElementId{0, 2, 0, 2}, U"sotka{rus}~84tochka{rus};");
            Parse(ElementId{0, 2, 0, 3}, U"dyum{rus}~10liniya{rus};");
            Parse(ElementId{0, 2, 0, 4}, U"vershok{rus}~(7)/(4)dyum{rus};");
            Parse(ElementId{0, 2, 0, 5}, U"chetvert{rus}~7dyum{rus};");
            Parse(ElementId{0, 2, 0, 6}, U"fut{rus}~12dyum{rus};");
            Parse(ElementId{0, 2, 0, 7}, U"arshin{rus}~28dyum{rus};");
            Parse(ElementId{0, 2, 0, 8}, U"sazhen{rus}~7fut{rus};");
            Parse(ElementId{0, 2, 0, 9}, U"versta{rus}~500sazhen{rus};");

            //mass
            Parse(ElementId{0, 2, 1, 1}, U"funt{rus}~0.4095124*kg;");
            Parse(ElementId{0, 2, 1, 2}, U"batman{rus}~10funt{rus};");
            Parse(ElementId{0, 2, 1, 3}, U"pud{rus}~40funt{rus};");
            Parse(ElementId{0, 2, 1, 4}, U"bezmen{rus}~(1)/(16pud{rus});");
            Parse(ElementId{0, 2, 1, 5}, U"kongar{rus}~40.95kg;");
            Parse(ElementId{0, 2, 1, 6}, U"pirog{rus}~43mg;");
            Parse(ElementId{0, 2, 1, 7}, U"zolotnik{rus}~4.266g;");
            Parse(ElementId{0, 2, 1, 8}, U"dolya{rus}~(1)/(96zolotnik{rus});");
            Parse(ElementId{0, 2, 1, 9}, U"pochka{rus}~(1)/(25zolotnik{rus});");
            Parse(ElementId{0, 2, 1, 10}, U"lot{rus}~zolotnik{rus};");

            //time
            Parse(ElementId{0, 2, 2, 1}, U"s{rus}~s;");
            Parse(ElementId{0, 2, 2, 2}, U"min{rus}~min;");
            Parse(ElementId{0, 2, 2, 3}, U"hour{rus}~hour;");

            //volume
            Parse(ElementId{0, 2, 3, 1}, U"stakan{rus}~0.273l;");
            Parse(ElementId{0, 2, 3, 2}, U"polygarnets{rus}~6stakan{rus};");
            Parse(ElementId{0, 2, 3, 3}, U"garnets{rus}~12stakan{rus};");
            Parse(ElementId{0, 2, 3, 4}, U"chetveric{rus}~8garnets{rus};");
            Parse(ElementId{0, 2, 3, 5}, U"osmina{rus}~4chetveric{rus};");
            Parse(ElementId{0, 2, 3, 6}, U"polosminy{rus}~52.48*l;");
            Parse(ElementId{0, 2, 3, 7}, U"chet{rus}~64garnets{rus};");
            Parse(ElementId{0, 2, 3, 8}, U"polovnik{rus}~419.84l;");
            Parse(ElementId{0, 2, 3, 9}, U"kadka{rus}~2polovnik{rus};");
            Parse(ElementId{0, 2, 3, 10}, U"shkalic{rus}~61.5ml;");
            Parse(ElementId{0, 2, 3, 11}, U"charka{rus}~2shkalic{rus};");
            Parse(ElementId{0, 2, 3, 12}, U"kosushka{rus}~5shkalic{rus};");
            Parse(ElementId{0, 2, 3, 13}, U"shtof{rus}~10charka{rus};");
            Parse(ElementId{0, 2, 3, 14}, U"vedro{rus}~4chet{rus};");
            Parse(ElementId{0, 2, 3, 15}, U"bochka{rus}~40vedro{rus};");
            break;
        case Language::Russian:
            //distance
            Parse(ElementId{0, 2, 0, 0}, U"точка{rus}~(254)/(1000)мм;");
            Parse(ElementId{0, 2, 0, 1}, U"линия{rus}~10точка{rus};");
            Parse(ElementId{0, 2, 0, 2}, U"сотка{rus}~84точка{rus};");
            Parse(ElementId{0, 2, 0, 3}, U"дюйм{rus}~10линия{rus};");
            Parse(ElementId{0, 2, 0, 4}, U"вершок{rus}~(7)/(4)дюйм{rus};");
            Parse(ElementId{0, 2, 0, 5}, U"четверть{rus}~7дюйм{rus};");
            Parse(ElementId{0, 2, 0, 6}, U"фут{rus}~12дюйм{rus};");
            Parse(ElementId{0, 2, 0, 7}, U"аршин{rus}~28дюйм{rus};");
            Parse(ElementId{0, 2, 0, 8}, U"сажень{rus}~7фут{rus};");
            Parse(ElementId{0, 2, 0, 9}, U"верста{rus}~500сажень{rus};");

            //mass
            Parse(ElementId{0, 2, 1, 1}, U"фунт{rus}~0.4095124*кг;");
            Parse(ElementId{0, 2, 1, 2}, U"батман{rus}~10фунт{rus};");
            Parse(ElementId{0, 2, 1, 3}, U"пуд{rus}~40фунт{rus};");
            Parse(ElementId{0, 2, 1, 4}, U"безмен{rus}~(1)/(16пуд{rus});");
            Parse(ElementId{0, 2, 1, 5}, U"конгарь{rus}~40.95кг;");
            Parse(ElementId{0, 2, 1, 6}, U"пирог{rus}~43мг;");
            Parse(ElementId{0, 2, 1, 7}, U"золотник{rus}~4.266г;");
            Parse(ElementId{0, 2, 1, 8}, U"доля{rus}~(1)/(96золотник{rus});");
            Parse(ElementId{0, 2, 1, 9}, U"почка{rus}~(1)/(25золотник{rus});");
            Parse(ElementId{0, 2, 1, 10}, U"лот{rus}~золотник{rus};");

            //time
            Parse(ElementId{0, 2, 2, 1}, U"сек{rus}~сек;");
            Parse(ElementId{0, 2, 2, 2}, U"мин{rus}~мин;");
            Parse(ElementId{0, 2, 2, 3}, U"час{rus}~час;");

            //volume
            Parse(ElementId{0, 2, 3, 1}, U"стакан{rus}~0.273л;");
            Parse(ElementId{0, 2, 3, 2}, U"полигарнец{rus}~6стакан{rus};");
            Parse(ElementId{0, 2, 3, 3}, U"гарнец{rus}~12стакан{rus};");
            Parse(ElementId{0, 2, 3, 4}, U"четверик{rus}~8гарнец{rus};");
            Parse(ElementId{0, 2, 3, 5}, U"осмина{rus}~4четверик{rus};");
            Parse(ElementId{0, 2, 3, 6}, U"полосмины{rus}~52.48л;");
            Parse(ElementId{0, 2, 3, 7}, U"чет{rus}~64гарнец{rus};");
            Parse(ElementId{0, 2, 3, 8}, U"половник{rus}~419.84л;");
            Parse(ElementId{0, 2, 3, 9}, U"кадка{rus}~2половник{rus};");
            Parse(ElementId{0, 2, 3, 10}, U"шкалик{rus}~61.5мл;");
            Parse(ElementId{0, 2, 3, 11}, U"чарка{rus}~2шкалик{rus};");
            Parse(ElementId{0, 2, 3, 12}, U"косушка{rus}~5шкалик{rus};");
            Parse(ElementId{0, 2, 3, 13}, U"штоф{rus}~10чарка{rus};");
            Parse(ElementId{0, 2, 3, 14}, U"ведро{rus}~4чет{rus};");
            Parse(ElementId{0, 2, 3, 15}, U"бочка{rus}~40ведро{rus};");
            break;
        }
    }

    void InitAngleUnits()
    {
    }

    Solver<Number> solver;

    Language language = Language::English;
    char decimal_point;
};

};

#endif
