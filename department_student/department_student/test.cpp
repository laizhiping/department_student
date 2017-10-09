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
	int accept_members[300];
	int accept_members_time[300];
	int member_num = 0;
}department[N];

struct Student{
	string student_no;
	string applications_department[5];
	string free_time[20];
	string tags[20];
	int join_departments = 0;
}student[M];

struct Admitted {
	string member[15];
	string department_no;
}admitted[20];

string unlucky_student[300];
string unlucky_department[20];

//读json文件
void jsonRead()//读json文件
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
		Value &free_time = stu["free_time"];
		if (free_time.IsArray())
		{
			for (unsigned int j = 0; j < free_time.Size(); j++)
				student[i].free_time[j] = free_time[j].GetString();
		}
		Value &student_no = stu["student_no"];
		student[i].student_no = student_no.GetString();
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

void jsonWrite()
{
	Document doc;
	doc.SetObject();
	Document::AllocatorType &allocator = doc.GetAllocator(); //获取分配器
	Value temp_string;
	Value unlucky_stu(kArrayType);
	int t = 0;
	while (unlucky_student[t] != ""&&t<300)
	{
		temp_string.SetString(StringRef(unlucky_student[t].c_str()));
		unlucky_stu.PushBack(temp_string, allocator);
		t++;
	}
	doc.AddMember("unlucky_student", unlucky_stu, allocator);
	Value lucky_dep(kArrayType);
	for (int i = 0; i < 20; i++)
	{
		Value obj(kObjectType);
		Value member_stu(kArrayType);
		t = 0;
		int j;
		for (j = 0; j < 20; j++)
		{
			if (department[j].department_no == admitted[i].department_no)
				break;
		}
		while (admitted[i].member[t] != ""&&t<department[j].member_limit)
		{
			temp_string.SetString(StringRef(admitted[i].member[t].c_str()));
			member_stu.PushBack(temp_string, allocator);
			t++;
		}
		obj.AddMember("member", member_stu, allocator);
		temp_string.SetString(StringRef(admitted[i].department_no.c_str()));
		obj.AddMember("department_no", temp_string, allocator);
		lucky_dep.PushBack(obj, allocator);
	}
	doc.AddMember("admitted", lucky_dep, allocator);
	Value unlucky_dep(kArrayType);
	t = 0;
	while (unlucky_department[t] != ""&&t<20)
	{
		temp_string.SetString(StringRef(unlucky_department[t].c_str()));
		unlucky_stu.PushBack(temp_string, allocator);
		t++;
	}
	doc.AddMember("unlucky_department", unlucky_dep, allocator);
	StringBuffer buffer;
	PrettyWriter<StringBuffer> pretty_writer(buffer);  //PrettyWriter是格式化的json，如果是Writer则是换行空格压缩后的json
	doc.Accept(pretty_writer);
	//输出到文件
	ofstream ffout;
	ffout.open("output_data.txt");
	ffout << buffer.GetString();
	ffout.close();
}

