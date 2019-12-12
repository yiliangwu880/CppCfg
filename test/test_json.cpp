// ���� json.hpp ���ʹ��
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


//����json�ӿ����ʹ��
UNITTEST(Json)
{
	return;
	{//�������͸�ֵ
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
	{//�쳣
		break; //�����У������������㿴���ѡ�
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
		//����һ��������
		json js = json::array();
		json v = 3;
		js.push_back(1);
		js.push_back(v);
		//js=[1,3] 
		UNIT_INFO("js=%s", Js2String(js).c_str());
	}
	{
		json js;
		js["n"] = 1;
		json::iterator it = js.begin();
		UNIT_INFO("key=%s", it.key().c_str());
	}
	{
		json js = json::parse(
			R"(
				{
					"happy":1,
					"pi" : 33,
					"a1" : 22
					
				}
				)"
		);
		json dynArray = { "a1", 1 };
		for (auto& el : js.items())
		{
			UNIT_INFO("find  key=%s", el.key().c_str());
			for (auto& v : dynArray)
			if (v== el.key())
			{
				UNIT_INFO("find in dynArray. key=%s", el.key().c_str());
				continue; //���Զ�̬����
			}
		}
		for (auto& el : dynArray.items())
		{
			UNIT_INFO("find  key=%s", el.key().c_str());//key= "0", key= "1"
		}
	}

}
