#include "util.h"

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
