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
		
		Number Parse(std::u32string expression, Dependencies& dependencies, const int precision = -1)
		{
			if (expression.empty() || expression == U";")
				throw SyntaxException(ExpressionExpected, 0, 0);
			
			Number res;
			std::u32string::iterator iter = expression.begin();
			std::u32string::iterator end = expression.end();
			unicode::space_type space;

			Script<Number> script(expression);
			ScriptNode<Number> script_node;

			phrase_parse(iter, end, script, space, script_node);
			solver.SetDependencies(&dependencies);
			return solver(script_node, precision);
		}

		Number Parse(std::u32string expression, const int precision = -1)
		{
			Dependencies dependencies;
			return Parse(expression, dependencies, precision);
		}

		Number Parse(std::string expression, Dependencies& dependencies, const int precision = -1)
		{
			return Parse(ToUtfString(expression), dependencies, precision);
		}

		bool RemoveIdentifier(const std::u32string& name)
		{
			return solver.RemoveIdentifier(name);
		}

		bool RemoveIdentifier(const std::string& name)
		{
			return RemoveIdentifier(ToUtfString(name));
		}

		void SetPrecision(const int precision)
		{
			solver.SetPrecision(precision);
		}
		
		Solver<Number> solver;	///< The solver
	};
};

#endif
