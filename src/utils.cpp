#include "utils.h"

namespace yutovo_calculator
{

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

}
