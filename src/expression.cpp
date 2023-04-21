#include "stdafx.h"
#include "expression.h"

namespace yutovo_calculator
{
	template<>
	Expression<Integer>::Expression(ElementId id, std::u32string& expr) : Expression::base_type(expression)
	{
		using unicode::char_;
		using boost::spirit::qi::raw;
		using boost::spirit::qi::lexeme;
		using unicode::alnum;
		using unicode::alpha;
		using boost::spirit::qi::omit;
		using boost::spirit::qi::on_error;
		using boost::spirit::qi::on_success;
		using boost::spirit::qi::fail;
		using boost::phoenix::function;
		using namespace boost::phoenix::arg_names;
		qi::_1_type _1;
		qi::_3_type _3;

		expression = addition.alias();
		
		addition = multiplication >> *((char_('+') > multiplication) | (char_('-') > multiplication));
		
		multiplication = unary >> *(multiply);

		multiply = char_('*') > unary | char_('/') > unary;
		
		unary = number | function_call | no_fences_function_call | identifier | unary_operation | '(' > expression > ')';
		
		number = digits_number;
		
		digits_number = +char_("0-9");

		identifier = name;
		
		name = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

		unary_operation = (qi::char_('+') > unary) | (qi::char_('-') > unary);
		
		function_call = identifier >> '(' >> -(expression % ',') > ')';

		no_fences_function_call = (identifier >> '%' >> *(expression >> omit[',']) >> function_param);
		
		function_param = number | identifier | '(' > expression > ')';

		//annotate the items with an expression's position
		on_success(unary, 
			boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(addition, 
			boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(multiplication, 
			boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(multiply, 
			boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(identifier, 
			boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
		
		//work out the exceptions
		on_error<fail>(expression, 
			boost::phoenix::function<ErrorHandler<SyntaxException>>(ErrorHandler<SyntaxException>(id, expr.begin(), expr.end(), SyntaxError))(_3));
		
		// BOOST_SPIRIT_DEBUG_NODE(expression);
		// BOOST_SPIRIT_DEBUG_NODE(addition);
		// BOOST_SPIRIT_DEBUG_NODE(multiplication);
		// BOOST_SPIRIT_DEBUG_NODE(number);
		// BOOST_SPIRIT_DEBUG_NODE(function_call);
		// BOOST_SPIRIT_DEBUG_NODE(identifier);
	}

	template<>
	Expression<Real>::Expression(ElementId id, std::u32string& expr) : Expression::base_type(expression)
	{
		using unicode::char_;
		using boost::spirit::qi::raw;
		using boost::spirit::qi::lexeme;
		using unicode::alnum;
		using unicode::alpha;
		using boost::spirit::qi::omit;
		using boost::spirit::qi::no_case;
		using boost::spirit::qi::on_error;
		using boost::spirit::qi::fail;
		using boost::phoenix::function;
		using namespace boost::phoenix::arg_names;
		qi::_1_type _1;
		qi::_3_type _3;

		expression = addition.alias();
		
		addition = multiplication >> *((char_('+') > multiplication) | (char_('-') > multiplication));
		
		multiplication = unary >> *(multiply);

		multiply = char_('*') > unary | char_('/') > unary;
		
		unary = number | function_call | no_fences_function_call | identifier | unary_operation | '(' > expression > ')';
		
		number = exp_number | digits_number;
		
		digits_number = +char_("0-9.");
		
		exp_number = +char_("0-9.") >> raw[lexeme[(no_case[char_("E")] > (char_('+') | char_('-')))]] > +(char_("0-9"));

		identifier = name;
		
		name = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

		unary_operation = (qi::char_('+') > unary) | (qi::char_('-') > unary);
		
		function_call = identifier >> '(' >> -(expression % ',') > ')';
		
		no_fences_function_call = (identifier >> '%' >> *(expression >> omit[',']) >> function_param);
		
		function_param = number | identifier | '(' > expression > ')';

		//annotate the items with the expression's position
		on_success(unary, 
			boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(addition, 
			boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(multiplication, 
			boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(multiply, 
			boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(function_call, 
			boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(no_fences_function_call, 
			boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
		on_success(identifier, 
			boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
		
		on_error<fail>(expression, 
			boost::phoenix::function<ErrorHandler<SyntaxException>>(ErrorHandler<SyntaxException>(id, expr.begin(), expr.end(), SyntaxError))(_3));
		
		// BOOST_SPIRIT_DEBUG_NODE(expression);
		// BOOST_SPIRIT_DEBUG_NODE(addition);
		// BOOST_SPIRIT_DEBUG_NODE(multiplication);
		// BOOST_SPIRIT_DEBUG_NODE(number);
		// BOOST_SPIRIT_DEBUG_NODE(function_call);
		// BOOST_SPIRIT_DEBUG_NODE(identifier);
	}

	template<>
	Expression<yutovo_calculator::Rational>::Expression(ElementId id, std::u32string& expr) : Expression::base_type(expression)
	{
		using unicode::char_;
		using boost::spirit::qi::raw;
		using boost::spirit::qi::lexeme;
		using unicode::alnum;
		using unicode::alpha;
		using boost::spirit::qi::omit;
		using boost::spirit::qi::on_error;
		using boost::spirit::qi::fail;
		using boost::phoenix::function;
		using namespace boost::phoenix::arg_names;
		qi::_1_type _1;
		qi::_3_type _3;

		expression = addition.alias();
		
		addition = multiplication >> *((char_('+') > multiplication) | (char_('-') > multiplication));
		
		multiplication = unary >> *(multiply);

		multiply = char_('*') > unary | char_('/') > unary;
		
		unary = number | function_call | no_fences_function_call | identifier | unary_operation | '(' > expression > ')';
		
		number = digits_number;
		
		digits_number = +char_("0-9");

		identifier = name;
		
		name = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

		unary_operation = (qi::char_('+') > unary) | (qi::char_('-') > unary);
		
		function_call = (identifier >> '(' >> -(expression % ',') > ')');

		no_fences_function_call = (identifier >> '%' >> *(expression >> omit[',']) >> function_param);
		
		function_param = number | identifier | '(' > expression > ')';

		//annotate the items with an expression's position
		on_success(unary, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
			expr.end(), id))(qi::_val, _1));
		on_success(addition, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
			expr.end(), id))(qi::_val, _1));
		on_success(multiplication, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
			expr.end(), id))(qi::_val, _1));
		on_success(multiply, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
			expr.end(), id))(qi::_val, _1));
		on_success(identifier, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
			expr.end(), id))(qi::_val, _1));
		
		//work out the exceptions
		on_error<fail>(expression, 
			boost::phoenix::function<ErrorHandler<SyntaxException>>(ErrorHandler<SyntaxException>(id, expr.begin(), expr.end(), SyntaxError))(_3));
		
		//BOOST_SPIRIT_DEBUG_NODE(expression);
		//BOOST_SPIRIT_DEBUG_NODE(addition);
		//BOOST_SPIRIT_DEBUG_NODE(multiplication);
		//BOOST_SPIRIT_DEBUG_NODE(number);
		//BOOST_SPIRIT_DEBUG_NODE(function_call);
		//BOOST_SPIRIT_DEBUG_NODE(identifier);
	}
};
