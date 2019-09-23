#include "SimpleCfg.h"
#include "log.h"
#include <sstream>

using json = nlohmann::json;
using namespace std;

namespace
{
	string Js2String(json &js)
	{
		std::stringstream ss;
		ss << js;

		return ss.str();
	}

}

bool SimpleCfg::ParseSimpleCfg(const string &cfg)
{
	try
	{
		if (!ParseObj(cfg.cbegin(), cfg.cend(), *this))
		{
			Log("parse fail");
			return false;
		}
		Log("parse ok");
		return true;
	}
	catch (exception& e)
	{
		Log("parse json fail. exception %s", e.what());
	}
	catch (...)
	{
		Log("parse json fail");
	}
	return false;
}

const char *SimpleCfg::c_str()
{
	std::stringstream ss;
	ss << *this;
	static string str;
	str.assign(ss.str());
	return str.c_str();
}

bool SimpleCfg::BuildCppCfg(string &cpp_str)
{
	return true;
}

bool SimpleCfg::CheckIllegal()
{
	return true;
}

//字符串解析成json对象
//@start, end 字符串迭代器，错误用户自己负责
bool SimpleCfg::ParseObj(CharIter start, CharIter end, json &js)
{
	if (start == end)
	{
		Log("error, empty cfg string");
		return false;
	}

	//迭代每个成员变量
	CharIter cur = start; //指向当前待分析的字符串开始位置
	for (uint32 n = 0; n < 50000; ++n)
	{
		//处理完成
		if (cur == end)
		{
			return true;
		}

		string name, value;
		if (!Find1stMemStr(cur, end, name, value))
		{
			return true;//没有成员了
		}
		L_DEBUG("find member %s = %s", name.c_str(), value.c_str());
		//到这里， cur 已经指向下一个成员字符串开头
		if (value[0] == '{')
		{
			json v;
			if (!ParseObj(value.cbegin(), value.cend(), v))
			{
				return false;
			}
			js[name] = v;
		}
		else if(value[0] == '[')
		{
			json v;
			if (!ParseArray(value.cbegin(), value.cend(), v))
			{
				return false;
			}
			js[name] = v;
		}
		else 
		{
			json v;
			if (!ParseBaseValue(value, v))
			{
				return false;
			}
			L_DEBUG("ParseBaseValue result js=%s", Js2String(v).c_str());
			js[name] = v;
			L_DEBUG("parent result js=%s", Js2String(js).c_str());
		}
	}
	Log("error, ParseObj endless loop");
	return false;
}

bool SimpleCfg::ParseArray(CharIter start, CharIter end, json &js)
{
	if (start == end)
	{
		Log("error, empty cfg string");
		return false;
	}
	js.clear();

	//迭代每个元素变量
	CharIter cur = start+1; //指向当前待分析的字符串开始位置,跳过[
	for (uint32 n = 0; n < 50000; ++n)
	{
		//处理完成
		if (cur == end)
		{
			return true;
		}

		string value;
		if (!Find1stElment(cur, end, value))
		{
			return true;//没有元素了
		}
		L_DEBUG("find elment = %s", value.c_str());
		//到这里， cur 已经指向下一个成员字符串开头

		if (value[0] == '{')
		{
			json v;
			if (!ParseObj(value.cbegin(), value.cend(), v))
			{
				return false;
			}
			js.push_back(v);
		}
		else if (value[0] == '[')
		{
			json v;
			if (!ParseArray(value.cbegin(), value.cend(), v))
			{
				return false;
			}
			js.push_back(v);
		}
		else
		{
			json v;
			if (!ParseBaseValue(value, v))
			{
				return false;
			}
			js.push_back(v);
		}
	}
	Log("error, ParseObj endless loop");
	return false;
}

bool SimpleCfg::CheckAndJumpSplit(CharIter &cur, CharIter end)
{
	if (cur == end)
	{
		return false;
	}
	//跳过分隔符
	bool isFind = false;
	for (uint32 n = 0; n < 50000; ++n)
	{
		if (IsSplitChar(*cur))
		{
			isFind = true;
			cur++; 	
			if (cur == end)
			{
				break;
			}
		}
		else
		{
			break;
		}
		if (n >= 50000 - 1)
		{
			L_DEBUG("error, CheckAndJumpSplit endless loop ");
			return false;
		}
	}
	return isFind;
}

