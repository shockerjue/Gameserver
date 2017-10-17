#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_test.h"
#include "thread_test.h"
#include "shareram_test.h"
#include "ramcache_test.h"
#include "./../../common/util/util.h"
#include "./../../common/include/type.h"
#include "./../../common/network/network.h"


using namespace engine::network;
using namespace engine;
using namespace util;


int main(int argc,const char* argv[]){
	if(argc < 2){
		Log::Perror(__func__, "Please input params!");

		return 1;
	}

	string cmd(argv[1]);
	if(0 == cmd.compare("thread")){
		thread_test();
	}else if(0 == cmd.compare("shareram")){
		shareram_test();
	}else if(0 == cmd.compare("ramcache")){
		ramcache_test();
	}else {
		/**
		* 输入服务器ip，服务器端口，玩家名
		*/
		if(argc < 3) {
			Log::Perror(__func__, "Run game_test Please input ip,port,name!");

			return 1;
		}

		int port = atoi(argv[2]);
		game_test(argv[1],port,argv[3]);
	}
	
	return 0;
}