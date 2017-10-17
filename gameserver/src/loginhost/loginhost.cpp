#include "loginhost.h"

namespace loginhost {
	void LoginHost::notify2loginforconnect(Network network){
		if(network.GetType() == ConnectType::NONEDEFAULT){
			inner_ipc::IoServerLoginIPC login;
			login.set_op(1);

			string data;
			login.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length() + 1);
			memcpy(_data,data.c_str(),data.length());

			NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::LOGIN,
				_MsgType_Login_Gameserver::SS_GAMESERVER_NOTI_CONNECT,
				_data,data.length());

			Log::Print(__func__,login.op());

			return ;
		}

		return ;
	}

	void LoginHost::notify2loginforclose(Network network){
		if(network.GetType() == ConnectType::NONEDEFAULT){
			inner_ipc::IoServerLoginIPC login;
			login.set_op(0);

			string data;
			login.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length() + 1);
			memcpy(_data,data.c_str(),data.length());

			NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::LOGIN,
				_MsgType_Login_Gameserver::SS_GAMESERVER_NOTI_CLOSED,
				_data,data.length());

			Log::Print(__func__,login.op());
			
			return ;
		}

		return ;
	}


	void LoginHost::onNotifyfromlogin(NetId netid,const char* buf,int size){
		if(nullptr == buf){
			return ;
		}
		
		string data;
		data.assign(buf,size);
		
		inner_ipc::LoginIoServerIPC login;
		login.ParseFromString(data);

		KeyManage::getKeyManage()->appendkey(login.rid(),login.key());

		Log::Print(__func__, login.key());

		return ;
	}
}