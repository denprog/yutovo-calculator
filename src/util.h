#ifndef __UTIL_H__
#define __UTIL_H__

#include <boost/locale.hpp>

std::u32string ToUtfString(const std::string& str);
std::u32string ToUtfString(const char* str);
std::string ToBasicString(const std::u32string& str);

#endif
