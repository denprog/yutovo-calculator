#ifndef __UTILS_H__
#define __UTILS_H__

#include <boost/locale.hpp>
#include <vector>
#include <atomic>

namespace yutovo_calculator
{

typedef unsigned int uint;

extern clockid_t thread_clock_id;

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

void GetThreadTime(uint64_t& time);

struct ParserContext
{
    void Init(uint64_t max_time)
    {
        break_solving = false;

        if (max_time == 0)
        {
            end_time = 0;
            return;
        }

        uint64_t now;
        GetThreadTime(now);
        end_time = now + max_time;
    }

    std::atomic<bool> break_solving{false};
    uint64_t end_time = 0; //solve before this time or rise TimeExceedException
};

std::u32string ToUtfString(const std::string& str);
std::u32string ToUtfString(const char* str);
std::string ToBasicString(const std::u32string& str);

std::string ElementIdToString(const ElementId& id);
std::string LogicalIdToString(const LogicalId& id);

bool IsLess(const LogicalId& id1, const LogicalId& id2);

void CheckBreak(ParserContext* parser_context);

}

#endif
