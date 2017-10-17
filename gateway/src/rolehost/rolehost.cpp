#include "rolehost.h"

namespace gateway {
	void RoleHost::onLoginVerifiticaion(NetId netid,const char* buf,int size){
		if(nullptr == buf){
			return ;
		}

		string from_data;
		from_data.assign(buf,size);
		
		RoleLoginRequest from_role;
		from_role.ParseFromString(from_data);

		if(Platom::getPlatom()->verification(from_role.platomid())) {
			Log::Print(__func__ ," Device login platom is pass!");
		}else {
			Log::Warning(__func__, "Device login platom is can't identify!");
		}
		/**
		* 验证通过后转发登录请求到login服务器请求登录信息
		*/
		inner_ipc::Gateway2LoginIPC to_login;
		to_login.set_requestid(netid);
		to_login.set_username(from_role.username());

		string data;
		to_login.SerializeToString(&data);
		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		LoginHost::sendLoginVeritification(_data,data.length());

		DDos::noti_ddos_thread("ddos",netid,
			5,S_WhatType::W_NOTI_NETID_NORMAL);

		// Log::Print(__func__, to_login.requestid());
		NetworkManage::getNetworkManage()->Send2LogServer(ConnectType::LOGSERVER,
			LogLevel::I,__func__,to_login.username());
	}

	void RoleHost::responseLogin(NetId netid,const char* buf,const int size){
		if(!netid || nullptr == buf ||
			!size){
			return ;
		}
		
		NetworkManage::getNetworkManage()->SendByNetId(netid,
			S_MsgType::CS_ROLE_TO_GATEWAY,buf,size);
	}

	void RoleHost::onCreateRole(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string from_role;
		from_role.assign(buf,size);

		CreateRoleRequest from_data;
		from_data.ParseFromString(from_role);

		/**
		* 平台认证通过后将创建请求转发到login服务器
		*/
		inner_ipc::Gateway2LoginIPC to_login;
		to_login.set_requestid(netid);
		to_login.set_username(from_data.username());

		string data;
		to_login.SerializeToString(&data);
		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		LoginHost::sendCreateRole(_data,sizeof(_data));

		DDos::noti_ddos_thread("ddos",netid,
			5,S_WhatType::W_NOTI_NETID_NORMAL);

		// Log::Print(__func__,to_login.username());
		NetworkManage::getNetworkManage()->Send2LogServer(ConnectType::LOGSERVER,
			LogLevel::I,__func__,to_login.username());
	}

	void RoleHost::responseCreateRole(NetId netid,const char* buf,const int size){
		if(!netid || nullptr == buf ||
			!size){
			return ;
		}

		NetworkManage::getNetworkManage()->SendByNetId(netid,
			S_MsgType::CS_ROLE_TOGATEWAY_CREATEROLE,buf,size);
	}
}