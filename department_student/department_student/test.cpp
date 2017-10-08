#include <iostream>
#include <string>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#define N 20
#define M 300
using namespace std;
using namespace rapidjson;  //引入rapidjson命名空
struct Department{
	string department_no;
	int member_limit;
	string event_schedules[20];
	string tags[20];
	int accept_members;
}department[N];

struct Student{
	string student_no;
	string applications_department[5];
	string free_time[20];
	string tags[20];
	int join_departments;
}student[M];
//写json文件
void json_write()
{
	Document doc;
	doc.SetObject();
	Document::AllocatorType &allocator = doc.GetAllocator(); //获取分配器
	//1.添加字符串对象
	doc.AddMember("author", "tashaxing", allocator);
	//2.添加数组对象
	Value array1(kArrayType);
	for (int i = 0; i<3; i++)
	{
		Value int_object(kObjectType);
		int_object.SetInt(i);
		array1.PushBack(int_object, allocator);
	}
	doc.AddMember("number", array1, allocator);
	//3.添加复合对象
	Value object(kObjectType);
	object.AddMember("language1", "C++", allocator);
	object.AddMember("language2", "java", allocator);
	doc.AddMember("language", object, allocator);
	//4.添加对象数组和复合对象的组合
	Value array2(kArrayType);
	Value object1(kObjectType);
	object1.AddMember("hobby", "drawing", allocator);
	array2.PushBack(object1, allocator);
	Value object2(kObjectType);
	object2.AddMember("height", 1.71, allocator);
	array2.PushBack(object2, allocator);
	doc.AddMember("information", array2, allocator);
	StringBuffer buffer;
	PrettyWriter<StringBuffer> pretty_writer(buffer);  //PrettyWriter是格式化的json，如果是Writer则是换行空格压缩后的json
	doc.Accept(pretty_writer);
	//打印到屏幕
	cout << "the json output:" << endl;
	cout << buffer.GetString() << endl;
	//输出到文件
	ofstream fout;
	fout.open("test.txt");    //可以使绝对和相对路径，用\\隔开目录，test, test.json, test.txt 都行，不局限于文件格式后缀，只要是文本文档
	fout << buffer.GetString();
	fout.close();
}

//读json文件
void json_read()
{
	ifstream in;
	in.open("input_data.txt");
	string str;
	string str_in = "";
	while (getline(in, str))    //一行一行地读到字符串str_in中
	{
		str_in = str_in + str + '\n';
	}

	//解析并打印出来
	Document document;
	document.Parse<0>(str_in.c_str());

	Value &students = document["students"];
	for (unsigned int i = 0; i < students.Size(); i++)
	{
		Value &stu = students[i];
		Value &free_time= stu["free_time"];
		if (free_time.IsArray())
		{
			for (unsigned int j = 0; j < free_time.Size(); j++)
				student[i].free_time [j] = free_time[j].GetString();
		}
		Value &student_no = stu["student_no"];
		student[i].student_no= student_no.GetString();
		Value &applications_department = stu["applications_department"];
		if (applications_department.IsArray())
		{
			for (unsigned int j = 0; j < applications_department.Size(); j++)
				student[i].applications_department[j] = applications_department[j].GetString();
		}
		Value &tags = stu["tags"];
		if (tags.IsArray())
		{
			for (unsigned int j = 0; j < tags.Size(); j++)
				student[i].tags[j] = tags[j].GetString();
		}
	}
	document.Parse<0>(str_in.c_str());
	Value &departments = document["departments"];
	for (unsigned int i = 0; i < departments.Size(); i++)
	{
		Value &dep = departments[i];
		Value &event_schedules = dep["event_schedules"];
		if (event_schedules.IsArray())
		{
			for (unsigned int j = 0; j < event_schedules.Size(); j++)
				department[i].event_schedules[j] = event_schedules[j].GetString();
		}
		Value &member_limit = dep["member_limit"];
		department[i].member_limit = member_limit.GetInt();
		Value &department_no = dep["department_no"];
		department[i].department_no = department_no.GetString();
		Value &tags = dep["tags"];
		if (tags.IsArray())
		{
			for (unsigned int j = 0; j < tags.Size(); j++)
				department[i].tags[j] = tags[j].GetString();
		}
	}
}
int main(int argc, char **argv)
{
	//写、读 测试
	//json_write();
	json_read();
	ofstream fout;
	fout.open("output_data.txt");
	int len;
	for (int i = 0; i < M; i++)
	{
		len = sizeof(student[i].free_time) / sizeof(student[i].free_time[0]);
		for (int j = 0; j <len; j++)
			fout << student[i].free_time[j] << endl;
		fout << student[i].student_no<<endl;
		len = sizeof(student[i].applications_department) / sizeof(student[i].applications_department[0]);
		for (int j = 0; j <len ;j++)
			fout << student[i].applications_department[j] << endl;
		len = sizeof(student[i].tags) / sizeof(student[i].tags[0]);
		for (int j = 0; j <len; j++)
			fout << student[i].tags[j] << endl;
	}
	system("pause");
	return 0;
}