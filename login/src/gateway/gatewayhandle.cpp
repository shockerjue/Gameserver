#include "gatewayhandle.h"

namespace loginhost {
	void GatewayHandle::onLoginVerification(NetId netid,const char* buf,int size){
		if(nullptr == buf || !netid){

			return ;
		}

		string to_data;
		to_data.assign(buf,size);

		inner_ipc::Gateway2LoginIPC to_login;
		int pret = to_login.ParseFromString(to_data);
		if(!pret) {
			Log::Perror(__func__, "to_login.ParseFromString() error!");
		}

		char data[1024] = {0};
		int ret = CacheManage::getRamCache()->getCache(to_login.username(),data);
		if(0 != ret){
			/**
			* 有缓存的话直接使用缓存数据
			*/
			string cache_data = data;

			inner_ipc::Login2GatewayIPC lg;
			lg.ParseFromString(cache_data);
			lg.set_requestid(to_login.requestid());

			string _dt;
			lg.SerializeToString(&_dt);
			char __dt[_dt.length() + 1];
			memset(__dt,0,_dt.length() + 1);
			memcpy(__dt,_dt.c_str(),_dt.length());

			NetworkManage::getNetworkManage()->SendByNetId(netid,
				_MsgType_Gateway_Login::SS_GATEWAY_REQUEST_LOGIN_VERI,
				__dt,_dt.length());

			inner_ipc::ResponseLoginIPC dblog;
			dblog.set_rid(lg.rid());
		
			NetId _netid = GameserverHandle::getNetIdByIpAPort(
				lg.address().c_str(),lg.port());
			if(_netid){
				string _dblog;
				dblog.SerializeToString(&_dblog);
				char __dblog[_dblog.length() + 1];
				memset(__dblog,0,_dblog.length() + 1);
				memcpy(__dblog,_dblog.c_str(),_dblog.length());

				char key[100] = {0};
				strcpy(key,lg.key().c_str());

				GameserverHandle::notifyToIoServer(_netid,_dblog.c_str(),
					_dblog.length(),key);
			}

			Log::Print(__func__, lg.key() + " Data from ramcache!");

			return ;
		}


		Log::Print(__func__, to_login.requestid());

		DbHandle::GetVerification(to_login);

		return ;
	}

	void GatewayHandle::onCreateRole(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string to_data;
		to_data.assign(buf,size);

		inner_ipc::Gateway2LoginIPC to_login;
		to_login.ParseFromString(to_data);

		if(!to_login.requestid()){
			return ;
		}

		char data[1024] = {0};
		int ret = CacheManage::getRamCache()->getCache(to_login.username(),
			data);
		if(0 != ret){
			return ;
		}

		Log::Print(__func__,to_login.username());

		DbHandle::CreateRole(buf,size);	
	}

	bool GatewayHandle::responseWithLogin(const char* buf,int size,char key[],
			const char* ip,unsigned short port){
		bool is_success = false;

		if(nullptr == buf || nullptr == ip ||
			 !port){

			return is_success;
		}

		string data;
		data.assign(buf,size);

		inner_ipc::ResponseLoginIPC db;
		db.ParseFromString(data);

		inner_ipc::Login2GatewayIPC gate;
		gate.set_rid(db.rid());
		gate.set_requestid(db.requestid());
		gate.set_status(db.status());
		gate.set_msg(db.msg());

		if(0 == db.status() && 0 != db.rid()) {
			gate.set_port(port);
			gate.set_address(string(ip));
			gate.set_key(Util::genkey());
			strcpy(key,gate.key().c_str());

			is_success = true;
		}

		string _data;
		gate.SerializeToString(&_data);
		char s_data[_data.length() + 1];
		memset(s_data,0,_data.length() + 1);
		memcpy(s_data,_data.c_str(),_data.length());

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::GATEWAY,
			_MsgType_Gateway_Login::SS_GATEWAY_REQUEST_LOGIN_VERI,
			s_data,_data.length());

		// 验证通过才缓存登录信息
		if(is_success) {
			CacheManage::getRamCache()->addCache(db.username(),s_data,
				_data.length());
		}

		Log::Print(__func__, is_success);
		Log::Print(__func__, gate.key());

		return is_success;
	}

	void GatewayHandle::responseWithCreateRole(const char* buf,int size,char key[],
			const char* ip,unsigned short port){
		if(nullptr == buf || nullptr == ip ||
			 !port){

			return ;
		}

		string data;
		data.assign(buf, size);  

		inner_ipc::ResponseLoginIPC db;
		db.ParseFromString(data);

		inner_ipc::Login2GatewayIPC gate;
		gate.set_port(port);
		gate.set_rid(db.rid());
		gate.set_requestid(db.requestid());
		gate.set_status(db.status());
		gate.set_key(Util::genkey());
		gate.set_address(string(ip));
		gate.set_msg("CreateRole  is success!");

		strcpy(key,gate.key().c_str());

		string _data;
		gate.SerializeToString(&_data);
		char s_data[_data.length() + 1];
		memset(s_data,0,_data.length() + 1);
		memcpy(s_data,_data.c_str(),_data.length());

		CacheManage::getRamCache()->addCache(db.username(),s_data,
			_data.length());

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::GATEWAY,
			_MsgType_Gateway_Login::SS_GATEWAYTO_LOGIN_CREATEROLE,
			s_data,_data.length());

		Log::Print(__func__, gate.key());
	}
}