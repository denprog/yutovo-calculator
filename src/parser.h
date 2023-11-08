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
	Parser(const int precision, const Language _language);
	
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

    void SetLanguage(Language _language);

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

	void ListUserUnits(std::vector<std::u32string>& units)
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
			solver.AddBuiltinUnit(Unit(U"Cd")); //candella (luminosity)
			solver.AddBuiltinUnit(Unit(U"k")); //kelvin (temperature)

			solver.symbols->buildin_elements = true;

			//derived SI units
		    Parse(ElementId{0, 0, 1}, U"mm~(1)/(1000)m;");
		    Parse(ElementId{0, 0, 2}, U"cm~10mm;");
		    Parse(ElementId{0, 0, 3}, U"km~1000m;");

			Parse(ElementId{0, 0, 4}, U"Hz~1/s;");
		    Parse(ElementId{0, 0, 5}, U"kHz~1000Hz;");
		    Parse(ElementId{0, 0, 6}, U"MHz~1000kHz;");

		    Parse(ElementId{0, 0, 7}, U"N~kg*m/pow(s,2);");
		    Parse(ElementId{0, 0, 8}, U"kN~1000N;");
		    Parse(ElementId{0, 0, 9}, U"MN~1000kN;");

			Parse(ElementId{0, 0, 10}, U"mcs~(1)/(1000000)s;");
			Parse(ElementId{0, 0, 11}, U"ms~(1)/(1000)s;");
			Parse(ElementId{0, 0, 12}, U"min~60s;");
			Parse(ElementId{0, 0, 13}, U"hour~60min;");
			Parse(ElementId{0, 0, 14}, U"day~24hour;");
			Parse(ElementId{0, 0, 15}, U"week~7day;");

			//derived russian units
			Parse(ElementId{0, 1, 0}, U"tochka{rus}~(254)/(1000)mm;");
			Parse(ElementId{0, 1, 1}, U"liniya{rus}~10tochka{rus};");
			Parse(ElementId{0, 1, 2}, U"sotka{rus}~84tochka{rus};");
			Parse(ElementId{0, 1, 3}, U"dyum{rus}~10liniya{rus};");
			Parse(ElementId{0, 1, 4}, U"vershok{rus}~(7)/(4)dyum{rus};");
			Parse(ElementId{0, 1, 5}, U"chetvert{rus}~7dyum{rus};");
			Parse(ElementId{0, 1, 6}, U"fut{rus}~12dyum{rus};");
			Parse(ElementId{0, 1, 7}, U"arshin{rus}~28dyum{rus};");
			Parse(ElementId{0, 1, 8}, U"sazhen{rus}~7fut{rus};");
			Parse(ElementId{0, 1, 9}, U"versta{rus}~500sazhen{rus};");
			break;
        case Language::Russian:
			solver.AddBuiltinUnit(Unit(U"м")); //meter (length)
			solver.AddBuiltinUnit(Unit(U"кг")); //kilogram (mass)
			solver.AddBuiltinUnit(Unit(U"сек")); //second (time)
			solver.AddBuiltinUnit(Unit(U"моль")); //mole (amount of matter)
			solver.AddBuiltinUnit(Unit(U"А")); //ampere (electric current)
			solver.AddBuiltinUnit(Unit(U"Кд")); //candella (luminosity)
			solver.AddBuiltinUnit(Unit(U"К")); //kelvin (temperature)

			solver.symbols->buildin_elements = true;

			//derived SI units
		    Parse(ElementId{0, 0, 1}, U"мм~(1)/(1000)м;");
		    Parse(ElementId{0, 0, 2}, U"см~10мм;");
		    Parse(ElementId{0, 0, 3}, U"км~1000м;");

			Parse(ElementId{0, 0, 4}, U"Гц~1/сек;");
		    Parse(ElementId{0, 0, 5}, U"кГц~1000Гц;");
		    Parse(ElementId{0, 0, 6}, U"МГц~1000кГц;");

		    Parse(ElementId{0, 0, 7}, U"Н~кг*м/pow(сек,2);");
		    Parse(ElementId{0, 0, 8}, U"кН~1000Н;");
		    Parse(ElementId{0, 0, 9}, U"МН~1000кН;");

			Parse(ElementId{0, 0, 10}, U"мкс~(1)/(1000000)сек;");
			Parse(ElementId{0, 0, 11}, U"мс~(1)/(1000)сек;");
			Parse(ElementId{0, 0, 12}, U"мин~60сек;");
			Parse(ElementId{0, 0, 13}, U"ч~60мин;");
			Parse(ElementId{0, 0, 14}, U"д~24ч;");
			Parse(ElementId{0, 0, 15}, U"нед~7д;");

			//derived russian units
			Parse(ElementId{0, 1, 0}, U"точка{rus}~(254)/(1000)мм;");
			Parse(ElementId{0, 1, 1}, U"линия{rus}~10точка{rus};");
			Parse(ElementId{0, 1, 2}, U"сотка{rus}~84точка{rus};");
			Parse(ElementId{0, 1, 3}, U"дюйм{rus}~10линия{rus};");
			Parse(ElementId{0, 1, 4}, U"вершок{rus}~(7)/(4)дюйм{rus};");
			Parse(ElementId{0, 1, 5}, U"четверть{rus}~7дюйм{rus};");
			Parse(ElementId{0, 1, 6}, U"фут{rus}~12дюйм{rus};");
			Parse(ElementId{0, 1, 7}, U"аршин{rus}~28дюйм{rus};");
			Parse(ElementId{0, 1, 8}, U"сажень{rus}~7фут{rus};");
			Parse(ElementId{0, 1, 9}, U"верста{rus}~500сажень{rus};");
            break;
        default:
            throw ParserException({}, ParserExceptionCode::UnknownLanguage);
		}

		solver.symbols->buildin_elements = false;
	}	

	Solver<Number> solver;
	Language language = Language::English;
};

};

#endif
