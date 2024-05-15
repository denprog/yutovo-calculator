#ifndef __UTILS_H__
#define __UTILS_H__

#include <boost/locale.hpp>
#include <vector>
#include <atomic>

namespace yutovo_calculator
{

typedef unsigned int uint;
typedef std::vector<int> ElementId;

struct ParserContext
{
    std::atomic<bool> break_solving{false};
};

std::u32string ToUtfString(const std::string& str);
std::u32string ToUtfString(const char* str);
std::string ToBasicString(const std::u32string& str);

ElementId MakeElementId(ElementId id);

std::string ElementIdToString(const ElementId& id);

bool IsLess(const ElementId& id1, const ElementId& id2);

}

#endif
