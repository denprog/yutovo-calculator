#include "utils.h"
#include "parser_exception.h"
#include <chrono>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#ifdef _WIN32
#include <processthreadsapi.h>
#include <timezoneapi.h>
#endif

namespace yutovo_calculator
{

#ifdef _WIN32
HANDLE thread_handle = 0;
#else
clockid_t thread_clock_id = 0;
#endif

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

void InitThreadTime()
{
#ifdef _WIN32
    thread_handle = GetCurrentThread();
#else
    pthread_getcpuclockid(pthread_self(), &thread_clock_id);
#endif
}

void GetThreadTime(uint64_t& time)
{
#ifdef EMSCRIPTEN
    time = (uint64_t)emscripten_get_now();
#elif _WIN32
    FILETIME creation_time, exit_time, kernel_time, user_time;
    if (GetThreadTimes(thread_handle, &creation_time, &exit_time, &kernel_time, &user_time))
    {
        SYSTEMTIME s1, s2;
        if (FileTimeToSystemTime(&kernel_time, &s1) && FileTimeToSystemTime(&user_time, &s2))
            time = s1.wMilliseconds + s1.wSecond * 1000 + s1.wMinute * 60 * 1000 + s2.wMilliseconds + s2.wSecond * 1000 + s2.wMinute * 60 * 1000;
    }
#else
    timespec s;
    clock_gettime(thread_clock_id, &s);
    time = s.tv_sec * 1000 + s.tv_nsec / 1000000;
#endif
}

}
