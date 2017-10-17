#include "./../src/msghandle/gameserver_msgheader.pb.h"
#include "./../../common/util/util.h"
#include "./../../common/msgdefine/msgheader.h"
#include "./../../common/network/network.h"

using namespace util;
using namespace msghandle;
using namespace gameserver_proto;
using namespace engine;
using namespace engine::network;

int main(int argc,const char* argv[]){
	NetworkAddress address("127.0.0.1",4);
	Network network(network::Protocol::TCP);
	bool ret = network.Connect(address,8092);
	if(!ret){
		Log::Perror(__func__,"Connect fail!");

		return -1;
	}

	LoginRequest login;
	login.set_rid(9021);
	login.set_key("ABCFARTQYT09879HJGJ^&**&()JKJI890");

	string data;
	login.SerializeToString(&data);

	struct MessageHeader header;
	header.msg_type = 10000;
	header.msg_size = data.length();

	char _data[sizeof(struct MessageHeader) + data.length()];
	memcpy(_data,&header,sizeof(struct MessageHeader));
	memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());

	int len = network.Send(_data,sizeof(_data));
	cout<<"Send sccuess len:"<<len<<" B"<<endl;

	char buf[100] = {0};
	len = network.Recv(buf,100);

	string _recv = buf + sizeof(struct MessageHeader);
	LoginResponse re_login;
	re_login.ParseFromString(_recv);

	cout<<"{status:"<<re_login.status()<<", msg:"<<re_login.msg()<<"}"<<endl;

	return 0;
}