void First_Choose()
{
	int i, j, m;
	for (i = 0; i < 300; i++)//每个学生
	{
		for (j = 0; j < 5; j++)//每个学生可以报 5 个部门
		{
			if (student[i].applications_department[j] != " ")//20 个部门情况
			{
				if (student[i].applications_department[j] == "D001")
				{
					m = department[0].member_num;
					department[0].accept_members[m] = i;
					department[0].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D002")
				{
					department[1].accept_members[department[1].member_num] = i;
					department[1].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D003")
				{
					department[2].accept_members[department[2].member_num] = i;
					department[2].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D004")
				{
					department[3].accept_members[department[3].member_num] = i;
					department[3].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D005")
				{
					department[4].accept_members[department[4].member_num] = i;
					department[4].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D006")
				{
					department[5].accept_members[department[5].member_num] = i;
					department[5].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D007")
				{
					department[6].accept_members[department[6].member_num] = i;
					department[6].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D008")
				{
					department[7].accept_members[department[7].member_num] = i;
					department[7].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D009")
				{
					department[8].accept_members[department[8].member_num] = i;
					department[8].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D010")
				{
					department[9].accept_members[department[9].member_num] = i;
					department[9].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D011")
				{
					department[10].accept_members[department[10].member_num] = i;
					department[10].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D012")
				{
					department[11].accept_members[department[11].member_num] = i;
					department[11].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D013")
				{
					department[12].accept_members[department[12].member_num] = i;
					department[12].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D014")
				{
					department[13].accept_members[department[13].member_num] = i;
					department[13].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D015")
				{
					department[14].accept_members[department[14].member_num] = i;
					department[14].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D016")
				{
					department[15].accept_members[department[15].member_num] = i;
					department[15].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D017")
				{
					department[16].accept_members[department[16].member_num] = i;
					department[16].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D018")
				{
					department[17].accept_members[department[17].member_num] = i;
					department[17].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D019")
				{
					department[18].accept_members[department[18].member_num] = i;
					department[18].member_num += 1;
				}
				else if (student[i].applications_department[j] == "D020")
				{
					department[19].accept_members[department[19].member_num] = i;
					department[19].member_num += 1;
				}
			}
		}
	}
}

int Count_Time(string a, string b)
{
	/*数据定义部分*/
	//char time[16];
	string week1;
	string week2;
	int start_h1;
	int start_m1;
	int end_h1;
	int end_m1;
	int start_h2;
	int start_m2;
	int end_h2;
	int end_m2;
	int start_h;
	int start_m;
	int end_h;
	int end_m;
	string temp;
	int m11, m12, m13, m21, m22, m23;

	/*功能部分*/
	int i, j, m, h;
	int time_lengh;
	i = a.find(".");
	week1 = a.substr(0, i);
	j = b.find(".");
	week2 = b.substr(0, j);
	if (week1 != week2)
	{
		time_lengh = 0;
	}
	else//判断时间的重复
	{
		//student
		m11 = a.find(":");
		m12 = a.find("~");
		m13 = a.rfind(":");
		m21 = b.find(":");
		m22 = b.find("~");
		m23 = b.rfind(":");
		temp = a.substr(i + 1, m11 - i - 1);
		start_h1 = stoi(temp);
		temp = a.substr(m11 + 1, m12 - m11 - 1);
		start_m1 = stoi(temp);
		temp = a.substr(m12 + 1, m13 - m12 - 1);
		end_h1 = stoi(temp);
		temp = a.substr(m13 + 1);
		end_m1 = stoi(temp);
		//department
		temp = b.substr(j + 1, m21 - i - 1);
		start_h2 = stoi(temp);
		temp = b.substr(m21 + 1, m22 - m21 - 1);
		start_m2 = stoi(temp);
		temp = b.substr(m22 + 1, m23 - m22 - 1);
		end_h2 = stoi(temp);
		temp = b.substr(m23 + 1);
		end_m2 = stoi(temp);
		//count
		if (start_h1 > start_h2)
		{
			start_h = start_h1;
			start_m = start_m1;
		}
		else if (start_h1 < start_h2)
		{
			start_h = start_h2;
			start_m = start_m2;
		}
		else if (start_h1 == start_h2)
		{
			if (start_m1 > start_m2)
			{
				start_h = start_h1;
				start_m = start_m1;
			}
			else
			{
				start_h = start_h2;
				start_m = start_m2;
			}
		}
		if (end_h1 < end_h2)
		{
			end_h = end_h1;
			end_m = end_m1;
		}
		else if (end_h1 > end_h2)
		{
			end_h = end_h2;
			end_m = end_m2;
		}
		else if (end_h1 == end_h2)
		{
			if (end_h1 < end_h2)
			{
				end_h = end_h1;
				end_m = end_m1;
			}
			else
			{
				end_h = end_h2;
				end_m = end_m2;
			}
		}

		h = end_h - start_h;
		m = end_m - start_m;
		time_lengh = 60 * h + m;
		if (time_lengh <= 0)
		{
			time_lengh = 0;
		}
	}
	return time_lengh;
}

void sort_department(int a)
{
	int p, q, temp1, temp2;
	for (q = 0; q < 299; q++)
	{
		for (p = 0; p < 299 - q; p++)
		{
			if (department[a].accept_members_time[p] < department[a].accept_members_time[p + 1])
			{
				temp1 = department[a].accept_members[p];
				department[a].accept_members[p] = department[a].accept_members[p + 1];
				department[a].accept_members[p + 1] = temp1;
				temp2 = department[a].accept_members_time[p];
				department[a].accept_members_time[p] = department[a].accept_members_time[p + 1];
				department[a].accept_members_time[p + 1] = temp2;
			}
		}
	}
}

void Second_Choose()
{
	//char time;
	int i, j, m, n;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 300; j++)
		{
			for (m = 0; m < 50; m++)
			{
				if (department[i].event_schedules[m].empty())
				{
					break;
				}
				else
				{
					for (n = 0; n < 50; n++)
					{
						if (student[department[i].accept_members[j]].free_time[n].empty())
						{
							break;;
						}
						else
						{
							department[i].accept_members_time[j] += Count_Time(student[department[i].accept_members[j]].free_time[n], department[i].event_schedules[m]);
						}
					}
				}

			}
		}
	}
	//排序
	for (i = 0; i < 20; i++)
	{
		sort_department(i);
	}
	//选择

}

void Choose()
{
	int i, j;
	for (i = 0; i < 20; i++)
	{
		admitted[i].department_no = department[i].department_no;
		for (j = 0; j < department[i].member_limit; j++)
		{
			if (department[i].accept_members[j] == 0)
			{
				break;
			}
			else
			{
				admitted[i].member[j] = student[department[i].accept_members[j]].student_no;
				department[i].member_num++;
				student[department[i].accept_members[j]].join_departments++;
			}
		}
	}

	j = 0;
	for (i = 0; i < 300; i++)
	{
		if (student[i].join_departments == 0)
		{
			unlucky_student[j] = student[i].student_no;
			j++;
		}
	}

	j = 0;
	for (i = 0; i < 20; i++)
	{
		if (department[i].member_num == 0)
		{
			unlucky_department[j] = department[i].department_no;
			j++;
		}
	}
}

void Output_Test()
{
	int i, j;
	for (i = 0; i < 20; i++)
	{
		cout << i + 1 << ": ";
		for (j = 0; j < department[i].member_num; j++)
		{
			cout << department[i].accept_members[j] << " ";
		}
		cout << endl;
	}
}

void test()
{
	int i;
	for (i = 0; i < 20; i++)
	{
		if (unlucky_department[i].empty())
		{
			break;
		}
		else
		{
			cout << unlucky_department[i] << endl;
		}
	}
}

int main(int argc, char **argv)
{
	jsonRead();
	First_Choose();
	Second_Choose();
	//Output_Test();
	Choose();
	//test();
	/*	ofstream fout;
	fout.open("result.txt");
	int t = 0;
	fout << "unlucky_student:" << endl;
	while (unlucky_student[t++] != "")
		fout<<unlucky_student[t++]<<endl;
	*//*fout << "admitted:" << endl;
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			fout << admitted[i].member[j] << endl;
		} 
		fout<< admitted[i].department_no << endl;
	}
	/*fout << "unlucky_sudent:" << endl;
	for (int i = 0; i < 300;i++)
		fout << unlucky_student[i]<<endl;*/
	jsonWrite();
	return 0;
}