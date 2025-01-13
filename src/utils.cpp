#include "utils.h"
#include "parser_exception.h"
#include <chrono>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

namespace yutovo_calculator
{

clockid_t thread_clock_id = 0;

std::u32string ToUtfString(const std::string& str)
{
    return boost::locale::conv::utf_to_utf<char32_t>(str);
}

std::u32string ToUtfString(const char* str)
{
    return boost::locale::conv::utf_to_utf<char32_t>(str);
}

std::string ToBasicString(const std::u32string& str)
{
    return boost::locale::conv::utf_to_utf<char>(str);
}

std::string ElementIdToString(const ElementId& id)
{
	std::string res;
	for (size_t i = 0; i < id.size(); ++i)
	{
		res += std::to_string(id[i]);
		if (i < id.size() - 1)
			res += ",";
	}
	return res;
}

std::string LogicalIdToString(const LogicalId& id)
{
	std::string res;
	for (size_t i = 0; i < id.size(); ++i)
	{
		res += std::to_string(id[i]);
		if (i < id.size() - 1)
			res += ",";
	}
	return res;
}

bool IsLess(const LogicalId& id1, const LogicalId& id2)
{
	for (size_t i = 0, j = 0; i < id1.size() && j < id2.size(); ++i, ++j)
	{
		if (id1[i] > id2[j])
			return false;
		if (id1[i] < id2[j])
			return true;
	}
	return id1.size() < id2.size();
}

void CheckBreak(ParserContext* parser_context)
{
	if (!parser_context)
		return;
    if (parser_context->break_solving)
        throw BreakException();
    if (parser_context->end_time == 0)
        return;
    
    uint64_t now;
    GetThreadTime(now);
	if (now > parser_context->end_time)
		throw TimeExceedException();
}

void GetThreadTime(uint64_t& time)
{
#ifdef EMSCRIPTEN
    time = (uint64_t)emscripten_get_now();
#else
    timespec s;
    clock_gettime(thread_clock_id, &s);
    time = s.tv_sec * 1000 + s.tv_nsec / 1000000;
#endif
}

}
