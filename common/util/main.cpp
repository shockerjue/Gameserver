#include <iostream>
#include "util.h"


using namespace std;
using namespace util;

int main(int argc,const char* argv[]){
	int i = 0;
	for(i = 0; i < 100; ++i){
		cout<<Util::genkey()<<endl;	
	}

	return 0;
}
