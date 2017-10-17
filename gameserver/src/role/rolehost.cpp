#include "rolehost.h"

namespace rolehost {
	void RoleHost::onRoleLogin(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		DDos::noti_ddos_thread("ddos",netid,
			5,S_WhatType::W_NOTI_NETID_NORMAL);
		
		LoginResponse sclogin;

		string to_data;
		to_data.assign(buf,size);

		LoginRequest login;
		login.ParseFromString(to_data);
		Log::Print(__func__,login.key());
		if(!login.rid()){
			sclogin.set_status(400);
			sclogin.set_msg("Login except!");

			string data;
			sclogin.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length() + 1);
			memcpy(_data,data.c_str(),data.length());

			NetworkManage::getNetworkManage()->SendByNetId(netid,
				S_MsgType::CS_REQUEST_LOGIN_TO_GAMESERVER,
				_data,data.length());

			NetworkManage::getNetworkManage()->getNetwork(netid).Disconnect();
			NetworkManage::removeNetwork(netid);

			return ;
		}

		KeyManage* pKeyManage = KeyManage::getKeyManage();
		bool isexsits = pKeyManage->isexists(login.rid());
		if(!isexsits){
			sclogin.set_status(401);
			sclogin.set_msg("Login except!");

			string data;
			sclogin.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length());
			memcpy(_data,data.c_str(),data.length());

			NetworkManage::getNetworkManage()->SendByNetId(netid,
				S_MsgType::CS_REQUEST_LOGIN_TO_GAMESERVER,
				_data,data.length());

			NetworkManage::getNetworkManage()->getNetwork(netid).Disconnect();
			NetworkManage::removeNetwork(netid);

			return ;
		}

		isexsits = pKeyManage->isexists(string(login.key()));
		if(!isexsits){
			sclogin.set_status(402);
			sclogin.set_msg("Login except!");

			string data;
			sclogin.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length() + 1);
			memcpy(_data,data.c_str(),data.length());

			NetworkManage::getNetworkManage()->SendByNetId(netid,
				S_MsgType::CS_REQUEST_LOGIN_TO_GAMESERVER,
				_data,data.length());

			NetworkManage::getNetworkManage()->getNetwork(netid).Disconnect();
			NetworkManage::removeNetwork(netid);

			return ;
		}

		bool istimeout = pKeyManage->istimeout(login.rid());
		if(istimeout){
			sclogin.set_status(403);
			sclogin.set_msg("Login timeout!");

			string data;
			sclogin.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length() + 1);
			memcpy(_data,data.c_str(),data.length());

			NetworkManage::getNetworkManage()->SendByNetId(netid,
				S_MsgType::CS_REQUEST_LOGIN_TO_GAMESERVER,
				_data,data.length());

			pKeyManage->removekey(login.rid());
			NetworkManage::getNetworkManage()->getNetwork(netid).Disconnect();
			NetworkManage::removeNetwork(netid);

			return ;
		}

		sclogin.set_status(0);
		sclogin.set_rid(login.rid());
		sclogin.set_msg("Login success!");

		Role* role = new Role();
		role->SetRoleId(login.rid());
		role->SetNetId(netid);

		RoleManage::getRoleManage()->appendRole(netid,role);

		string data;
		sclogin.SerializeToString(&data);
		char _data[data.length() + 1];
		memset(_data,0,data.length());
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetId(netid,
				S_MsgType::CS_REQUEST_LOGIN_TO_GAMESERVER,
				_data,data.length());

		Log::Print(__func__, login.key());
		
		NetworkManage::getNetworkManage()->Send2LogServer(ConnectType::LOGSERVER,
			LogLevel::I,__func__,login.key());

		return ;
	}

	void RoleHost::onRoleLogout(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);

		return ;
	}

	void RoleHost::onRequestRoleInfo(NetId netid,const char* buf,int size) {
		if(!netid || nullptr == buf ||
			!size) {
			return ;
		}

		string data;
		data.assign(buf,size);

		RequestRoleInfo req;
		req.ParseFromString(data);

		Role* role = RoleManage::getRoleManage()->getRoleByNetId(netid);
		if(nullptr == role) {
			return ;
		}
		
		int rid = role->GetRoleId();
		DbOperator::sendRequestRoleInfo(netid,rid,req.rid());

		NetworkManage::getNetworkManage()->Send2LogServer(ConnectType::LOGSERVER,
			LogLevel::I,__func__,"Role request get info!");
	}

	void RoleHost::onRequestFriends(NetId netid,const char* buf,int size) {
		if(!netid || nullptr == buf ||
			!size) {
			return ;
		}

		string data;
		data.assign(buf,size);

		RequestFrineds request;
		request.ParseFromString(data);

		Role* role = RoleManage::getRoleManage()->getRoleByNetId(netid);
		if(nullptr == role) {
			return ;
		}

		int rid = role->GetRoleId();
		DbOperator::sendRequestFriends(netid,rid,request.type());

		NetworkManage::getNetworkManage()->Send2LogServer(ConnectType::LOGSERVER,
			LogLevel::I,__func__,Util::int2str(request.type()));
	}

	void RoleHost::onRequestAddFriend(NetId netid,const char* buf,int size) {
		if(!netid || nullptr == buf ||
		 !size) {
			 return ;
		}

		string data;
		data.assign(buf,size);

		RequestAddFriend request;
		request.ParseFromString(data);

		Role* role = RoleManage::getRoleManage()->getRoleByNetId(netid);
		if(nullptr == role) {
			return ;
		}

		int arid = 0;
		int rid = role->GetRoleId();
		DbOperator::sendRequestAddFriends(netid,rid,request.arid());

		NetworkManage::getNetworkManage()->Send2LogServer(ConnectType::LOGSERVER,
			LogLevel::I, __func__, Util::int2str(request.arid()));
	}

	void RoleHost::onRequestOfflineMsg(NetId netid,const char* buf, int size) {
		if(!netid || nullptr == buf ||
			!size) {
			return ;
		}

		string data;
		data.assign(buf,size);

		RequestOfflineMsgs request;
		request.ParseFromString(data);

		Role* role = RoleManage::getRoleManage()->getRoleByNetId(netid);
		if(nullptr == role) {
			return ;
		}
		
		int rid = role->GetRoleId();
		
		DbOperator::sendRequestOfflineMsgs(netid,rid,request);

		NetworkManage::getNetworkManage()->Send2LogServer(ConnectType::LOGSERVER,
			LogLevel::I, __func__, "rid:" + Util::int2str(rid) + "  rrid:" + Util::int2str(request.rrid()));
	}
}