
//this file is build by CppCfg Tool
//don't modify by manual
#pragma once
#include <string>
#include <array>
#include "SimpleCfg.h"

struct test_cfg
{
	struct S_a3
	{
		struct S_b2
		{
			bool c1;
			std::vector<int16_t> c2;
		};
		uint16_t b1;
		S_b2 b2;
	};
	struct S_a31
	{
		bool b1;
		uint16_t b2;
	};


	////////////////////////define member list////////////////////////
	uint64_t a1;
	std::string a2;
	std::vector<uint16_t> a22;
	S_a3 a3;
	std::vector<S_a31> a31;
	std::string dyn1;
	std::string dyn2;

	////////////////////////method list////////////////////////
	//load or reload cfg file .
	bool LoadFile(const char *file_name=nullptr)
	{
		//default load original cfg file name
		if (nullptr == file_name)
		{
			file_name = "test_cfg.txt";
		}
		SimpleCfg js;
		if (!js.ParseFile(file_name))
		{
			return false;
		}
		return Assign(js);
	}

private:
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

			a1 = js["a1"];
			a2 = js["a2"];
			for (size_t i = 0; i < js["a22"].size(); ++i)
			{
				a22.push_back(js["a22"][i]);
			}
			a3.b1 = js["a3"]["b1"];
			a3.b2.c1 = js["a3"]["b2"]["c1"];
			for (size_t i = 0; i < js["a3"]["b2"]["c2"].size(); ++i)
			{
				a3.b2.c2.push_back(js["a3"]["b2"]["c2"][i]);
			}
			for (size_t i = 0; i < js["a31"].size(); ++i)
			{
				a31.push_back({});
				a31[i].b1 = js["a31"][i]["b1"];
				a31[i].b2 = js["a31"][i]["b2"];
			}
			dyn1 = js["dyn1"].dump().c_str();
			dyn2 = js["dyn2"].dump().c_str();


			return true;
		}
		catch (...)
		{
			//if fail, pls check if your cfg fomart is legal.
			return false;
		}
	}
};
