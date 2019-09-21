// testPrj.cpp : 定义控制台应用程序的入口点。
//

#include "unit_test.h"
#include <sstream>
#include "json.hpp"
#include <exception>

using namespace std;
using json = nlohmann::json;



string Js2String(json &js)
{
	std::stringstream ss;
	ss << js;

	return ss.str();
}


//测试json接口如何使用
UNITTEST(Json)
{
	return;
	{//基础类型赋值
		json js = json::parse("333443.31");
		
		UNIT_INFO("js=%s type=%s", Js2String(js).c_str(), js.type_name());
		 js = json::parse("false");

		UNIT_INFO("js=%s type=%s", Js2String(js).c_str(), js.type_name());
		 js = json::parse("true");

		 UNIT_INFO("js=%s type=%s", Js2String(js).c_str(), js.type_name());
		 js = json::parse("\"true\"");

		 UNIT_INFO("js=%s type=%s", Js2String(js).c_str(), js.type_name());
		//js.is_number_float();
	}
	do 
	{//异常
		break; //不运行，给代码例子你看而已。
		try
		{
			json js = json::parse(
				R"(
				{
					"happy":1,aa
					"pi" : 33
				}
				)"
			);
		}
		catch (exception& e)
		{
			UNIT_INFO("parse json fail. exception %s", e.what());
		}
		catch (...)
		{
			UNIT_INFO("parse json fail");
		}
	} while (false);
	{
		json js;
		json v = 3;
		js["n1"]=v;
		//js={"n1":3} 
		UNIT_INFO("js=%s", Js2String(js).c_str());
	}
	{
		json js = { 1,"a" };
		json v = 3;
		js.push_back(v);
		//js=[1,"a",3] 
		UNIT_INFO("js=%s", Js2String(js).c_str());
	}
	{
		//创建一个空数组
		json js = json::array();
		json v = 3;		js.push_back(1);
		js.push_back(v);
		//js=[1,3] 
		UNIT_INFO("js=%s", Js2String(js).c_str());
	} 
}
