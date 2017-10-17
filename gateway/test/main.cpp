#include "./../../common/util/util.h"
#include "./../src/msghandle/msgcode.h"
#include "./../../common/include/type.h"
#include "./../../common/network/network.h"
#include "./../../common/msgdefine/msgheader.h"
#include "./../src/msghandle/gateway_msgheader.pb.h"
#include "./../../gameserver/src/msghandle/gameserver_msgheader.pb.h"


#include <time.h>

using namespace util;
using namespace engine;
using namespace gateway;
using namespace msghandle;
using namespace gateway_proto;
using namespace engine::network;
using namespace gameserver_proto;


void authori(Network network,const char* name){
	if(nullptr == name){
		return ;
	}

	RoleLoginRequest role;
	role.set_platomid("1");
	role.set_username(string(name));

	string data;
	role.SerializeToString(&data);

	struct MessageHeader header;
	header.msg_type = 500;
	header.msg_size = data.length();
	Log::Print(__func__,header.toString());
	
	char _data[sizeof(struct MessageHeader) + data.length()];
	memcpy(_data,(const char*)&header,sizeof(struct MessageHeader));
	memcpy(_data +sizeof(struct MessageHeader),data.c_str(),data.length());

	int size = network.Send(_data,sizeof(_data));

	sleep(2);
	Log::Print(__func__,size);
}

LoginResponse login(Network gamenetwork,RoleLoginResponse sendClient){
	LoginRequest login;
	login.set_rid(sendClient.rid());
	login.set_key(sendClient.key());

	string data;
	login.SerializeToString(&data);

	struct MessageHeader header;
	header.msg_type = 10000;
	header.msg_size = data.length();

	char _data[sizeof(struct MessageHeader) + data.length()];
	memcpy(_data,&header,sizeof(MessageHeader));
	memcpy(_data + sizeof(MessageHeader), data.c_str(),data.length());

	LoginResponse retlogin;

	int size = gamenetwork.Send(_data,sizeof(struct MessageHeader) + data.length());
	if(!size) {
		Log::Perror(__func__, "Login fail!");

		return retlogin;
	}

	Log::Print(__func__,login.rid());
	Log::Print(__func__,size);

	char buf1[100] = {0};
	size = gamenetwork.Recv(buf1,100);
	if(0 == size){
		return retlogin;
	}

	string _rec = buf1 + sizeof(struct MessageHeader);
	retlogin.ParseFromString(_rec);

	cout<<"rid:"<<retlogin.rid()<<endl;
	cout<<"{status:"<<retlogin.status()<<", msg:"<<retlogin.msg()<<"}"<<endl;

	return retlogin;
}

void createRole(Network network,const char* name){
	if(nullptr == name){
		return ;
	}

	CreateRoleRequest role;
	role.set_platomid("1");
	role.set_username(string(name));

	string data;
	role.SerializeToString(&data);

	struct MessageHeader header;
	header.msg_type = CS_ROLE_TOGATEWAY_CREATEROLE;
	header.msg_size = data.length();

	char _data[data.length() + sizeof(struct MessageHeader)];
	memcpy(_data,&header,sizeof(struct MessageHeader));
	memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());

	int size = network.Send(_data,sizeof(struct MessageHeader) + data.length());

	Log::Print(__func__,role.username());
}

void requestRoleInfo(Network network,int rid) {
	RequestRoleInfo req;
    req.set_rid(rid);

    string data;
    req.SerializeToString(&data);

    struct MessageHeader header;
    header.msg_type = 10001;
    header.msg_size = data.length();

	char _data[sizeof(struct MessageHeader) + data.length()];
	memset(_data,0,sizeof(struct MessageHeader) + data.length());
    memcpy(_data,&header,sizeof(struct MessageHeader));
    memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());

	network.Send(_data,sizeof(struct MessageHeader) + data.length());
	Log::Print(__func__, "Request RoleInfo !");
	
	char buf[128] = {0};
	network.Recv(buf,128);

	Log::Print(__func__, "Request roleinfo return !");
}

// zhou
void sendChat(Network network,LoginResponse retlogin,int rid,string msg_data){
	SendChat chat;
	chat.set_rid(retlogin.rid());
	chat.set_did(rid);
	chat.set_msg_type(engine::MsgType::TEXT);
	chat.set_msg_size(msg_data.length());
	chat.set_msg(msg_data);

	string data;
	chat.SerializeToString(&data);

	struct MessageHeader header;
	header.msg_type = 10100;
	header.msg_size = data.length();

	char _data[sizeof(struct MessageHeader) + data.length()];
	memcpy(_data,&header,sizeof(struct MessageHeader));
	memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());
	
	sleep(2);
	network.Send(_data,sizeof(struct MessageHeader) + data.length());

	Log::Print(__func__,chat.msg());
}

// wen
int recvChat(Network network){
	char rmsg[256] = {0};
	int ret = network.Recv(rmsg,256);
	if(!ret){
		return ret;
	}

	string data = rmsg + sizeof(struct MessageHeader);
	RecvChat rchat;
	rchat.ParseFromString(data);

	Log::Print(__func__,rchat.msg());

	return ret;
}

int main(int argc,const char* argv[]){
	if(3 != argc){
		Log::Perror(__func__ ,"Please input name!");
		
		return 1;
	}

	NetworkAddress address("127.0.0.1",4);
	Network network(network::Protocol::TCP);
	bool ret = network.Connect(address,8989);
	if(!ret){
		Log::Perror(__func__,"Connect fail!");

		return 1;
	}

	authori(network,argv[1]);


	char buf[1024] = {0};
	int size = network.Recv(buf,1024);
	if(0 == size){
		Log::Perror(__func__,"Authori recv fail!");

		return 1;
	}
	string data = buf + sizeof(struct MessageHeader);
	RoleLoginResponse sendClient;
	sendClient.ParseFromString(data);

	Log::Print(__func__,sendClient.address());
	Log::Print(__func__,sendClient.port());
	Log::Print(__func__,sendClient.rid());
	Log::Print(__func__,sendClient.key());
	network.Disconnect();
	
	NetworkAddress gameAddress(sendClient.address().c_str(),4);
	Network gamenetwork(network::Protocol::TCP);
	ret = gamenetwork.Connect(gameAddress,sendClient.port());
	if(!ret){
		Log::Perror(__func__, "Connect gameserver fail!");

		return 1;
	}

	sleep(2);
	
	LoginResponse retlogin = login(gamenetwork,sendClient);
	if(!retlogin.rid()){
		Log::Print(__func__,"Login fail!");

		return -1;
	}

	requestRoleInfo(gamenetwork,retlogin.rid());

	string userName(argv[1]);
	string msg_data(argv[2]);

	if(0 == userName.compare("zhou")){
		do{
			sendChat(gamenetwork,retlogin,9000,msg_data);
			
			ret = recvChat(gamenetwork);
		}while(ret > 0);
	}
	
	if(0 == userName.compare("wen")){
		do{
			ret = recvChat(gamenetwork);

			sendChat(gamenetwork,retlogin,9001,msg_data);
		}while(ret > 0);
	}

	if(0 == userName.compare("hai")){
		do{
			ret = recvChat(gamenetwork);

			sendChat(gamenetwork,retlogin,9003,msg_data);
		}while(ret > 0);
	}

	if(0 == userName.compare("glory")){
		do{
			sendChat(gamenetwork,retlogin,9002,msg_data);
			
			ret = recvChat(gamenetwork);
		}while(ret > 0);
	}
	

	return 0;
}
