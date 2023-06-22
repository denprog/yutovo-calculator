#ifndef PARSER_H
#define PARSER_H

#include "stdafx.h"
#include "integer.h"
#include "rational.h"
#include "real.h"
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
	Parser(const int precision);
	
	Number Parse(ElementId id, std::u32string expression, Dependencies& dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
		const int precision = -1)
	{
		if (expression.empty() || expression == U";")
			throw SyntaxException(id, ExpressionExpected, 0, 0);
		
		std::u32string::iterator iter = expression.begin();
		std::u32string::iterator end = expression.end();
		unicode::space_type space;

		Script<Number> script(id, expression);
		ScriptNode<Number> script_node;

		phrase_parse(iter, end, script, space, script_node);
		solver.SetDependencies(&dependencies);
		Number res = solver(script_node, id, default_angle_measure, result_angle_measure, precision);
		return solver.GetSuitableUnit(id, res, solver.symbols->last_unit_system);
	}

	Number Parse(ElementId id, std::u32string expression, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, const int precision = -1)
	{
		Dependencies dependencies;
		return Parse(id, expression, dependencies, default_angle_measure, result_angle_measure, precision);
	}

	Number Parse(ElementId id, std::string expression, Dependencies& dependencies, AngleMeasure default_angle_measure, AngleMeasure result_angle_measure, 
		const int precision = -1)
	{
		return Parse(id, ToUtfString(expression), dependencies, default_angle_measure, result_angle_measure, precision);
	}

	Number Parse(ElementId id, std::u32string expression)
	{
		Dependencies dependencies;
		return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None);
	}

	Number Parse(ElementId id, std::u32string expression, Dependencies& dependencies)
	{
		return Parse(id, expression, dependencies, AngleMeasure::Radian, AngleMeasure::None);
	}

	Number Parse(ElementId id, std::string expression, Dependencies& dependencies)
	{
		return Parse(id, ToUtfString(expression), dependencies, AngleMeasure::Radian, AngleMeasure::None);
	}

	bool RemoveIdentifier(ElementId id, const std::u32string& name)
	{
		return solver.RemoveIdentifier(id, name);
	}

	bool RemoveIdentifier(ElementId id, const std::string& name)
	{
		return RemoveIdentifier(id, ToUtfString(name));
	}

	void AddUnits()
	{
		solver.AddBuildinUnit(Unit(U"m")); //meter (length)
		solver.AddBuildinUnit(Unit(U"kg")); //kilogram (mass)
		solver.AddBuildinUnit(Unit(U"s")); //second (time)
		solver.AddBuildinUnit(Unit(U"mol")); //mole (amount of matter)
		solver.AddBuildinUnit(Unit(U"A")); //ampere (electric current)
		solver.AddBuildinUnit(Unit(U"Cd")); //candella (luminosity)
		solver.AddBuildinUnit(Unit(U"k")); //kelvin (temperature)

		//derived SI units
	    Parse(ElementId{0, 0, 0, 0, 0, 0, 1}, U"mm~(1)/(1000)m;");
	    Parse(ElementId{0, 0, 0, 0, 0, 0, 2}, U"cm~10mm;");
	    Parse(ElementId{0, 0, 0, 0, 0, 0, 3}, U"km~1000m;");

		Parse(ElementId{0, 0, 0, 0, 0, 0, 4}, U"Hz~1/s;");
	    Parse(ElementId{0, 0, 0, 0, 0, 0, 5}, U"kHz~1000Hz;");
	    Parse(ElementId{0, 0, 0, 0, 0, 0, 6}, U"MHz~1000kHz;");

	    Parse(ElementId{0, 0, 0, 0, 0, 0, 7}, U"N~kg*m/pow(s,2);");
	    Parse(ElementId{0, 0, 0, 0, 0, 0, 8}, U"kN~1000N;");
	    Parse(ElementId{0, 0, 0, 0, 0, 0, 9}, U"MN~1000kN;");

		//derived russian units
		Parse(ElementId{0, 0, 0, 0, 0, 0, 10}, U"tochka{rus}~(254)/(1000)mm;");
		Parse(ElementId{0, 0, 0, 0, 0, 0, 12}, U"liniya{rus}~10tochka{rus};");
		Parse(ElementId{0, 0, 0, 0, 0, 0, 13}, U"sotka{rus}~84tochka{rus};");
		Parse(ElementId{0, 0, 0, 0, 0, 0, 14}, U"dyum{rus}~10liniya{rus};");
		Parse(ElementId{0, 0, 0, 0, 0, 0, 15}, U"vershok{rus}~(7)/(4)dyum{rus};");
		Parse(ElementId{0, 0, 0, 0, 0, 0, 16}, U"chetvert{rus}~7dyum{rus};");
		Parse(ElementId{0, 0, 0, 0, 0, 0, 17}, U"fut{rus}~12dyum{rus};");
		Parse(ElementId{0, 0, 0, 0, 0, 0, 18}, U"arshin{rus}~28dyum{rus};");
		Parse(ElementId{0, 0, 0, 0, 0, 0, 19}, U"sazhen{rus}~7fut{rus};");
		Parse(ElementId{0, 0, 0, 0, 0, 0, 20}, U"versta{rus}~500sazhen{rus};");
	}	

	Solver<Number> solver;
};

};

#endif
