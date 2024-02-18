#include "stdafx.h"
#include "expression.h"
#include "solver.h"

namespace yutovo_calculator
{

//Expression

template<>
Expression<Integer>::Expression(ElementId id, std::u32string& expr, Solver<Integer>* _solver) : 
    Expression::base_type(expression),
    solver(_solver)
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

    multiply = char_('*') > unary | char_('/') > unary | char_('&') > unary | char_('|') > unary | char_('^') > unary;

    if (solver->default_notation == 16)
    {
        unary = compare | postfix_operation | implicit_mul | function_call | function_call_string | 
            no_fences_function_call | identifier | number | implicit_string_mul | unary_operation | '(' > expression > ')';
    }
    else
    {
        unary = compare | postfix_operation | implicit_string_mul | implicit_mul | number | function_call | function_call_string | 
            identifier | no_fences_function_call | unary_operation | '(' > expression > ')';
    }
    
    if (solver->default_notation == 16)	
        number = hex_number;
    else
        number = dec_number;
    
    hex_number = +char_("0-9a-fA-F");

    dec_number = +char_("0-9");

    identifier = name >> -('{' > (dec_number | name) > '}');
    
    name = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

    unary_operation = (char_('+') > unary) | (char_('-') > unary) | (char_('!') > unary);

    postfix_operation = ((number | '(' > expression > ')') >> char_('!'));

    implicit_string_mul = (number >> identifier);
    
    implicit_mul = number >> '(' >> expression > ')';

    function_call = identifier >> '(' >> -(expression % ',') > ')';

    function_call_string = identifier >> '[' >> +(alnum | '_') > ']';

    no_fences_function_call = (identifier >> '%' >> *(expression >> omit[',']) >> function_param);
    
    function_param = number | identifier | '(' > expression > ')';

