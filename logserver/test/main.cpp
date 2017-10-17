#include "./../../common/util/util.h"
#include "./../../common/network/network.h"
#include "./../../common/msgdefine/msgheader.h"


using namespace util;
using namespace msghandle;
using namespace engine::network;


int main(int argc,const char* argv[]){
	NetworkAddress networkAddres("127.0.0.1",4);
	#ifdef __linux
		Log::Print(__func__,"__linux");
	#else
		Log::Print(__func__,"Other");
	#endif

	// int op = 1;
	// Network network(network::Protocol::TCP);
 //    network.SetOpt(SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
 //    network.Bind(networkAddres,8999);
 //    network.Listen(5);

 //    Network acce;
 //    network.Accept(&acce);
 //    Log::Print(__func__,"New connect to here!");
 //    char buf[1024] = {0};
 //    int ret = acce.Recv(buf,sizeof(buf));
 //    cout<<ret<<endl;
 //    struct SendLogin* login = (struct SendLogin*)buf;
 //    Log::Print("SendLogin",string(login->userName));

	// struct FromLogin client;
	// client.port = 8000;
	// client.requestId = login->requestId;
	// client.status = 200;
	// strcpy(client.address,"127.0.0.1");
	// strcpy(client.key,"ADBHJAKJH9878687ASDFS");
	// strcpy(client.msg,"Ok");
	
	// int size = acce.Send((char*)&client,sizeof(client));
	// cout<<sizeof(struct SSChatMsg)<<endl;
	Log::Print(__func__,"Send success-----------");

	return 0;
}