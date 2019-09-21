#include "stdafx.h"
#include "log.h"

void Log(const char * pattern, ...)
{
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
