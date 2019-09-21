
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