    compare = '(' >> expression >> (+char_("<>") | +char_("==") | char_('<') | char_('>')) >> expression >> ')';

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
    on_success(implicit_string_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(postfix_operation, 
        boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(function_call_string, 
        boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_fraction_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Integer>>(Annotation<yutovo_calculator::Integer>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(compare, 
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
    // BOOST_SPIRIT_DEBUG_NODE(unary);
    // BOOST_SPIRIT_DEBUG_NODE(postfix_operation);
}

template<>
Expression<Real>::Expression(ElementId id, std::u32string& expr, Solver<Real>* _solver) : 
    Expression::base_type(expression),
    solver(_solver)
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

    multiply = char_('*') > unary | char_('/') > unary | char_('%') > unary;
    
    unary = compare | postfix_operation | implicit_div_mul | implicit_string_mul | implicit_fraction_mul | mixed_division | implicit_mul | number | function_call | 
        no_fences_function_call | identifier | unary_operation | '(' > expression > ')';
    
    number = exp_number | digits_number;
    
    auto n = std::string("0-9.");
    if (solver->decimal_point != '.')
        n += solver->decimal_point; //add the local decimal point character
    digits_number = +char_(n);

    integer_number_str = +char_("0-9");

    integer_number = integer_number_str;

    mixed_division = integer_number >> '(' >> integer_number >> '/' >> integer_number > ')';

    implicit_fraction_mul = '(' >> number >> '/' >> number >> ')' >> identifier;

    real_number = digits_number;

    exp_number = +char_("0-9.") >> raw[lexeme[(no_case[char_("E")] > (char_('+') | char_('-')))]] > +(char_("0-9"));

    identifier = name >> -('{' > (integer_number_str | name) > '}');

    implicit_div_mul = '(' >> expression >> ')' >> '/' >> '(' >> expression >> ')' >> identifier;

    implicit_string_mul = (number >> identifier);

    implicit_mul = real_number >> '(' >> expression > ')';
    
    name = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

    unary_operation = (char_('+') > unary) | (char_('-') > unary);

    postfix_operation = ((number | '(' > expression > ')') >> char_('!'));
    
    function_call = identifier >> '(' >> -(expression % ',') > ')';
    
    no_fences_function_call = (identifier >> '%' >> *(expression >> omit[',']) >> function_param);
    
    function_param = number | identifier | '(' > expression > ')';

    compare = '(' >> expression >> (+char_("<>") | +char_("==") | char_('<') | char_('>')) >> expression >> ')';

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
    on_success(mixed_division, 
        boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_string_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_div_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_fraction_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(compare, 
        boost::phoenix::function<Annotation<yutovo_calculator::Real>>(Annotation<yutovo_calculator::Real>(expr.begin(), expr.end(), id))(qi::_val, _1));
    
    on_error<fail>(expression, 
        boost::phoenix::function<ErrorHandler<SyntaxException>>(ErrorHandler<SyntaxException>(id, expr.begin(), expr.end(), SyntaxError))(_3));
    
    // BOOST_SPIRIT_DEBUG_NODE(expression);
    // BOOST_SPIRIT_DEBUG_NODE(addition);
    // BOOST_SPIRIT_DEBUG_NODE(multiplication);
    // BOOST_SPIRIT_DEBUG_NODE(mixed_division);
    // BOOST_SPIRIT_DEBUG_NODE(number);
    // BOOST_SPIRIT_DEBUG_NODE(function_call);
    // BOOST_SPIRIT_DEBUG_NODE(identifier);
    // BOOST_SPIRIT_DEBUG_NODE(implicit_mul);
    // BOOST_SPIRIT_DEBUG_NODE(implicit_div_mul);
    // BOOST_SPIRIT_DEBUG_NODE(compare);
}

template<>
Expression<yutovo_calculator::Rational>::Expression(ElementId id, std::u32string& expr, Solver<Rational>* _solver) : 
    Expression::base_type(expression),
    solver(_solver)
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
    
    multiplication = (unary >> *(multiply));

    multiply = (char_('*') > unary) | (char_('/') > unary) | (char_('%') > unary);
    
    unary = compare | implicit_div_mul | implicit_string_mul | implicit_fraction_mul | mixed_division | implicit_mul | number | function_call | 
        no_fences_function_call | identifier | unary_operation | '(' > expression > ')';
    
    number = digits_number;

    mixed_division = (number >> '(' >> number >> '/' >> number >> ')');

    implicit_div_mul = ('(' >> expression >> ')' >> '/' >> '(' >> expression >> ')' >> identifier);

    implicit_fraction_mul = '(' >> number >> '/' >> number >> ')' >> identifier;

    implicit_string_mul = (number >> identifier);

    implicit_mul = number >> '(' >> expression > ')';
    
    auto n = std::string("0-9.");
    if (solver->decimal_point != '.')
        n += solver->decimal_point; //add the local decimal point character
    digits_number = +char_(n);

    number = digits_number;

    identifier = name >> -('{' > (digits_number | name) > '}');
    
    name = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

    unary_operation = (char_('+') > unary) | (char_('-') > unary);
    
    function_call = (identifier >> '(' >> -(expression % ',') > ')');

    no_fences_function_call = (identifier >> '%' >> *(expression >> omit[',']) >> function_param);
    
    function_param = number | identifier | '(' > expression > ')';

    compare = '(' >> expression >> (+char_("<>") | +char_("==") | char_('<') | char_('>')) >> expression >> ')';

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
    on_success(mixed_division, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
        expr.end(), id))(qi::_val, _1));
    on_success(implicit_string_mul, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
        expr.end(), id))(qi::_val, _1));
    on_success(implicit_div_mul, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
        expr.end(), id))(qi::_val, _1));
    on_success(implicit_mul, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
        expr.end(), id))(qi::_val, _1));
    on_success(implicit_fraction_mul, boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), 
        expr.end(), id))(qi::_val, _1));
    on_success(compare, 
        boost::phoenix::function<Annotation<yutovo_calculator::Rational>>(Annotation<yutovo_calculator::Rational>(expr.begin(), expr.end(), id))(qi::_val, _1));
    
    //work out the exceptions
    on_error<fail>(expression, 
        boost::phoenix::function<ErrorHandler<SyntaxException>>(ErrorHandler<SyntaxException>(id, expr.begin(), expr.end(), SyntaxError))(_3));
    
    // BOOST_SPIRIT_DEBUG_NODE(expression);
    // BOOST_SPIRIT_DEBUG_NODE(addition);
    // BOOST_SPIRIT_DEBUG_NODE(mixed_division);
    // BOOST_SPIRIT_DEBUG_NODE(multiplication);
    // BOOST_SPIRIT_DEBUG_NODE(number);
    // BOOST_SPIRIT_DEBUG_NODE(function_call);
    // BOOST_SPIRIT_DEBUG_NODE(identifier);
}

