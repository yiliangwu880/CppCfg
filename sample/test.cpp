//²âÊÔ SimpleCfg ½âÎö×Ö·û´®

#include "unit_test.h"
#include <sstream>
#include "stdafx.h"
#include "CC_test_cfg.h"

using namespace std;
using json = nlohmann::json;
using namespace SimpleCfgLog;

UNITTEST(build_cpp)
{
	test_cfg cfg;
	UNIT_ASSERT(cfg.LoadFile());

	UNIT_ASSERT(cfg.a1 == 999999999999999333);
	UNIT_ASSERT(cfg.a2 == "tt33");
	UNIT_ASSERT(cfg.a22[0] == 1);
	UNIT_ASSERT(cfg.a22[1] == 2);

	UNIT_ASSERT(cfg.a3.b1 == 2);
	UNIT_ASSERT(cfg.a3.b2.c1 == true);
	UNIT_ASSERT(cfg.a3.b2.c2[0] == 'a');
	UNIT_ASSERT(cfg.a3.b2.c2[1] == 'b');


	UNIT_ASSERT(cfg.a31[0].b1 == false);
	UNIT_ASSERT(cfg.a31[1].b1 == true);

	{
		json dyn = json::parse(cfg.dyn1);

		UNIT_ASSERT(dyn == 1);
	}
	{
		json dyn = json::parse(cfg.dyn2);

		UNIT_ASSERT(dyn["a"] == "a");
		UNIT_ASSERT(dyn["b"] == 2);
		UNIT_ASSERT(dyn["b1"][0] == 1);
		UNIT_ASSERT(dyn["b1"][1] == 2);
	}
	UNIT_INFO("test ok");
}
