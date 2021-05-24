// testPrj.cpp : 定义控制台应用程序的入口点。
//

#include "unit_test.h"
#include <sstream>
#include "json.hpp"
#include <exception>

using namespace std;
using json = nlohmann::json;
using namespace SimpleCfgLog;


int main(int argc, char* argv[])
{
	EnableLog(true);
	UnitTestMgr::Obj().Start();
	return 0;
}