void SimpleCfg::CheckAndJumpSplitComment(CharIter &cur, CharIter end)
{
	if (cur == end)
	{
		return;
	}
	//auto start = cur;
	for (uint32 n = 0; n < 50000; ++n)
	{
		if (CheckAndJumpSplit(cur, end))
		{
			continue;
		}
		if (CheckAndJumpComment(cur, end))
		{
			continue;
		}
		break;
		if (n >= 50000 - 1)
		{
			L_DEBUG("error, CheckAndJumpSplitComment endless loop  %d [%s] [%s]", n, &*start, &*cur);
			return;
		}
	}
}

bool SimpleCfg::CheckAndJumpComment(CharIter &cur, CharIter end)
{
	if (cur == end)
	{
		return false;
	}
	//cur 跳过注释 "//"
	if ('/' == *cur && cur != end)
	{
		if ('/' != *(cur))
		{
			return false;
		}

		//有注释 "//"
		L_DEBUG("find comment //");
		CharIter it = std::find(cur, end, '\n');
		if (it == end)
		{
			cur = end;
			L_DEBUG("jump to =%s", &*cur);
			return true;
		}
		cur = it + 1;
		L_DEBUG("jump to =%s", &*cur);
		return true;
	}
	return false;
}

//解析基础值，
//@js [out] 值赋值给js=xx
bool SimpleCfg::ParseBaseValue(const string &value, json &js)
{
	if (value.empty())
	{
		Log("error para in ParseBaseValue");
		return false;
	}
	try
	{
		L_DEBUG("ParseBaseValue %s", value.c_str());
		//大写库不支持，特殊处理
		{
			if (value == "TRUE"
				)
			{
				js = true;
				return true;
			}
			if (value == "FALSE")
			{
				js = false;
				return true;
			}
		}
		if (value.front() == '\'')
		{
			if (value.size() < 2)
			{
				Log("error para in ParseBaseValue. value str=%s", value.c_str());
				return false;
			}
			js = value[1];
			return true;
		}

		js = json::parse(value);
		return true;
	}
	catch (exception& e)
	{
		Log("parse json fail. exception %s", e.what());
	}
	catch (...)
	{
		Log("parse json fail");
	}
	return false;

	//字符串
	{
		CharIter it = std::find(value.cbegin(), value.cend(), '"');
		if (it != value.cend())
		{//is string
			++it;
			CharIter end_it = std::find(it, value.cend(), '"');
			if (end_it == value.cend())
			{
				Log("error string value when ParseBaseValue. [%s]", value.c_str());
				return false;
			}
			js["base"] = string(it, end_it);
			return true;
		}
	}

	//bool
	{
		if (value == "true" 
			|| value == "TRUE"
			)
		{
			js["base"] = true;
			return true;
		}
		if (value == "FALSE"
			|| value == "false"
			)
		{
			js["base"] = false;
			return true;
		}
	}

	//浮点数
	{
		json js = json::parse(value);

	}

	//数字
	{
		char *endptr = nullptr;
		int64 v = strtoll(value.c_str(), &endptr, 10);
		if (nullptr != endptr)
		{
			Log("error number value when ParseBaseValue. [%s]", value.c_str());
			return false;
		}
		js["base"] = v;
		return true;
	}

	Log("error, unknow value when ParseBaseValue. [%s]", value.c_str());
	return false;
}

bool SimpleCfg::IsBaseValue(const string &value)
{
	if (value.empty())
	{
		Log("error para in IsBaseValue");
		return true;
	}
	if (value[0] == '{' || value[0] == '[')
	{
		return false;
	}
	return true;
}

