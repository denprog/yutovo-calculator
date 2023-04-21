#ifndef __UTILS_H__
#define __UTILS_H__

#include <boost/locale.hpp>
#include <vector>

namespace yutovo_calculator
{

typedef std::vector<uint> ElementId;

std::u32string ToUtfString(const std::string& str);
std::u32string ToUtfString(const char* str);
std::string ToBasicString(const std::u32string& str);

ElementId MakeElementId(ElementId id);

std::string ElementIdToString(const ElementId& id);

}

#endif
