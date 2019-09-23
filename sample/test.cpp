//²âÊÔ SimpleCfg ½âÎö×Ö·û´®

#include "unit_test.h"
#include <sstream>
#include "json.hpp"
#include <exception>

using namespace std;
using json = nlohmann::json;


namespace {
	string Js2String(json &js)
	{
		std::stringstream ss;
		ss << js;

		return ss.str();
	}

	void t1()
	{

	}
}

UNITTEST(build_cpp)
{
	t1();
}
