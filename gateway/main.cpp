#include <stdio.h>

#include "src/gateway.h"
#include "src/platom/platom.h"
#include "src/callback/callback.h"
#include "./../common/include/thread.h"
#include "./../common/ddos/threadimplement.h"
#include "./../common/include/servermodule.h"

using namespace platom;
using namespace engine;
using namespace gateway;
using namespace engine::ddos;
using namespace engine::thread;

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

    GOOGLE_PROTOBUF_VERIFY_VERSION;
        
	string config(argv[1]);
	Platom::getPlatom()->parse("./../config/platom.json");
	
	#ifdef __linux
		assert(CheckResource());
	#endif

	ServerModule* gateway = Gateway::getGateway();
	Callback* callback = new Callback();

	gateway->onLoadConfig(config);
	gateway->onInit();
	gateway->setNetworkCallback(callback);

	/**
	* 连接到登录服务器
	*/
	gateway->ConnectHostBySign("loginhost","loginport",ConnectType::LOGIN,
		ConnectType::GATEWAY);
	gateway->ConnectHostBySign("loghost","logport",ConnectType::LOGSERVER,
		ConnectType::GATEWAY);

	Thread thread("ddos");
	thread.run(Thread_Call(ddos));
	gateway->addThreadMsgEvent(thread);

	Thread tout("timeout");
	tout.run(Thread_Call(timeout));
	gateway->addThreadMsgEvent(tout);

	gateway->onStart();
	gateway->registerHandler();
	
	gateway->Run();

	delete callback;
	delete gateway;

	callback = nullptr;
	gateway = nullptr;

    google::protobuf::ShutdownProtobufLibrary();

	return 0;
}
