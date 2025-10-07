#include "graph.h"

namespace yutovo_calculator
{

template<>
Graph<Array<Real>>::Graph(LogicalId id, std::u32string& expr, Solver<Array<Real>>* solver) :
    Graph::base_type(graph),
    expression(id, expr, solver)
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
    qi::_3_type _3;

    graph = line_graph | bar_graph;

    line_graph = "graph_line(" >> expression >> ',' >> identifier >> ',' >> expression >> ',' >> expression >> ',' >> 
        expression >> ',' >> expression >> ',' >> number >> ',' >> number >> ',' >> number >> ')';

    bar_graph = "graph_bar(" > expression > ',' > number > ')';
    
    identifier = name >> -('{' > (+char_("0-9") | name) > '}') >> -('`' > description > '`');
    
    name = (raw[lexeme[(alpha | char_(U'_')) >> *(alnum | char_(U'_'))]]);

    description = raw[lexeme[+(alnum | '_' | ' ')]];

    number = exp_number | digits_number;

    digits_number = +char_("0-9.");

    exp_number = +char_("0-9.") >> raw[lexeme[(no_case[char_("E")] > (char_('+') | char_('-')))]] > +(char_("0-9"));

    on_error<fail>(graph, 
        boost::phoenix::function<ErrorHandler<SyntaxException>>(ErrorHandler<SyntaxException>(id, expr.begin(), expr.end(), SyntaxError))(_3));

    // BOOST_SPIRIT_DEBUG_NODE(graph);
    // BOOST_SPIRIT_DEBUG_NODE(line_graph);
    // BOOST_SPIRIT_DEBUG_NODE(bar_graph);
    // BOOST_SPIRIT_DEBUG_NODE(identifier);
    // BOOST_SPIRIT_DEBUG_NODE(number);
}

}
