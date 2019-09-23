#include "stdafx.h"
#include "version.h"
#include "SimpleCfg.h"
#include "log.h"

using json = nlohmann::json;
using namespace std;




//管理输出cpp
class CppMgr
{
public:
	CppMgr();

	void AddLine(const char * pattern, ...)
	{
		va_list vp;
		va_start(vp, pattern);
		{
			string s;
			s.append(pattern);
			s.append("\n");

			char out_str[1000];
			vsnprintf(out_str, sizeof(out_str), s.c_str(), vp);

			m_context.append(out_str);
		}
		va_end(vp);
	}

	void BuildFile(const string &file_name)
	{
		ofstream ofs(file_name.c_str(), ios::ate | ios::out);
		ofs << m_context;
		ofs.close();
	}

private:
	string m_context;
};



const char *TEST_SIMPLE_CFG = R"(
//用例子说明配置格式
example:
name1 = 3
n2="abc13"
n3=3.3
n4=4;
n5=true;
array=[1,2,3];
array_str=["1","a"]
//每个对象必须结构一样，因为不支持数组里面放不同类型
array_str=[
{a=1,b="a"},
{a=11,b="aa"}
]

//这里为注释
j1 = json //这里为json格式
{
	"happy":true，
	"pi" : 3.141
	"obj":
{

	"happy":true，
	"ar":[
		1,2,3,{
		"happy":true
		}],
	"pi" : 3
}
}
)";

const char *TEST_JS = R"(
{
	"happy":true，
	"pi" : 3.141
	"obj":
{

	"happy":true，
	"ar":[
		1,2,3,{
		"happy":true
		}],
	"pi" : 3
}
}
)";


void ReadCfg()
{
}


void Str2File(const string &file_name, const string &content)
{
	ofstream ofs(file_name, ios::ate | ios::out);
	ofs << content;
	ofs.close();
}

//获取前缀名
void CfgFileNamePrefix(const string &file_name, string &prefix)
{
	auto it = std::find(file_name.begin(), file_name.end(), '.');

	prefix.assign(file_name.begin(), it);
}


void BuildCppFile(const char *fileName)
{
	Log("build file: %s", fileName);
	std::ifstream t(fileName);
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	t.close();                    // close file handle


	SimpleCfg js;
	if (!js.ParseSimpleCfg(str))
	{
		Log("parse file fail.");
		return;
	}

	//cfg to cpp file
	string cpp_str;
	//if (!js.BuildCppCfg(cpp_str))
	//{
	//	Log("parse file fail. error struct");
	//	return;
	//}

	//build file
	string prefixName;
	CfgFileNamePrefix(fileName, prefixName);
	Str2File(prefixName + ".h", cpp_str);
}

int main(int argc, char* argv[]) 
{
	for (int i=0; i<argc; ++i)
	{
		const char *fileName = argv[i];
		BuildCppFile(fileName);
	}
	return 0;
}

