#include <iostream>

#include "ncallback.h"



void NCallback::onRecv(Socket socket,const char* buf,const int size){
	switch(socket.GetType()){
		case ConnectType::NONEDEFAULT:{
			cout<<"NONEDEFAULT :" << buf<<endl;
		}break;
	}
}

void NCallback::onSend(const int size){

}

void NCallback::onClose(Socket socket){
	cout<<"disconnected "<< socket.getAddress().getHostAddress()<<endl;
}