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

using namespace std;

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace phx = boost::phoenix;
namespace fusion = boost::fusion;

namespace yutovo_calculator
{
	/**
	 * Big numbers parser.
	 */
	template<typename Number>
	struct Parser
	{
		Parser(const int precision);
		
		/**
		 * Parses an expression.
		 * @param [in,out] expression The expression.
		 * @param precision	(optional) the precision.
		 * @return The result.
		 */
		Number Parse(std::string expression, const int precision = -1);

		/**
		 * Sets a precision.
		 * @param precision The precision.
		 */
		void SetPrecision(const int precision)
		{
			solver.SetPrecision(precision);
		}
		
		Solver<Number> solver;	///< The solver
	};
};

#endif
