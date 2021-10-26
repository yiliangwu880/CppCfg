//测试 SimpleCfg 解析字符串

#include "unit_test.h"
#include <sstream>
#include "json.hpp"
#include <exception>
#include "SimpleCfg.h"

using namespace std;
using namespace SimpleCfgLog;
using json = nlohmann::json;


namespace {
	string Js2String(json &js)
	{
		std::stringstream ss;
		ss << js;

		return ss.str();
	}
	void PrintClass(const char *TEST_STR)
	{
		SimpleCfg js;
		if (!js.ParseStr(TEST_STR))
		{
			return;
		}
		string cpp_str;
		JsToCpp::Build(js, "MyCfg", cpp_str);

		string s1(cpp_str.begin(), cpp_str.begin() + cpp_str.size() / 2);
		string s2(cpp_str.begin() + cpp_str.size() / 2, cpp_str.end());

		UNIT_INFO("[%s", s1.c_str());//太长，分开2部分
		UNIT_INFO("%s]", s2.c_str());
	}
	void test_error()
	{
		EnableLog(false);
		//repeated member name
		{
			const char *TEST_STR = R"(
		a1=1
		a1="abc"
		
		)";
			SimpleCfg js;
			UNIT_ASSERT(!js.ParseStr(TEST_STR));
		}
		{
			const char *TEST_STR = R"(
		a1=1
		a2={a2=3,a2=4}
		
		)";
			SimpleCfg js;
			UNIT_ASSERT(!js.ParseStr(TEST_STR));
		}
		//数组元素不一致
		{
			const char *TEST_STR = R"(
		a1=1
		a2=[1,2,"a"]
		
		)";
			SimpleCfg js;
			UNIT_ASSERT(!js.ParseStr(TEST_STR));

			string cpp_str;
			JsToCpp::Build(js, "MyCfg", cpp_str);
		}
		//对象匿名不一致
		{
			const char *TEST_STR = R"(
		a1=1
		{bb1=3}
		
		)"; 
			//EnableLog(true);
			SimpleCfg js;
			UNIT_ASSERT(!js.ParseStr(TEST_STR));

			string cpp_str;
			JsToCpp::Build(js, "MyCfg", cpp_str);
		}
		EnableLog(true);
	}
	void TestBuildClassStr()
	{
		{
			const char *TEST_STR = R"(
		a1=1
		a2="abc"
		)";
			PrintClass(TEST_STR);
		} 
		{
			const char *TEST_STR = R"(
		a1=1
		a2="abc"
		a3={b1=2}
		)";
			PrintClass(TEST_STR);
		}
		{
			const char *TEST_STR = R"(
		name33 =1;
		a1=1
		a2="tt33"
		a22=[1,2]
		a3={b1=2, b2:{
					c1=true, 
					c2:['a','b']
					}
			}
		a31=[{b1=false},{b1=true}]
		
		)";
			PrintClass(TEST_STR);
		}
		//动态对象 json
		{
			const char *TEST_STR = R"(
		name33 =1;
		a1=999999999999999333
		a2="tt33"
		a22=[1,2]
		a3={b1=2, b2:{
					c1=true, 
					c2:['a','b']
					}
			}
		a31=[{b1=false},{b1=true}]
			dynamic dyn1 = 1
			dyn2 = { a="a",b=2, dynamic b1 = [1,2] }
		)";
			PrintClass(TEST_STR);
		}
		{
			const char *TEST_STR = R"(
			a1=999999999999999333
			a2="tt33"
			a22=[[1,2],1,2]
			a3={b1=2, b2:{
						c1=true, 
						c2:['a','b']
						}
				}
			a31=[{b1=false},{b1=true}]
			dynamic dyn1 = 1
			dynamic dyn2 = { a="a",b=2,  b1 = [1,2] }
		)";			
			EnableLog(false);
			SimpleCfg js;
			UNIT_ASSERT(!js.ParseStr(TEST_STR));

			string cpp_str;
			UNIT_ASSERT(!JsToCpp::Build(js, "MyCfg", cpp_str));
			EnableLog(true);
		}
		{
			const char *TEST_STR = R"(
			a1=999999999999999333
			a11=0x1
			a2 = 10 * 1 + 0x2
		)";
			PrintClass(TEST_STR);
		}



	}

}
UNITTEST(build_cpp)
{
	{
		const char *TEST_STR = R"(
ResLs=[
	{//Gold
		TalentRange=[0,0], //天赋范围,默认为 0,MaxTalentDef
		b = 3,
	},
]

		)";
		PrintClass(TEST_STR);
	}
	return;
	test_error();
	TestBuildClassStr();
}
