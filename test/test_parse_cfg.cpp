//测试 SimpleCfg 解析字符串

#include "unit_test.h"
#include <sstream>
#include "json.hpp"
#include <exception>
#include "SimpleCfg.h"

using namespace std;
using json = nlohmann::json;

using namespace SimpleCfgLog;
namespace{
//
//const char *TEST_SIMPLE_CFG = R"(
////用例子说明配置格式
//example:
//name1 = 3
//n2="abc13"
//n3=3.3
//n4=4;
//n5=true;
//array=[1,2,3];
//array_str=["1","a"]
////每个对象必须结构一样，因为不支持数组里面放不同类型
//array_str=[
//{a=1,b="a"},
//{a=11,b="aa"}
//]
//
////这里为注释
//j1 = json //这里为json格式
//{
//	"happy":true，
//	"pi" : 3.141
//	"obj":
//{
//
//	"happy":true，
//	"ar":[
//		1,2,3,{
//		"happy":true
//		}],
//	"pi" : 3
//}
//}
//)";

	string Js2String(json &js)
	{
		std::stringstream ss;
		ss << js;

		return ss.str();
	}

	//测试基本格式
	void t1()
	{
		{
			const char *TEST_STR = R"(
				
					n1 : 31
					n2 4
				)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 31);
			UNIT_ASSERT(js["n2"] == 4);
		} 
		{
			const char *TEST_STR = "n1=3";
			SimpleCfg js;
			js.ParseStr(TEST_STR);
			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 3);
		}
		{
			const char *TEST_STR = "n1:3";
			SimpleCfg js;
			js.ParseStr(TEST_STR);
			UNIT_ASSERT(js["n1"] == 3);
		}
		{
			const char *TEST_STR = "n1:33; n2=true;	n3=false;	n4=FALSE; n5=\"abc\"; n6='a' ";
			SimpleCfg js;
			js.ParseStr(TEST_STR);
			UNIT_ASSERT(js["n1"] == 33);
			UNIT_ASSERT(js["n2"] == true);
			UNIT_ASSERT(js["n3"] == false);
			UNIT_ASSERT(js["n4"] == false);
			UNIT_ASSERT(js["n5"] == "abc");
			UNIT_ASSERT(js["n6"] == 'a');
			UNIT_INFO("js=%s", js.c_str());
		}
		{
			const char *TEST_STR = R"(
				
					n1 : 31,
					n2 4
				)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 31);
			UNIT_ASSERT(js["n2"] == 4);
		}
		{
			const char *TEST_STR = R"(
				
					n1 : 3.1
					n2 4
				)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 3.1);
			UNIT_ASSERT(js["n2"] == 4);
		}
	}

	void testObjArray()
	{

		{
			const char *TEST_STR = R"(
					n1 : 331
					n2= 4 
				)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 331);
			UNIT_ASSERT(js["n2"] == 4);
		}
		{
			const char *TEST_STR = "n1 : 31; n2 4";
			SimpleCfg js;
			js.ParseStr(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 31);
			UNIT_ASSERT(js["n2"] == 4);
		} 
		{
			const char *TEST_STR = R"(
				
					o1 :{s1 = 3, s2 = "abc", s3=true}
					n2 4

				)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["o1"]["s1"] == 3);
			UNIT_ASSERT(js["o1"]["s2"] == "abc");
			UNIT_ASSERT(js["o1"]["s3"] == true);
			UNIT_ASSERT(js["n2"] == 4);
		}
		{//多层嵌套
			const char *TEST_STR = R"(
				
					o1 :{s1 = 3, s2 = "abc", s3=true}
					a1 = {b1=1, b2={c1='a'}
							b3 4
							b4 =[1,2]
						}
		
				)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["o1"]["s1"] == 3);
			UNIT_ASSERT(js["o1"]["s2"] == "abc");
			UNIT_ASSERT(js["o1"]["s3"] == true);

			UNIT_ASSERT(js["a1"]["b1"] == 1);
			UNIT_ASSERT(js["a1"]["b2"]["c1"] == 'a');
			UNIT_ASSERT(js["a1"]["b3"] == 4);
			UNIT_ASSERT(js["a1"]["b4"][0] == 1);
			UNIT_ASSERT(js["a1"]["b4"][1] == 2);
		}
		//数组
		{
			const char *TEST_STR = "a=[1,2, :true,		\"ab3c\", \'a\']";
			SimpleCfg js;
			js.ParseStr(TEST_STR);
			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["a"][0] == 1);
			UNIT_ASSERT(js["a"][1] == 2);
			UNIT_ASSERT(js["a"][2] == true);
			UNIT_ASSERT(js["a"][3] == "ab3c");
			UNIT_ASSERT(js["a"][4] == 'a');
		}
		{//数组 嵌套
			const char *TEST_STR = R"(

a:[1,2, :true,	
	"ab3c", 'a'
	[11,22, "abb"],
	{a=3.3,b:"abc"}
]
b=3
)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);
			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["b"] == 3);
			UNIT_ASSERT(js["a"][0] == 1);
			UNIT_ASSERT(js["a"][1] == 2);
			UNIT_ASSERT(js["a"][2] == true);
			UNIT_ASSERT(js["a"][3] == "ab3c");
			UNIT_ASSERT(js["a"][4] == 'a');
			UNIT_ASSERT(js["a"][5][0] == 11);
			UNIT_ASSERT(js["a"][5][1] == 22);
			UNIT_ASSERT(js["a"][5][2] == "abb");
			UNIT_ASSERT(js["a"][6]["a"] == 3.3);
			UNIT_ASSERT(js["a"][6]["b"] == "abc");
		}
		{//测试随意格式，注释
			const char *TEST_STR = R"(

//注释
a:[1,2, :true,	
	"ab3c", 'a'
	[11,22],
//abad, dkfj
	{
//abc
a=3.3, //注释
b:"abc",;//dd
//注释
}
]
b=3//a
//abb
)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);
			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["b"] == 3);
			UNIT_ASSERT(js["a"][0] == 1);
			UNIT_ASSERT(js["a"][1] == 2);
			UNIT_ASSERT(js["a"][2] == true);
			UNIT_ASSERT(js["a"][3] == "ab3c");
			UNIT_ASSERT(js["a"][4] == 'a');
			UNIT_ASSERT(js["a"][5][0] == 11);
			UNIT_ASSERT(js["a"][5][1] == 22);
			UNIT_ASSERT(js["a"][6]["a"] == 3.3);
			UNIT_ASSERT(js["a"][6]["b"] == "abc");
		}

	}

	void testDynamic()
	{
		{//测试随意格式，注释
			const char *TEST_STR = R"(
//注释
a:[1,2, :true,	
	"ab3c", 'a'
	[11,22],
//abad, dkfj
	{
//abc
a=3.3, //注释
b:"abc",;//dd
//注释
}
]
b=3//a
//abb
	dynamic dyn1 = 1
	dynamic dyn2 = { a="a",b=2, dynamic b1 = [1,true] }
	dynamic dyn3 = ["a", 2]
	dynamic dyn4 = ["a",  { b = 1,b2=1,c=[1,2] }]
)";

	/*	

*/
			SimpleCfg js;
			js.ParseStr(TEST_STR);
			UNIT_INFO("js=%s", js.c_str());

			UNIT_ASSERT(js["dyn1"] == 1);
			UNIT_ASSERT(js["dynamic"][0] == "dyn1");
			UNIT_ASSERT(js["dynamic"][1] == "dyn2");
			UNIT_ASSERT(js["dynamic"][2] == "dyn3");
			UNIT_ASSERT(js["dynamic"][3] == "dyn4");

			UNIT_ASSERT(js["dyn4"][1]["c"][0] == 1);

			UNIT_ASSERT(js["dyn2"]["dynamic"][0] == "b1");



			UNIT_ASSERT(js["b"] == 3);
			UNIT_ASSERT(js["a"][0] == 1);
			UNIT_ASSERT(js["a"][1] == 2);
			UNIT_ASSERT(js["a"][2] == true);
			UNIT_ASSERT(js["a"][3] == "ab3c");
			UNIT_ASSERT(js["a"][4] == 'a');
			UNIT_ASSERT(js["a"][5][0] == 11);
			UNIT_ASSERT(js["a"][5][1] == 22);
			UNIT_ASSERT(js["a"][6]["a"] == 3.3);
			UNIT_ASSERT(js["a"][6]["b"] == "abc");
		}
	}
	void testCalc()
	{

		{
			const char *TEST_STR = R"(
					n1 : 331
					n2= 2 + 3 + 3+2
n3=0x10 * 1 / 0x2
				)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 331);
			UNIT_ASSERT(js["n2"] == 10);
		}
		{
			const char *TEST_STR = R"(
					n1 : -331
					n2 = [ -0x3f, 0xa]
				)";
			SimpleCfg js;
			js.ParseStr(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == -331);
			UNIT_ASSERT(js["n2"][0] == -0x3f);
			UNIT_ASSERT(js["n2"][1] == 0xa);
		}
	}
}//namespace

//测试json接口如何使用
UNITTEST(parse_cfg)
{
	t1();
	testObjArray();
	testDynamic();
	testCalc();

}
