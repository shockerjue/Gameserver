#include "callback.h"

namespace engine {
	
	void NetworkCallback::onClose(Network network){
		Log::Print(__func__, "Network closed!");
	}

	void NetworkCallback::onTimeout(Network network){
		Log::Print(__func__,"Network timeout!");
	}

	void NetworkCallback::onConnect(Network network){
		Log::Print(__func__, "Network connect!");
	}

	void BaseCallback::onThreadMsg(const char* buf,const int len,
		NetworkCallback* callback /* = nullptr */){
		if(nullptr == buf || !len){
			return ;
		}
		
		int _len = 0;
		const char* p = buf;
		while(p != nullptr && _len < len){
			Message message;
			message.setwhat(*((int*)p));
			message.setsize(*((int*)(p + sizeof(int))));
			message.setobject(p + sizeof(int) + sizeof(int),message.getsize());

			_len = _len + sizeof(int) + sizeof(int) + message.getsize();
			p = p + sizeof(int) + sizeof(int) + message.getsize();

			int what = message.getwhat();
			int size = message.getsize();
			if(!what){
				continue ;
			}

			map<ThreadWhat,ThreadMsgFunc>::iterator it = EventHandle::g_thread_msg_call.find(what);
			if(it == EventHandle::g_thread_msg_call.end()){
				Log::Warning(__func__,message.toString());

				continue ;
			}

			if(message.getwhat() == S_WhatType::W_NOTI_RECV_TIMEOUT){
				int netid = 0;
				message.getobject(&netid,sizeof(int));
				if(nullptr != callback && 0 != netid){
					Network network = NetworkManage::getNetworkManage()->getNetwork(netid);
					if(1 == network.isConnect()){
						callback->onTimeout(network);
					}
				}
			}

			EventHandle::g_thread_msg_call[what](message);

			Log::Print(__func__, message.toString());
		}
	}

	void BaseCallback::onRecv(Network network,const char* buf,
		const int len){
		if(nullptr == buf || !len){
			return ;
		}

		int _len = 0;
		const char* p = buf;

		while(p != nullptr && _len < len){
			struct MessageHeader* header = (struct MessageHeader*)p;
			int msg_type = header->msg_type;

			if(0 >= msg_type ){
				Log::Perror(__func__, "Msg_type is error!");

				break;
			}
			
			if((_len + header->msg_size) > len){
				break;
			}

			char* data = new char[header->msg_size + 1];
			if(nullptr == data){
				break;
			}

			memcpy(data,p + sizeof(struct MessageHeader),header->msg_size);

			_len = _len + sizeof(struct MessageHeader) + header->msg_size;
			p = p + sizeof(struct MessageHeader) + header->msg_size;
			
			map<ProtocolHeader,EngineFunc>::iterator it = EventHandle::g_msg_call.find(msg_type);
			if(it != EventHandle::g_msg_call.end()){
				EventHandle::g_msg_call[msg_type](network.GetNetId(),data,header->msg_size);
			}else {
				Log::Print(__func__,"Except data ---->");
				NetworkManage::removeNetwork(network);
			}

			delete[] data;
			data = nullptr;
		}

		// 添加到超时队列中
		if(network.GetType() == ConnectType::NONEDEFAULT){
			Timeout::noti_ddos_thread("timeout",network.GetNetId(),60 * 1,
				S_WhatType::W_NOTI_RECV_NORMAL);
		}
	}

	void EngineCallback::onInit(void){
		Log::Print(__func__, "Engine onInit called!");
	}

	void EngineCallback::onStop(void){
		Log::Print(__func__, "Engine onStop called!");
	}

	void EngineCallback::onStart(void){
		Log::Print(__func__, "Engine onStart called!");
	}

	void EngineCallback::onLoadConfig(const string _config){
		Log::Print(__func__, "Engine onLoadConfig called! config file is " + _config);
	}
}