#include <stdio.h>

#include "src/gameworld.h"
#include "src/callback/callback.h"
#include "./../common/include/thread.h"
#include "./../common/include/servermodule.h"

using namespace engine;
using namespace gameworld;
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

Thread_Func(thread){
	Thread* thread = (Thread*)_thread;
	int i  = 0;

	while(i++ < 10){
		char data[128] = {0};
		sprintf(data,"10000---------[%d]",1);

		Message message;
		message.setwhat(10000);
		message.setsize(strlen(data));
		message.setobject(data,strlen(data));
		thread->send_message_to_main(message);

		sleep(1);
	}
	
	Log::Print(__func__,"Thread::sendMessage  test!");

	return nullptr;
}


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

	ServerModule* pGameWorld = GameWorld::getGameWorld();
	Callback*  callback = new Callback();

	pGameWorld->onLoadConfig(config);
	pGameWorld->onInit();
	pGameWorld->setNetworkCallback(callback);
	pGameWorld->ConnectHostBySign("dbhost2","dbport2",ConnectType::DB2,ConnectType::GAMEWORLD);
	pGameWorld->ConnectHostBySign("loghost","logport",ConnectType::LOGSERVER,ConnectType::GAMEWORLD);

	pGameWorld->onStart();

	// create thread
	Thread th("thread");
	th.run(Thread_Call(thread));

	pGameWorld->addThreadMsgEvent(th);
	
	pGameWorld->registerHandler();
	pGameWorld->Run();
	
	delete pGameWorld;
	delete callback;

	pGameWorld = nullptr;
	callback = nullptr;

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}