/*
从字符串解析出配置信息
存为json对象
分析非法格式（比如数组元素类型不一致）
*/

#pragma once
#include "stdafx.h"
#include "../com/SimpleCfg.h"


class JsToCpp
{
public:
	//根据json，生成cpp代码配置
	static bool Build(const nlohmann::json &js, const std::string &file_name, std::string &cpp_str);
	//return true表示合法的cfg对象
	static bool IsEnableObj(const nlohmann::json &js);

	//获取前缀名
	static void CfgFileNamePrefix(const std::string &file_name, std::string &prefix);
private:
	//根据json,生成成员函数
	static bool BuildMethod(const nlohmann::json &js, const std::string &file_name, std::string &cpp_str);
	//根据json,生成成员赋值列表
	//@js [in] 
	//@cpp_str [out]
	//@preName 前缀名， 比如： a.ab.c[0]
	//@preName_js 前缀名， 比如： a["ab"]["c"][0]
	//@int array_recursion_cnt  0表示非数组处理， >0表示数组处理，递归BuildAssignMem次数
	static bool BuildAssignMem(const nlohmann::json &js, std::string &cpp_str, const std::string &preName, const std::string &preName_js, int array_recursion_cnt= 0);
	//根据json, 生成子类和成员
	//比如:js格式： {n1=3,n2={a=3}}
	//生成 uint32 n1; struct n2{uint32 a;};
	//@tab [in/out]tab按键格式，嵌套缩进用
	static void BuildSubClassAndMember(const nlohmann::json &js, std::string &str, std::string &tab);
	//return true表示合法的数组
	static bool IsEnableArray(const nlohmann::json &js);
	static std::string GetBaseCppType(const nlohmann::json &js);
	//获取数组类型，元素必须为基础类型
	static std::string GetArrayCppType(const nlohmann::json &js);
};


//cpp 配置模板,参考用
struct MyCfg 
{
	//define struct type
	struct n1
	{
		uint32_t a;
		bool arr[3];
	};
	struct a3
	{
		struct n2
		{
			std::string n1;
		};

		uint32_t a;
		n2 n2;
	};

	////////////////////////define member list////////////////////////
	uint32_t a;
	n1 n1;
	a3 a3;

	////////////////////////method list////////////////////////
	template<typename Array>
	inline size_t ArrayLen(const Array &array)
	{
		return sizeof(array) / sizeof(array[0]);
	}
	//initialize or reload cfg content.
	bool Assign(const nlohmann::json &js)
	{
		try
		{
			a = js["a"];
			n1.a = js["n1"]["a"];
			{
				auto js_arr = js["n1"]["arr"];
				for (unsigned int i = 0; i < ArrayLen(n1.arr); ++i)
				{
					n1.arr[i] = js_arr[i];
				}
			}
			return true;
		}
		catch (...)
		{
			return false;//失败就是配置格式和cpp结构不一致，重新导出就能修复了。
		}
	}
};