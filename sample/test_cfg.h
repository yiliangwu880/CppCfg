
//this file is build by CppCfg Tool
//don't modify by manual
#pragma once
#include <string>
#include <array>

struct test_cfg
{
	struct S_a3
	{
		struct S_b2
		{
			bool c1;
			std::array<int32,2> c2;
		};
		uint32 b1;
		S_b2 b2;
	};
	struct S_a31
	{
		bool b1;
	};


	////////////////////////define member list////////////////////////
	uint64 a1;
	std::string a2;
	std::array<uint32,2> a22;
	S_a3 a3;
	std::array<S_a31,2> a31;
	std::string dyn1;
	std::string dyn2;

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

			a1 = js["a1"];
			a2 = js["a2"];
			a22[0] = js["a22"][0];
			a22[1] = js["a22"][1];
			a3.b1 = js["a3"]["b1"];
			a3.b2.c1 = js["a3"]["b2"]["c1"];
			a3.b2.c2[0] = js["a3"]["b2"]["c2"][0];
			a3.b2.c2[1] = js["a3"]["b2"]["c2"][1];
			a31[0].b1 = js["a31"][0]["b1"];
			a31[1].b1 = js["a31"][1]["b1"];
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