template<>
Expression<Complex>::Expression(ElementId id, std::u32string& expr, Solver<Complex>* _solver) : 
    Expression::base_type(expression),
    solver(_solver)
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

    multiply = char_('*') > unary | char_('/') > unary | char_('%') > unary;
    
    unary = compare | postfix_operation | implicit_div_mul | implicit_string_mul | implicit_fraction_mul | mixed_division | implicit_mul | number | function_call | 
        no_fences_function_call | identifier | unary_operation | '(' > expression > ')';
    
    number = exp_number | digits_number;
    
    auto n = std::string("0-9.");
    if (solver->decimal_point != '.')
        n += solver->decimal_point; //add the local decimal point character
    digits_number = +char_(n);

    integer_number_str = +char_("0-9");

    integer_number = integer_number_str;

    mixed_division = integer_number >> '(' >> integer_number >> '/' >> integer_number > ')';

    implicit_fraction_mul = '(' >> number >> '/' >> number >> ')' >> identifier;

    real_number = digits_number;

    exp_number = +char_("0-9.") >> raw[lexeme[(no_case[char_("E")] > (char_('+') | char_('-')))]] > +(char_("0-9"));

    identifier = name >> -('{' > (integer_number_str | name) > '}');

    implicit_div_mul = '(' >> expression >> ')' >> '/' >> '(' >> expression >> ')' >> identifier;

    implicit_string_mul = (number >> identifier);

    implicit_mul = real_number >> '(' >> expression > ')';
    
    name = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

    unary_operation = (char_('+') > unary) | (char_('-') > unary);

    postfix_operation = ((number | '(' > expression > ')') >> char_('!'));
    
    function_call = identifier >> '(' >> -(expression % ',') > ')';
    
    no_fences_function_call = (identifier >> '%' >> *(expression >> omit[',']) >> function_param);
    
    function_param = number | identifier | '(' > expression > ')';

    compare = '(' >> expression >> (+char_("<>") | +char_("==") | char_('<') | char_('>')) >> expression >> ')';

    //annotate the items with the expression's position
    on_success(unary, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(addition, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(multiplication, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(multiply, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(function_call, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(no_fences_function_call, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(identifier, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(mixed_division, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_string_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_div_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(implicit_fraction_mul, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    on_success(compare, 
        boost::phoenix::function<Annotation<yutovo_calculator::Complex>>(Annotation<yutovo_calculator::Complex>(expr.begin(), expr.end(), id))(qi::_val, _1));
    
    on_error<fail>(expression, 
        boost::phoenix::function<ErrorHandler<SyntaxException>>(ErrorHandler<SyntaxException>(id, expr.begin(), expr.end(), SyntaxError))(_3));
    
    // BOOST_SPIRIT_DEBUG_NODE(expression);
    // BOOST_SPIRIT_DEBUG_NODE(addition);
    // BOOST_SPIRIT_DEBUG_NODE(multiplication);
    // BOOST_SPIRIT_DEBUG_NODE(mixed_division);
    // BOOST_SPIRIT_DEBUG_NODE(number);
    // BOOST_SPIRIT_DEBUG_NODE(function_call);
    // BOOST_SPIRIT_DEBUG_NODE(identifier);
    // BOOST_SPIRIT_DEBUG_NODE(implicit_mul);
    // BOOST_SPIRIT_DEBUG_NODE(implicit_div_mul);
}

};
