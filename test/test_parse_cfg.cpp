// testPrj.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "unit_test.h"
#include <sstream>
#include "json.hpp"
#include <exception>
#include "SimpleCfg.h"

using namespace std;
using json = nlohmann::json;

namespace{
//
//const char *TEST_SIMPLE_CFG = R"(
////������˵�����ø�ʽ
//example:
//name1 = 3
//n2="abc13"
//n3=3.3
//n4=4;
//n5=true;
//array=[1,2,3];
//array_str=["1","a"]
////ÿ���������ṹһ������Ϊ��֧����������Ų�ͬ����
//array_str=[
//{a=1,b="a"},
//{a=11,b="aa"}
//]
//
////����Ϊע��
//j1 = json //����Ϊjson��ʽ
//{
//	"happy":true��
//	"pi" : 3.141
//	"obj":
//{
//
//	"happy":true��
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

	//���Ի�����ʽ
	void t1()
	{
		{
			const char *TEST_STR = R"(
				{
					n1 : 31
					n2 4
				)";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 31);
			UNIT_ASSERT(js["n2"] == 4);
		} 
		{
			const char *TEST_STR = "n1=3";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);
			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 3);
		}
		{
			const char *TEST_STR = "n1:3";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);
			UNIT_ASSERT(js["n1"] == 3);
		}
		{
			const char *TEST_STR = "n1:33 n2=true	n3=false	n4=FALSE n5=\"abc\" n6='a' ";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);
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
				{
					n1 : 31,
					n2 4
				)";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 31);
			UNIT_ASSERT(js["n2"] == 4);
		}
		{
			const char *TEST_STR = R"(
				{
					n1 : 3.1
					n2 4
				)";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 3.1);
			UNIT_ASSERT(js["n2"] == 4);
		}
	}

	void testObjArray()
	{

		{
			const char *TEST_STR = R"(
				{
					n1 : 31
					n2 4}
				)";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["n1"] == 31);
			UNIT_ASSERT(js["n2"] == 4);
		}
		{
			const char *TEST_STR = "{n1 : 31 n2 4}";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);

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
			js.ParseSimpleCfg(TEST_STR);

			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["o1"]["s1"] == 3);
			UNIT_ASSERT(js["o1"]["s2"] == "abc");
			UNIT_ASSERT(js["o1"]["s3"] == true);
			UNIT_ASSERT(js["n2"] == 4);
		}
		{//���Ƕ��
			const char *TEST_STR = R"(
				
					o1 :{s1 = 3, s2 = "abc", s3=true}
					a1 = {b1=1, b2={c1='a'}
							b3 4
							b4 =[1,2]
						}
		
				)";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);

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
		//����
		{
			const char *TEST_STR = "{a=[1,2, :true,		\"ab3c\", \'a\']}";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);
			UNIT_INFO("js=%s", js.c_str());
			UNIT_ASSERT(js["a"][0] == 1);
			UNIT_ASSERT(js["a"][1] == 2);
			UNIT_ASSERT(js["a"][2] == true);
			UNIT_ASSERT(js["a"][3] == "ab3c");
			UNIT_ASSERT(js["a"][4] == 'a');
		}
		{//���� Ƕ��
			const char *TEST_STR = R"(

a:[1,2, :true,	
	"ab3c", 'a'
	[11,22],
	{a=3.3,b:"abc"}
]
b=3
)";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);
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
		{//���������ʽ��ע��
			const char *TEST_STR = R"(

//ע��
a:[1,2, :true,	
	"ab3c", 'a'
	[11,22],
//abad, dkfj
	{
//abc
a=3.3, //ע��
b:"abc",;//dd
//ע��
}
]
b=3//a
//abb
)";
			SimpleCfg js;
			js.ParseSimpleCfg(TEST_STR);
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
}//namespace

//����json�ӿ����ʹ��
UNITTEST(parse_cfg)
{
	t1();
	testObjArray();
}
