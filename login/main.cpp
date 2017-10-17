#include "src/login.h"
#include "src/callback/datacallback.h"
#include "./../common/include/servermodule.h"

using namespace engine;
using namespace login;

#ifdef __linux
#ifdef _cplusplus
    extern "C"{
#endif
	#include <assert.h>
	#include <sys/resource.h>
#ifdef _cplusplus
    }
#endif

	#define MAX_FILE_NO		(1024 * 1)

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

struct Test {
	int a;
	int b;
};

int main(int argc,const char* argv[]){
	if(argc < 2){
		Log::Perror(__func__,"plaese input config file!");

		return 1;
	}

    GOOGLE_PROTOBUF_VERIFY_VERSION;

	string config(argv[1]);

	#ifdef __linux
		assert(CheckResource());
	#endif
	
	ServerModule* pLogin = Login::getLogin();
	DataCallback* pDataCallback = new DataCallback();

	pLogin->onLoadConfig(config);
	pLogin->onInit();
	pLogin->setNetworkCallback(pDataCallback);
	pLogin->ConnectHostBySign("dbhost","dbport",ConnectType::DB,ConnectType::LOGIN);
	pLogin->ConnectHostBySign("loghost","logport",ConnectType::LOGSERVER,ConnectType::LOGIN);
	pLogin->onStart();
	pLogin->registerHandler();

	pLogin->Run();

	delete pLogin;
	delete pDataCallback;

	pLogin = nullptr;
	pDataCallback = nullptr;

    google::protobuf::ShutdownProtobufLibrary();


	return 0;
}