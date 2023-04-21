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
		
		Number Parse(ElementId id, std::u32string expression, Dependencies& dependencies, const int precision = -1)
		{
			if (expression.empty() || expression == U";")
				throw SyntaxException(id, ExpressionExpected, 0, 0);
			
			Number res;
			std::u32string::iterator iter = expression.begin();
			std::u32string::iterator end = expression.end();
			unicode::space_type space;

			Script<Number> script(id, expression);
			ScriptNode<Number> script_node;

			phrase_parse(iter, end, script, space, script_node);
			solver.SetDependencies(&dependencies);
			return solver(script_node, id, precision);
		}

		Number Parse(ElementId id, std::u32string expression, const int precision = -1)
		{
			Dependencies dependencies;
			return Parse(id, expression, dependencies, precision);
		}

		Number Parse(ElementId id, std::string expression, Dependencies& dependencies, const int precision = -1)
		{
			return Parse(id, ToUtfString(expression), dependencies, precision);
		}

		bool RemoveIdentifier(ElementId id, const std::u32string& name)
		{
			return solver.RemoveIdentifier(id, name);
		}

		bool RemoveIdentifier(ElementId id, const std::string& name)
		{
			return RemoveIdentifier(id, ToUtfString(name));
		}

		void SetPrecision(const int precision)
		{
			solver.SetPrecision(precision);
		}
		
		Solver<Number> solver;	///< The solver
	};
};

#endif
