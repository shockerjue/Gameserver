#include "loginhost.h"

namespace gateway {
	void LoginHost::sendLoginVeritification(const char* buf,const int size){
		if(nullptr == buf || !size){
			return ;
		}

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::LOGIN,
			_MsgType_Gateway_Login::SS_GATEWAY_REQUEST_LOGIN_VERI,
			buf,size);
	}

	void LoginHost::onLoginVeritification(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}
		
		string to_data;
		to_data.assign(buf,size);
		
		inner_ipc::Login2GatewayIPC to_login;
		to_login.ParseFromString(to_data);

		int requestId = to_login.requestid();

		RoleLoginResponse role;
		role.set_status(to_login.status());
		role.set_port(to_login.port());
		role.set_rid(to_login.rid());
		role.set_address(to_login.address());
		role.set_key(to_login.key());
		role.set_msg(to_login.msg());

		string data;
		role.SerializeToString(&data);
		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		RoleHost::responseLogin(requestId,_data,data.length());
		
		Log::Perror(__func__, size);
		Log::Print(__func__, to_login.rid());
		Log::Print(__func__, to_login.msg());
	}

	void LoginHost::sendCreateRole(const char* buf,const int size){
		if(nullptr == buf || !size){
			return ;
		}

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::LOGIN,
			_MsgType_Gateway_Login::SS_GATEWAYTO_LOGIN_CREATEROLE,
			buf,size);
	}

	void  LoginHost::onCreateRole(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string to_data;
		to_data.assign(buf,size);

		inner_ipc::Login2GatewayIPC to_login;
		to_login.ParseFromString(to_data);

		int requestId = to_login.requestid();

		CreateRoleResponse role;
		role.set_status(to_login.status());
		role.set_port(to_login.port());
		role.set_rid(to_login.rid());
		role.set_address(to_login.address());
		role.set_key(to_login.key());
		role.set_msg(to_login.msg());

		string data;
		role.SerializeToString(&data);
		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		RoleHost::responseCreateRole(requestId,_data,sizeof(_data));

		Log::Print(__func__, to_login.msg());
	}
}