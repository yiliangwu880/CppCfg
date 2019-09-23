//���� SimpleCfg �����ַ���

#include "unit_test.h"
#include <sstream>
#include "json.hpp"
#include <exception>
#include "SimpleCfg.h"
#include "log.h"

using namespace std;
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
		if (!js.ParseSimpleCfg(TEST_STR))
		{
			return;
		}
		string cpp_str;
		JsToCpp::Build(js, "MyCfg", cpp_str);
		UNIT_INFO("[%s]", cpp_str.c_str());
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
			UNIT_ASSERT(!js.ParseSimpleCfg(TEST_STR));
		}
		{
			const char *TEST_STR = R"(
		a1=1
		a2={a2=3,a2=4}
		
		)";
			SimpleCfg js;
			UNIT_ASSERT(!js.ParseSimpleCfg(TEST_STR));
		}
		//����Ԫ�ز�һ��
		{
			const char *TEST_STR = R"(
		a1=1
		a2=[1,2,"a"]
		
		)";
			SimpleCfg js;
			UNIT_ASSERT(!js.ParseSimpleCfg(TEST_STR));

			string cpp_str;
			JsToCpp::Build(js, "MyCfg", cpp_str);
		}
		//����������һ��
		{
			const char *TEST_STR = R"(
		a1=1
		{bb1=3}
		
		)"; 
			//EnableLog(true);
			SimpleCfg js;
			UNIT_ASSERT(!js.ParseSimpleCfg(TEST_STR));

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
		//��̬���� json
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
			UNIT_ASSERT(!js.ParseSimpleCfg(TEST_STR));

			string cpp_str;
			UNIT_ASSERT(!JsToCpp::Build(js, "MyCfg", cpp_str));
			EnableLog(true);
		}
		{
			const char *TEST_STR = R"(
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
			dynamic dyn2 = { a="a",b=2,  b1 = [1,2] }
		)";
			PrintClass(TEST_STR);
		}


	}

}
UNITTEST(build_cpp)
{
	test_error();
	TestBuildClassStr();
}