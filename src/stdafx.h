#ifndef STDAFX_H
#define STDAFX_H

#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_UNICODE

#if defined(_MSC_VER)
# pragma warning(disable: 4345)
#endif

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/function.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/spirit/include/qi_no_case.hpp>
#include <boost/foreach.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <stack>
#include <deque>
#include <list>
#include <map>

#include "mpir.h"
#include "mpfr.h"

namespace spirit = boost::spirit;
namespace qi = spirit::qi;

#endif
