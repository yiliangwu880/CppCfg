#pragma once

#include "stdafx.h"
#include "version.h"

#define PRINT_DEBUG_LOG  //打印调试日志



//enum class CfgValueType
//{
//	BOOL,
//	UINT32,
//	UINT64,
//	DOUBLE,
//	INT32,
//	INT64,
//	STR,
//	OBJ,
//	ARRAY,
//};

//打印日志
void Log(const char * pattern, ...);
void EnableLog(bool enable_log);

#ifdef PRINT_DEBUG_LOG
#define L_DEBUG(x, ...)   Log(x, ##__VA_ARGS__)
#else
#define L_DEBUG(x, ...)  
#endif

