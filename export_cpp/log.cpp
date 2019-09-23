#include "stdafx.h"
#include "log.h"

namespace {

	static bool g_enable_log = true;
}
bool IsEnableLog()
{
	return g_enable_log;
}

void EnableLog(bool enable_log)
{
	g_enable_log = enable_log;
}

void Log(const char * pattern, ...)
{
	if (!g_enable_log)
	{
		return;
	}
	va_list vp;
	va_start(vp, pattern);
	{
		std::string s;
		s.append(pattern);
		s.append("\n");

		vprintf(s.c_str(), vp);
	}
	va_end(vp);
}
