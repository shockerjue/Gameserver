#include "src/chatserver.h"
#include "src/callback/callback.h"
#include "./../common/include/servermodule.h"

using namespace engine;
using namespace chatserver;

#ifdef __linux
#ifdef _cplusplus
    extern "C"{
#endif
	#include <assert.h>
	#include <sys/resource.h>
#ifdef _cplusplus
    }
#endif

	#define MAX_FILE_NO		(1024 *1)

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

	ServerModule* chatserver = ChatServer::getChatServer();
	Callback* callback = new Callback();

	chatserver->onLoadConfig(config);
	chatserver->onInit();
	chatserver->setNetworkCallback(callback);
	chatserver->ConnectHostBySign("dbhost","dbport",ConnectType::DB,
		ConnectType::CHATSERVER);
	chatserver->ConnectHostBySign("dbhost2","dbport2",ConnectType::DB2,
		ConnectType::CHATSERVER);
	chatserver->ConnectHostBySign("loghost","logport",ConnectType::LOGSERVER,
		ConnectType::CHATSERVER);

	chatserver->onStart();
	chatserver->registerHandler();
	
	chatserver->Run();


	delete chatserver;
	delete callback;

	chatserver = nullptr;
	callback = nullptr;

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}