#include <iostream>//赖志平
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <string>
#include <set>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#define N 20
#define M 300
using namespace std;
using namespace rapidjson; 
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

/*string generateTime()//产生随机的时间段
{
	int tmp;
	string temp;
	char c[10];
	string week[7] = { "Mon", "Tues", "Thur", "Wed", "Fri", "Sat", "Sun" };
	tmp = rand() % 7;
	string temp1 = "";
	temp1 = temp1 + week[tmp] + ".";
	tmp = 6 + rand() % 19;//产生6~24的随机数
	int t = 1 + rand() % 4;//产生活动时长，1~4小时
	_itoa(tmp, c, 10);
	temp = c;
	temp1 = temp1 + temp + ":00~";
	tmp = tmp + t;
	_itoa(tmp, c, 10);
	temp = c;
	temp1 = temp1 + temp + ":00";
	return temp1;
}
*/
void generateTime(string* tim)//产生随机的时间段
{
	int tmp;
	string week[7] = { "Mon.", "Tues.", "Thur.", "Wed.", "Fri.", "Sat.", "Sun." };
	char c[10];
	int tt = rand()%8;//产生一星期中有空的天数
	if (tt == 0)
		return;
	int t = 0;
	set<string>ss;
	for(int i = 0; i < tt;i++)
	{
		tmp = rand() % 7; 
		ss.insert(week[tmp]);

	}
	set<string>::iterator it;
	for (it = ss.begin(); it != ss.end(); it++)
	{
		string temp1 = "";
		string temp;
		temp1 =temp1 + *it;
		tmp = 6+rand() % 18;//产生一天中初始时间点(6-23之间)
		_itoa(tmp, c, 10);
		temp = c;
		temp1 = temp1 + temp + ":00~";
		while (tmp<24)
		{
			tmp = tmp + 1+rand() % 5;//生成结束时间
			if (tmp>24)
				break;
			_itoa(tmp, c, 10);
			temp = c;
			temp1 = temp1 + temp + ":00";
			tim[t++] = temp1;
			temp1 = "";
			tmp = tmp + 1+ rand() % 16;//生成一天内下一次初始时间点
			if (tmp >= 24)
				break;
			_itoa(tmp, c, 10);
			temp = c;
			temp1 = temp1 + *it+ temp + ":00~";
		}
	}
}

string generateTags()//产生随机的tags
{
	//srand((unsigned)time(NULL));
	string tag[20] = { "study", "film", "English", "music", "reading", "chess", "football", "dance", "programming", "basketball",
		"table tennisy", "volleyball", "badminton", "run", "tour", "climb", "compute games", "swim", "chat", "eat" };
	int tmp = rand() % 20;
	return tag[tmp];
}


int main()
{
	int tmp;
	char c[10];
	set <string> strset;
	srand((unsigned)time(NULL));
	Document doc;
	doc.SetObject();
	Document::AllocatorType &allocator = doc.GetAllocator(); //获取分配器
	Value temp_string;
	Value stu(kArrayType);
	for (int i = 0; i < M; i++)
	{
		Value obj(kObjectType);

		Value free_time_array(kArrayType);

		generateTime(student[i].free_time);
		int t = 0;
		while (student[i].free_time[t] != ""&&t < 20)
		{
			temp_string.SetString(StringRef(student[i].free_time[t].c_str()));
			free_time_array.PushBack(temp_string, allocator);
			t++;
		}
/*		int k = rand() % 11;
		for (int j = 0; j < k; j++)
		{
			student[i].free_time[j] = generateTime();
			temp_string.SetString(StringRef(student[i].free_time[j].c_str()));
			free_time_array.PushBack(temp_string, allocator);
		}
	*/	
		obj.AddMember("free_time", free_time_array, allocator);

		tmp = 1 + rand() % 20;
		_itoa(i + 1, c, 10);
		string temp = c;
		if (i < 10)
			student[i].student_no = "03150200" + temp;
		else if (i < 100)
			student[i].student_no = "0315020" + temp;
		else
			student[i].student_no = "031502" + temp;

		temp_string.SetString(StringRef(student[i].student_no.c_str()));
		obj.AddMember("student_no", temp_string, allocator);

		Value applications_department_array(kArrayType);
		int	k = 1 + rand() % 5;
		for (int j = 0; j < k; j++)
		{
			tmp = rand() % 20 + 1;
			_itoa(tmp, c, 10);
			string temp = c;
			if (tmp < 10)
			{
				string s = "";
				s = "D00" + temp;
				student[i].applications_department[j] = s;
			}
			else
			{
				string s = "";
				s = s + "D0" + temp;
				student[i].applications_department[j] = s;
			}
			temp_string.SetString(StringRef(student[i].applications_department[j].c_str()));
			applications_department_array.PushBack(temp_string, allocator);
		}
		obj.AddMember("applications_department", applications_department_array, allocator);

		Value tag_array(kArrayType);
		tmp = 0 + rand() % 7;
		for (int j = 0; j < tmp; j++)
		{
			student[i].tags[j] = generateTags();
			temp_string.SetString(StringRef(student[i].tags[j].c_str()));
			tag_array.PushBack(temp_string, allocator);
		}
		obj.AddMember("tags", tag_array, allocator);


		stu.PushBack(obj, allocator);
		
	}
	doc.AddMember("students",stu, allocator);

	Value dep(kArrayType);
	for (int i = 0; i < N; i++)
	{
		Value obj(kObjectType);

		Value event_schedules_array(kArrayType);
		generateTime(department[i].event_schedules);
		int t = 0;
		while (department[i].event_schedules[t] != ""&&t < 20)
		{
			temp_string.SetString(StringRef(department[i].event_schedules[t].c_str()));
			event_schedules_array.PushBack(temp_string, allocator);
			t++;
		}

/*		int k =  rand() % 8;
		for (int j = 0; j < k; j++)
		{
			department[i].event_schedules[j] = generateTime();
			temp_string.SetString(StringRef(department[i].event_schedules[j].c_str()));
			event_schedules_array.PushBack(temp_string, allocator);
		}*/
		obj.AddMember("event_schedules", event_schedules_array, allocator);

		department[i].member_limit = 10 + rand() % 6;
		obj.AddMember("member_limit", department[i].member_limit, allocator);

		_itoa(i + 1, c, 10);
		string temp = c;
		if (i < 10)
		{
			string s = "";
			s = "D00" + temp;
			department[i].department_no = s;
		}
		else
		{
			string s = "";
			s = s + "D0" + temp;
			department[i].department_no = s;
		}
		temp_string.SetString(StringRef(department[i].department_no.c_str()));
		obj.AddMember("department_no",temp_string, allocator);


		Value tag_array(kArrayType);
		tmp = 1 + rand() % 10;
		for (int j = 0; j < tmp; j++)
		{
			department[i].tags[j] = generateTags();
			temp_string.SetString(StringRef(department[i].tags[j].c_str()));
			tag_array.PushBack(temp_string, allocator);
		}
		obj.AddMember("tag", tag_array, allocator);

		dep.PushBack(obj, allocator);
	}
	doc.AddMember("departments", dep, allocator);

	StringBuffer buffer;
	PrettyWriter<StringBuffer> pretty_writer(buffer);  //PrettyWriter是格式化的json，如果是Writer则是换行空格压缩后的json
	doc.Accept(pretty_writer);
	//输出到文件
	ofstream ffout;
	ffout.open("generate.txt");
	ffout << buffer.GetString();
	ffout.close();
	return 0;
}
