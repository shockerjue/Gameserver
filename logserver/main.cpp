#include <stdio.h>

#include "src/logserver.h"
#include "src/callback/callback.h"
#include "./../common/include/servermodule.h"

using namespace engine;
using namespace logserver;

#ifdef __linux
#ifdef _cplusplus
    extern "C"{
#endif
	#include <assert.h>
	#include <sys/resource.h>
#ifdef _cplusplus
    }
#endif

	#define MAX_FILE_NO		(1024 * 2)

	bool CheckResource()
	{
		struct rlimit rl;
		if (getrlimit(RLIMIT_NOFILE, &rl) == -1)
		{
			return false;
		}
		rl.rlim_cur = (rl.rlim_cur > MAX_FILE_NO ? rl.rlim_cur : MAX_FILE_NO);
		rl.rlim_max = (rl.rlim_max > MAX_FILE_NO ? rl.rlim_max : MAX_FILE_NO);
		if (setrlimit(RLIMIT_NOFILE, &rl) == -1)
		{
			return false;
		}
		return true;
	}
#endif

int main(int argc,const char* argv[]){
	if(argc < 2){
		Log::Perror(__func__,"plaese input config file!");

		return 1;
	}

	string config(argv[1]);

	#ifdef __linux
	assert(CheckResource());
	#endif

	ServerModule* access = LogServer::getLogServer();
	Callback* callback = new Callback();

	access->onLoadConfig(config);
	access->onInit();
	access->setNetworkCallback(callback);
	access->registerHandler();
	
	access->onStart();
	access->Run();

	delete access;
	delete callback;

	access = nullptr;
	callback = nullptr;


	return 0;
}