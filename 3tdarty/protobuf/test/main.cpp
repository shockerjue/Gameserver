#include <string>
#include <iostream>

#include "student.pb.h"

using namespace std;

int main(int argc,const char* argv[]){
	tutorial::Student student;
	student.set_id(123456);
	student.set_name("shockejrue");
	student.set_email("shockerjue@126.com");

	string data;
	student.SerializeToString(&data); 
	cout<<data.size()<<endl;

	tutorial::Student _stu;
	_stu.ParseFromString(data);
	cout<<_stu.email()<<endl;

	return 0;
}

/**
注：对于发送数据和接受数据，就可以使用上面的data。
	string data;
	student.SerializeToString(&data);
	char bts[data.length()];  
  	strcpy(bts, data.c_str());  
  	send(connectfd, bts, sizeof(bts), 0);

  	string data = buf;  
  	tutorial::Student p;  
  	p.ParseFromString(data);
*/