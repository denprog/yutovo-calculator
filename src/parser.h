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
        Notation default_notation, const int precision = -1, ParserContext* _context = nullptr)
    {
        if (expression.empty() || expression == U";")
            throw SyntaxException(id, ExpressionExpected, 0, 0);

        context = _context;
        if (context)
            context->break_solving = false;
        
        std::u32string::iterator iter = expression.begin();
        std::u32string::iterator end = expression.end();
        unicode::space_type space;

        solver.ClearTempVariables();
        solver.ClearCastUnits();
        solver.SetDefaultNotation(default_notation);
        solver.parser_context = context;

        Script<Number> script(id, expression, &solver);
        ScriptNode<Number> script_node;

        phrase_parse(iter, end, script, space, script_node);
        return solver(script_node, id, default_angle_measure, result_angle_measure, precision, dependencies);
    }

    Number Parse(ElementId id, std::u32string expression, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, const int precision = -1, 
        ParserContext* _context = nullptr)
    {
        Dependencies dependencies;
        return Parse(id, expression, &dependencies, default_angle_measure, result_angle_measure, Notation::Decimal, precision, _context);
    }

    Number Parse(ElementId id, std::string expression, Dependencies* dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
        const int precision = -1, ParserContext* _context = nullptr)
    {
        return Parse(id, ToUtfString(expression), dependencies, default_angle_measure, result_angle_measure, Notation::Decimal, precision, _context);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies* dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
        const int precision = -1, ParserContext* _context = nullptr)
    {
        return Parse(id, expression, dependencies, default_angle_measure, result_angle_measure, Notation::Decimal, precision, _context);
    }

    Number Parse(ElementId id, std::u32string expression, ParserContext* _context = nullptr)
    {
        Dependencies dependencies;
        return Parse(id, expression, &dependencies, AngleMeasure::Radian, AngleMeasure::None, Notation::Decimal, -1, _context);
    }

    Number Parse(ElementId id, std::u32string expression, const int precision, ParserContext* _context = nullptr)
    {
        Dependencies dependencies;
        return Parse(id, expression, &dependencies, AngleMeasure::Radian, AngleMeasure::None, Notation::Decimal, precision, _context);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies* dependencies, ParserContext* _context = nullptr)
    {
        return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None, Notation::Decimal, -1, _context);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies* dependencies, Notation default_notation, ParserContext* _context = nullptr)
    {
        return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None, default_notation, -1, _context);
    }

    Number Parse(ElementId id, std::string expression, Dependencies* dependencies, Notation default_notation, ParserContext* _context = nullptr)
    {
        return Parse(id, ToUtfString(expression), dependencies, AngleMeasure::Radian, AngleMeasure::None, default_notation, -1, _context);
    }

    Number Parse(ElementId id, std::string expression, Dependencies* dependencies, ParserContext* _context = nullptr)
    {
        return Parse(id, ToUtfString(expression), dependencies, AngleMeasure::Radian, AngleMeasure::None, -1, _context);
    }

    Number Parse(ElementId id, std::u32string expression, Notation default_notation, ParserContext* _context = nullptr)
    {
        Dependencies dependencies;
        return Parse(id, expression, &dependencies, AngleMeasure::Radian, AngleMeasure::None, default_notation, -1, _context);
    }

    Number Parse(ElementId id, std::u32string expression, Dependencies* dependencies, 
        AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, const int precision, const int res_count, 
        std::vector<Number>& results, ParserContext* _context = nullptr)
    {
        if (expression.empty() || expression == U";")
            throw SyntaxException(id, ExpressionExpected, 0, 0);
        
        context = _context;
        if (context)
            context->break_solving = false;
        
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
        std::vector<Number>& results, ParserContext* _context = nullptr)
    {
        return Parse(id, ToUtfString(expression), dependencies, default_angle_measure, result_angle_measure, precision, res_count, results, _context);
    }

    bool RemoveIdentifier(ElementId id, const std::u32string& name)
    {
        return solver.RemoveIdentifier(id, name);
    }

    bool RemoveIdentifier(ElementId id, const std::string& name)
    {
        return RemoveIdentifier(id, ToUtfString(name));
    }

    void RemoveUserIdentifiers()
    {
        solver.RemoveUserIdentifiers();
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

    void ListBuiltinUnits(std::vector<std::pair<std::u32string, std::u32string>>& units)
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

    void SetMaxCastUnitSize(int max_cast_unit_size)
    {
        solver.SetMaxCastUnitSize(max_cast_unit_size);
    }

private:
    void InitUnits()
    {
        solver.ResetBuildinUnits();

        switch (language)
        {
        case Language::English:
            solver.AddBuiltinUnit(Unit(U"m", U"metre", 1)); //meter (length)
            solver.AddBuiltinUnit(Unit(U"kg", U"kilogram", 1)); //kilogram (mass)
            solver.AddBuiltinUnit(Unit(U"s", U"second", 1)); //second (time)
            solver.AddBuiltinUnit(Unit(U"mol", U"mole", 1)); //mole (amount of matter)
            solver.AddBuiltinUnit(Unit(U"A", U"ampere", 1)); //ampere (electric current)
            solver.AddBuiltinUnit(Unit(U"cd", U"candella", 1)); //candella (luminosity)
            solver.AddBuiltinUnit(Unit(U"K", U"kelvin", 1)); //kelvin (temperature)
            solver.AddBuiltinUnit(Unit(U"rad", U"radian", 1)); //radian (angle)
            solver.AddBuiltinUnit(Unit(U"sr", U"steradian", 1)); //steradian (solid angle)
            solver.AddBuiltinUnit(Unit(U"bit")); //bit (data)
            break;
        case Language::Russian:
            solver.AddBuiltinUnit(Unit(U"м", U"метр", 1)); //meter (length)
            solver.AddBuiltinUnit(Unit(U"кг", U"килограмм", 1)); //kilogram (mass)
            solver.AddBuiltinUnit(Unit(U"с", U"секунда", 1)); //second (time)
            solver.AddBuiltinUnit(Unit(U"моль")); //mole (amount of matter)
            solver.AddBuiltinUnit(Unit(U"А", U"ампер", 1)); //ampere (electric current)
            solver.AddBuiltinUnit(Unit(U"Кд", U"кандела", 1)); //candella (luminosity)
            solver.AddBuiltinUnit(Unit(U"К", U"кельвин", 1)); //kelvin (temperature)
            solver.AddBuiltinUnit(Unit(U"рад", U"радиан", 1)); //radian (angle)
            solver.AddBuiltinUnit(Unit(U"ср", U"стерадиан", 1)); //steradian (solid angle)
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

    ParserContext* context = nullptr;

    Language language = Language::English;
    Language last_language = Language::None;

    uint64_t max_time = 0;

    std::map<Language, std::vector<std::u32string>> si_units = 
        {
            {
                Language::English,
                {
                    //distance
                    U"nm`nanometre`~0.000000001m;",
                    U"mcm`micrometre`~0.000001m;",
                    U"mm`millimetre`~0.001m;",
                    U"cm`centimetre`~0.01m;",
                    U"dm`decimetre`~0.1m;",
                    U"km`kilometre`~1000m;",

                    //time
                    U"ps`picosecond`~0.000000000001s;",
                    U"ns`nanosecond`~0.000000001s;",
                    U"mcs`microsecond`~0.000001s;",
                    U"ms`millisecond`~0.001s;",
                    U"min`minute`~60s;",
                    U"hour~60min;",
                    U"day~24hour;",
                    U"week~7day;",

                    //mass
                    U"g`gram`~0.001kg;",
                    U"mcg`microgram`~0.000001kg;",
                    U"mg`milligram`~0.001kg;",
                    U"ton~1000kg;",

                    //electric current
                    U"mcA`microampere`~0.000001A;",
                    U"mA`milliampere`~0.001A;",
                    U"kA`kiloampere`~1000A;",
                    U"MA`megaampere`~1000000A;",

                    //frequency
                    U"Hz`hertz`~1/s;",
                    U"kHz`kilohertz`~1000Hz;",
                    U"MHz`megahertz`~1000kHz;",
                    U"GHz`gigahertz`~1000MHz;",

                    //force
                    U"N`newton`~(kg*m)/(pow(s,2));",
                    U"mcN`micronewton`~0.000001N;",
                    U"mN`millinewton`~0.001N;",
                    U"kN`kilonewton`~1000N;",
                    U"MN`meganewton`~1000kN;",

                    //energy
                    U"J`joule`~N*m;",
                    U"mcJ`microjoule`~0.000001J;",
                    U"mJ`millijoule`~0.001J;",
                    U"kJ`kilojoule`~1000J;",
                    U"MJ`megajoule`~1000000J;",

                    //power
                    U"W`watt`~(J)/(s);",
                    U"mcW`microwatt`~0.000001W;",
                    U"mW`milliwatt`~0.001W;",
                    U"kW`kilowatt`~1000W;",
                    U"MW`megawatt`~1000000W;",

                    //pressure
                    U"Pa`pascal`~(N)/(pow(m,2));",
                    U"mcPa`micropascal`~0.000001Pa;",
                    U"mPa`millipascal`~0.001Pa;",
                    U"kPa`kilopascal`~1000Pa;",
                    U"MPa`megapascal`~1000000Pa;",

                    //luminous intensity
                    U"mccd`microcandela`~0.000001cd;",
                    U"mcd`millicandela`~0.001cd;",
                    U"kcd`kilocandela`~1000cd;",
                    U"Mcd`megacandela`~1000000cd;",

                    //luminous flux
                    U"lm`lumen`~cd*sr;",
                    U"mclm`microlumen`~0.000001lm;",
                    U"mlm`millilumen`~0.001lm;",
                    U"klm`kilolumen`~1000lm;",
                    U"Mlm`megalumen`~1000000lm;",

                    //illuminance
                    U"lx`lux`~lm/pow(m,2);",
                    U"mclx`microlux`~0.000001lx;",
                    U"mlx`millilux`~0.001lx;",
                    U"klx`kilolux`~1000lx;",
                    U"Mlx`megalux`~1000000lx;",

                    //electrical charge
                    U"C`coulomb`~A*s;",
                    U"mcC`microcoulomb`~0.000001C;",
                    U"mC`millicoulomb`~0.001C;",
                    U"kC`kilocoulomb`~1000C;",
                    U"MC`megacoulomb`~1000000C;",

                    //potential
                    U"V`volt`~(J)/(C);",
                    U"pV`pikovolt`~0.000000000001V;",
                    U"nV`nanovolt`~0.000000001V;",
                    U"mcV`microvolt`~0.000001V;",
                    U"mV`millivolt`~0.001V;",
                    U"kV`kilovolt`~1000V;",
                    U"MV`megavolt`~1000000V;",

                    //electrical resistance
                    U"Ohm~V/A;",
                    U"mcOhm`microohm`~0.000001Ohm;",
                    U"mOhm`milliohm`~0.001Ohm;",
                    U"kOhm`kiloohm`~1000Ohm;",
                    U"MOhm`megaohm`~1000000Ohm;",

                    //electrical capacity
                    U"F`farad`~C/V;",
                    U"pF`pikofarad`~0.000000000001F;",
                    U"nF`nanofarad`~0.000000001F;",
                    U"mcF`microfarad`~0.000001F;",
                    U"mF`millifarad`~0.001F;",

                    //magnetic flux
                    U"Wb`weber`~(kg*pow(m,2)/(pow(s,2)*A));",
                    U"mcWb`microweber`~0.000001Wb;",
                    U"mWb`milliweber`~0.001Wb;",
                    U"kWb`kiloweber`~1000Wb;",
                    U"MWb`megaweber`~1000000Wb;",

                    //magnetic field
                    U"T`tesla`~(Wb)/(pow(m,2));",
                    U"mcT`microtesla`~0.000001T;",
                    U"mT`millitesla`~0.001T;",
                    U"kT`kilotesla`~1000T;",
                    U"MT`megatesla`~1000000T;",

                    //electrical inductance
                    U"H`henry`~(kg*pow(m,2))/(pow(s,2)*pow(A,2));",
                    U"pH`pikohenry`~0.000000000001H;",
                    U"nH`nanohenry`~0.000000001H;",
                    U"mcH`microhenry`~0.000001H;",
                    U"mH`millihenry`~0.001H;",

                    //electric conductance
                    U"S`siemens`~(1)/(Ohm);",
                    U"mcS`microsiemens`~0.000001S;",
                    U"mS`millisiemens`~0.001S;",
                    U"kS`kilosiemens`~1000S;",
                    U"MS`megasiemens`~1000000S;",

                    //ionizing radiation
                    U"Gy`gray`~(J)/(kg);",
                    U"mcGy`microgray`~0.000001Gy;",
                    U"mGy`milligray`~0.001Gy;",
                    U"kGy`kilogray`~1000Gy;",
                    U"MGy`megagray`~1000000Gy;"
                }
            },
            {
                Language::Russian,
                {
                    //distance
                    U"нм`нанометр`~0.000000001м;",
                    U"мкм`микрометр`~0.000001м;",
                    U"мм`миллиметр`~0.001м;",
                    U"см`сантиметр`~0.01м;",
                    U"дм`дециметр`~0.1м;",
                    U"км`километр`~1000м;",

                    //time
                    U"пс`пикосекунда`~0.000000000001с;",
                    U"нс`наносекунда`~0.000000001с;",
                    U"мкс`микросекунда`~0.000001с;",
                    U"мс`миллисекунда`~0.001с;",
                    U"мин`минута`~60с;",
                    U"час~60мин;",
                    U"сутки~24час;",
                    U"неделя~7сутки;",

                    //mass
                    U"г`грамм`~0.001кг;",
                    U"мкг`микрограмм`~0.000001кг;",
                    U"мг`миллиграмм`~0.001кг;",
                    U"тонна~1000кг;",

                    //electric current
                    U"мкА`микроампер`~0.000001А;",
                    U"мА`миллиампер`~0.001А;",
                    U"кА`килоампер`~1000А;",
                    U"МА`мегаампер`~1000000А;",

                    //frequency
                    U"Гц`герц`~1/с;",
                    U"кГц`килогерц`~1000Гц;",
                    U"МГц`мегагерц`~1000кГц;",
                    U"ГГц`гигагерц`~1000МГц;",

                    //force
                    U"Н`ньютон`~кг*м/pow(с,2);",
                    U"мкН`микроньютон`~0.000001Н;",
                    U"мН`миллиньютон`~0.001Н;",
                    U"кН`килоньютон`~1000Н;",
                    U"МН`меганьютон`~1000кН;",

                    //energy
                    U"Дж`джоуль`~Н*м;",
                    U"мкДж`микроджоуль`~0.000001Дж;",
                    U"мДж`миллиджоуль`~0.001Дж;",
                    U"кДж`килоджоуль`~1000Дж;",
                    U"МДж`мегаджоуль`~1000000Дж;",

                    //power
                    U"Вт`ватт`~Дж/с;",
                    U"мкВт`микроватт`~0.000001Вт;",
                    U"мВт`милливатт`~0.001Вт;",
                    U"кВт`киловатт`~1000Вт;",
                    U"МВт`мегаватт`~1000000Вт;",

                    //pressure
                    U"Па`паскаль`~(Н)/pow(м,2);",
                    U"мкПа`микропаскаль`~0.000001Па;",
                    U"мПа`миллипаскаль`~0.001Па;",
                    U"кПа`килопаскаль`~1000Па;",
                    U"МПа`мегапаскаль`~1000000Па;",

                    //luminous intensity
                    U"мкКд`микрокандела`~0.000001Кд;",
                    U"мКд`милликандела`~0.001Кд;",
                    U"кКд`килокандела`~1000Кд;",
                    U"МКд`мегакандела`~1000000Кд;",

                    //luminous flux
                    U"лм`люмен`~Кд*ср;",
                    U"мклм`микролюмен`~0.000001лм;",
                    U"млм`миллилюмен`~0.001лм;",
                    U"клм`килолюмен`~1000лм;",
                    U"Млм`мегалюмен`~1000000лм;",

                    //illuminance
                    U"лк`люкс`~лм/pow(м,2);",
                    U"мклк`микролюкс`~0.000001лк;",
                    U"млк`миллилюкс`~0.001лк;",
                    U"клк`килолюкс`~1000лк;",
                    U"Млк`мегалюкс`~1000000лк;",

                    //electrical charge
                    U"Кл`кулон`~А*с;",
                    U"мкКл`микрокулон`~0.000001Кл;",
                    U"мКл`милликулон`~0.001Кл;",
                    U"кКл`килокулон`~1000Кл;",
                    U"МКл`мегакулон`~1000000Кл;",

                    //potential
                    U"В`вольт`~Дж/Кл;",
                    U"пВ`пиковольт`~0.000000000001В;",
                    U"нВ`нановольт`~0.000000001В;",
                    U"мкВ`микровольт`~0.000001В;",
                    U"мВ`милливольт`~0.001В;",
                    U"кВ`киловольт`~1000В;",
                    U"МВ`мегавольт`~1000000В;",

                    //electrical resistance
                    U"Ом~В/А;",
                    U"мкОм`микроом`~0.000001Ом;",
                    U"мОм`миллиом`~0.001Ом;",
                    U"кОм`килоом`~1000Ом;",
                    U"МОм`мегаом`~1000000Ом;",

                    //electrical capacity
                    U"Ф`фарад`~Кл/В;",
                    U"пФ`пикофарад`~0.000000000001Ф;",
                    U"нФ`нанофарад`~0.000000001Ф;",
                    U"мкФ`микрофарад`~0.000001Ф;",
                    U"мФ`миллифарад`~0.001Ф;",

                    //magnetic flux
                    U"Вб`вебер`~(кг*pow(м,2)/(pow(с,2)*А));",
                    U"мкВб`микровебер`~0.000001Вб;",
                    U"мВб`милливебер`~0.001Вб;",
                    U"кВб`киловебер`~1000Вб;",
                    U"МВб`мегавебер`~1000000Вб;",

                    //magnetic field
                    U"Тл`тесла`~(Вб)/(pow(м,2));",
                    U"мкТл`микротесла`~0.000001Тл;",
                    U"мТл`миллитесла`~0.001Тл;",
                    U"кТл`килотесла`~1000Тл;",
                    U"МТл`мегатесла`~1000000Тл;",

                    //electrical inductance
                    U"Гн`генри`~(кг*pow(м,2))/(pow(с,2)*pow(А,2));",
                    U"пГн`пикогенри`~0.000000000001Гн;",
                    U"нГн`наногенри`~0.000000001Гн;",
                    U"мкГн`микрогенри`~0.000001Гн;",
                    U"мГн`миллигенри`~0.001Гн;",

                    //electric conductance
                    U"См`сименс`~(1)/(Ом);",
                    U"мкСм`микросименс`~0.000001См;",
                    U"мСм`миллисименс`~0.001См;",
                    U"кСм`килосименс`~1000См;",
                    U"МСм`мегасименс`~1000000См;",

                    //ionizing radiation
                    U"Гр`грей`~(Дж)/(кг);",
                    U"мкГр`микрогрей`~0.000001Гр;",
                    U"мГр`миллигрей`~0.001Гр;",
                    U"кГр`килогрей`~1000Гр;",
                    U"МГр`мегагрей`~1000000Гр;",
                }
            }
        };

    std::map<Language, std::vector<std::u32string>> other_units = 
        {
            {
                Language::English,
                {
                    //distance
                    U"ang`angstrom`~0.000000000001m;",

                    //temperature
                    U"°C~K;",
                    
                    //data
                    U"byte~8bit;",
                    U"kbyte~1024byte;",
                    U"Mbyte~1024kbyte;",
                    U"Gbyte~1024Mbyte;",
                    U"Tbyte~1024Gbyte;",

                    //square
                    U"a`ar`~100*pow(m,2);", //ar
                    U"ha`hectar`~100a;", //hectar

                    //volume
                    U"l`litre`~pow(dm,3);", //litre
                    U"ml`millilitre`~0.001l;",
                    U"dl`decilitre`~0.1l;",
                    U"dal~10l;",
                    U"hl`hectolitre`~100l;"
                }
            },
            {
                Language::Russian,
                {
                    //distance
                    U"анг`ангстрем`~0.0000000001м;",

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
                    U"га`гектар`~100ар;", //hectar

                    //volume
                    U"л`литр`~pow(дм,3);", //litre
                    U"мл`миллилитр`~0.001л;",
                    U"дл`децилитр`~0.1л;",
                    U"дал~10л;",
                    U"гл`гекталитр`~100л;"
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
                    U"с{rus}~с;",
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
                    U"k`Boltzmann constant`=1.380649*pow(10,-23)*(J/K);",
                    U"g_a`Gravitational acceleration`=9.80665(m/pow(s,2));"
                }
            },
            {
                Language::Russian,
                {
                    U"ск_св`скорость света`=299792458(м/с);",
                    U"G`гравитационная постоянная`=6.674301515151515*pow(10,-11)*(pow(м,3)/(кг*pow(с,2)));",
                    U"h`постоянная Планка`=6.62607015*pow(10,-34)*(Дж*с);",
                    U"e_c`элементарный заряд`=1.602176634*pow(10,-19)*Кл;",
                    U"k`постоянная Больцмана`=1.380649*pow(10,-23)*(Дж/К);",
                    U"g_a`ускорение свободного падения`=9.80665(м/pow(с,2));"
                }
            }
        };
};

};

#endif
