#include <iostream>
#include <stdio.h>
#include <fstream>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int main(int argc,const char* argv[]){
	try{
		int fd = open("./test.json",O_RDONLY);
		if(-1 == fd){
			return -1;
		}

		char buf[1024] = {0};
		read(fd,buf,1024);
		string jstr(buf);
		auto j3 = json::parse(jstr);

		// json j3 = jstr;
		// cout<<jh<<endl;
		// ifstream ifs("./test.json");
		// json load;
    	// ifs >> load;
		
		cout<<j3["wgs"]["port"]<<endl;
	}catch( ... ){
		cout<<"happened except"<<endl;
	}
	return 0;
}
