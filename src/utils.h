#ifndef __UTILS_H__
#define __UTILS_H__

#include <boost/locale.hpp>

namespace yutovo_calculator
{

std::u32string ToUtfString(const std::string& str);
std::u32string ToUtfString(const char* str);
std::string ToBasicString(const std::u32string& str);

}

#endif