bool SimpleCfg::Find1stMemStr(CharIter &cur, const CharIter end , string &name, string &value)
{
	if (cur == end)
	{
		Log("error, FindMemStr input empty string");
		return false;
	}
	name.clear();
	value.clear();
	//const char *cur_p = nullptr; //调试用

	//找开始，cur指向成员名开始位置。
	{
		//cur_p = &*cur;
		bool isFind = false;
		for (uint32 n = 0; n < 50000; ++n)
		{
			//cur 跳过注释 "//"
			CheckAndJumpComment(cur, end);
			if (cur == end)
			{
				//Log("no member found. n=%d", n);
				return false;
			}

			if (
				(*cur >= 'a' && *cur <= 'z' )
				||
				(*cur >= 'A' && *cur <= 'Z')
				)
			{
				isFind = true;
				break;
			}

			++cur;
			if (cur == end)
			{
				//Log("error, 找开始，cur指向成员名开始位置失败。 %s", cur_p);
				return false;
			}
		}
		if (!isFind)
		{
			//Log("error, 找开始，cur指向成员名开始位置失败。 %s", cur_p);
			return false;
		}
	}


	//找名字结束
	{
		CharIter mem_name_start = cur;
		for (uint32 n = 0; n < 50000; ++n)
		{
			if (cur == end)
			{
				Log("error, 找名字结束失败。 ");
				return false;
			}				
			if (
				' ' == *cur
				|| '=' == *cur
				|| ':' == *cur
				|| '	' == *cur
				)
			{
				name.assign(mem_name_start, cur);
				break;
			}
			++cur;
		}
	}

	//跳过赋值符，空格cur指向成员值开始位置。
	{
		for (uint32 n = 0; n < 50000; ++n)
		{
			if (cur == end)
			{
				Log("error, 查找成员值失败");
				return false;
			}

			if (
				' ' == *cur
				|| '=' == *cur
				|| ':' == *cur
				|| '	' == *cur
				)
			{
				++cur;
				continue;
			}
			else
			{
				break;
			}
		}
	}

	//找对象值
	return FindValueStr(cur, end, value);
}

bool SimpleCfg::Find1stElment(CharIter &cur, const CharIter end, std::string &value)
{
	if (cur == end)
	{
		Log("error, FindMemStr input empty string");
		return false;
	}
	value.clear();

	//cur 跳过注释 "//"
	{
		CheckAndJumpComment(cur, end);
		if (cur == end)
		{
			L_DEBUG("no member found. ");
			return false;
		}
	}

	//跳过分隔符
	//CheckAndJumpSplit(cur, end);

	//CheckAndJumpComment(cur, end);

	CheckAndJumpSplitComment(cur, end);
	//找对象值
	if (cur == end)
	{
		L_DEBUG("FindValueStr end with comment");
		return false;
	}
	return FindValueStr(cur, end, value);
}

bool SimpleCfg::FindValueStr(CharIter &cur, const CharIter end, std::string &value)
{
	L_DEBUG("FindValueStr cur=%s", &*cur);
	if (cur == end)
	{
		Log("error para");
		return false;
	}
	CharIter value_start = cur;

	CheckAndJumpSplitComment(cur, end);
	//找对象值结束位置
	if (*cur == '{' || *cur == '[')
	{
		char end_c = 0;
		char start_c = 0;
		if (*cur == '{')
		{
			start_c = '{';
			end_c = '}';
		}
		else
		{
			start_c = '[';
			end_c = ']';
		}

		//找到结束符号
		uint32  start_symbol_num = 0;
		uint32  end_symbol_num = 0;
		auto IsEndSymbol = [&](const char c)
		{
			if (c == start_c)
			{
				start_symbol_num++;
			}
			else if (c == end_c)
			{
				end_symbol_num++;
			}

			if (start_symbol_num == end_symbol_num)
			{
				return true;
			}
			return false;
		};
		CharIter it = std::find_if(cur, end, IsEndSymbol);
		if (it == end)
		{
			Log("error, find {} or [] fail, is not match");
			return false;
		}
		value.assign(value_start, it + 1);
		L_DEBUG("it=%s", (const char *)&(*it));
		cur = it;
		cur++;
		L_DEBUG("cur=%s", (const char *)&(*cur));
		L_DEBUG("FindMemStr, find obj end value=%s, cur=%s", (const char *)&(*value_start), (const char *)&(*cur));
		return true;
	}
	else//找基本值结束位置
	{
		auto IsEndSymbol = [&](const char c)
		{
			if ('}' == c || ']' == c || '/' == c)
			{
				return true;
			}
			return IsSplitChar(c);
		};
		CharIter it = std::find_if(cur, end, IsEndSymbol);
		if (it == end)
		{
			//没结束符号
			value.assign(value_start, it);
			cur = end;
			return true;
		}
		if (value_start == it)//处理 “]”字符串
		{
			return false;
		}
		value.assign(value_start, it);
		cur = it + 1;
		return true;
	}
}

bool SimpleCfg::IsSplitChar(char c)
{
	if (
		' ' == c
		|| ',' == c
		|| ':' == c
		|| '	' == c
		|| '\n' == c
		)
	{
		return true;
	}
	return false;
}


