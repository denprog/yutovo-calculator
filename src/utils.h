#ifndef __UTILS_H__
#define __UTILS_H__

#include <boost/locale.hpp>
#include <vector>
#include <atomic>

namespace yutovo_calculator
{

typedef unsigned int uint;

struct ElementId : std::vector<int>
{
    ElementId() = default;

    ElementId(std::initializer_list<int> c) : 
        std::vector<int>{c}
    {
    }

    ElementId(const ElementId& source, const int size) : 
        std::vector<int>(source.begin(), source.begin() + size)
    {
    }
};

struct LogicalId : std::vector<int> //logical Id does not include row id, so it does not depend on the formatting of rows
{
    LogicalId() = default;

    LogicalId(std::initializer_list<int> c) : 
        std::vector<int>{c}
    {
    }

    LogicalId(const LogicalId& source, const int size) : 
        std::vector<int>(source.begin(), source.begin() + size)
    {
    }
};

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
