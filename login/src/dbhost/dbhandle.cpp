#include "dbhandle.h"

namespace loginhost {
	void DbHandle::GetVerification(inner_ipc::Gateway2LoginIPC to_login){
		inner_ipc::RequestLoginIPC db;
		db.set_requestid(to_login.requestid());
		db.set_username(to_login.username());
		if(0 >= to_login.requestid() || 0 >= to_login.username().length()){
			return ;
		}

		string  data;
		db.SerializeToString(&data);

		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::DB,
			_MsgType_Login_Db::SS_LOGIN_VERI_TO_DB,_data,data.length());

		Log::Print(__func__, to_login.requestid());
		
		return ;
	}

	void DbHandle::CreateRole(const char* buf,int size){
		if(nullptr == buf){
			return ;
		}

		string to_data = buf;
		inner_ipc::Gateway2LoginIPC to_login;
		to_login.ParseFromString(to_data);

		inner_ipc::RequestLoginIPC db;
		db.set_requestid(to_login.requestid());
		db.set_username(to_login.username());
		if(0 >= to_login.requestid() || 0 >= to_login.username().length()){
			return ;
		}

		string data;
		db.SerializeToString(&data);

		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::DB,
			 _MsgType_Login_Db::SS_LOGIN_TO_DB_CREATEROLE,
			 _data,data.length());

		Log::Print(__func__, db.username());

		return ;
	}

	void DbHandle::onGetVerification(NetId netid,const char* buf,int size){
		if(nullptr == buf){
			return ;
		}

		char key[100] = {0};

		NetId net = GameserverHandle::getLightest();
		if(!net){
			Log::Perror(__func__, "IOServer didn't open!");

			return ;
		}

		struct LightestIpInfo info = GameserverHandle::getGameserverInfo(net);

		bool ret = GatewayHandle::responseWithLogin(buf,size,key,info.m_ip,info.m_port);
		if(ret) {
			GameserverHandle::notifyToIoServer(net,buf,size,key);
		}

		Log::Print(__func__, info.toString());
	}

	void DbHandle::onCreateRole(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		if(nullptr == buf){
			return ;
		}

		char key[100] = {0};

		NetId net = GameserverHandle::getLightest();
		if(!net){
			Log::Perror(__func__, "IOServer didn't open!");

			return ;
		}

		struct LightestIpInfo info = GameserverHandle::getGameserverInfo(net);

		GatewayHandle::responseWithCreateRole(buf,size,key,info.m_ip,info.m_port);
		GameserverHandle::notifyToIoServer(net,buf,size,key);

		Log::Print(__func__,info.toString());
	}
}