#include "threadhandle.h"

void ThreadHandle::Msg_10000(Message message){
	char data[128] = {0};
	message.getobject(data,message.getsize());

	Log::Print(__func__,data);
}

void ThreadHandle::Msg_20000(Message message){
	char data[128] = {0};
	message.getobject(data,message.getsize());

	Log::Print(__func__,data);
}

void ThreadHandle::Msg_30000(Message message){
	char data[128] = {0};
	message.getobject(data,message.getsize());

	Log::Print(__func__,data);
}