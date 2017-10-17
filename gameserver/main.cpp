#include <stdio.h>
#include <iostream>
#include <memory>

#include "src/gameserver.h"
#include "src/callback/callback.h"
#include "./../common/include/thread.h"
#include "./../common/ddos/threadimplement.h"
#include "./../common/include/servermodule.h"

using namespace std;
using namespace engine;
using namespace gameserver;
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

	#ifdef __linux
	assert(CheckResource());
	#endif

	ServerModule* pGameServer = GameServer::getGameServer();
	Callback* pCallback = new Callback();

	pGameServer->onLoadConfig(config);
	pGameServer->onInit();
	pGameServer->setNetworkCallback(pCallback);
	pGameServer->ConnectHostBySign("loginhost","loginport",ConnectType::LOGIN,ConnectType::IOSERVER);
	pGameServer->ConnectHostBySign("gwhost","gwport",ConnectType::GAMEWORLD,ConnectType::IOSERVER);
	pGameServer->ConnectHostBySign("dbhost","dbport",ConnectType::DB,ConnectType::IOSERVER);
	pGameServer->ConnectHostBySign("chathost","chatport",ConnectType::CHATSERVER,ConnectType::IOSERVER);
	pGameServer->ConnectHostBySign("loghost","logport",ConnectType::LOGSERVER,ConnectType::IOSERVER);

	Thread thread("ddos");
	thread.run(Thread_Call(ddos));
	pGameServer->addThreadMsgEvent(thread);

	Thread tout("timeout");
	tout.run(Thread_Call(timeout));
	pGameServer->addThreadMsgEvent(tout);

	pGameServer->onStart();
	pGameServer->registerHandler();
	
	pGameServer->Run();

	delete pGameServer;
	delete pCallback;

	pGameServer = nullptr;
	pCallback = nullptr;

    google::protobuf::ShutdownProtobufLibrary();

	return 0;
}