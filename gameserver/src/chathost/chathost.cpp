#include "chathost.h"

namespace chathost {
	void ChatHost::notify2chatserverforlogin(Network network){
		if(network.GetType() == ConnectType::NONEDEFAULT) {
			RoleManage* rmanage = RoleManage::getRoleManage();
			Role* role = rmanage->getRoleByNetId(network.GetNetId());
			if(nullptr == role){
				return ;
			}

			inner_ipc::PlayerOnlineIPC online;
			online.set_rid(role->GetRoleId());
			online.set_gulid(0);
			online.set_uname(role->GetName());

			string data;
			online.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length() + 1);
			memcpy(_data,data.c_str(),data.length());

			NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::CHATSERVER,
				_MsgType_Gameserver_Chatserver::SS_PLAYER_ONLINE_NOTIFICATION,
				_data,data.length());

			Log::Print(__func__,online.rid());

			return ;
		}

		return ;
	}

	void ChatHost::notify2chatserverforlogout(Network network){
		if(network.GetType() == ConnectType::NONEDEFAULT){
			RoleManage* rmanage = RoleManage::getRoleManage();
			Role* role = rmanage->getRoleByNetId(network.GetNetId());
			if(nullptr == role){
				return ;
			}

			inner_ipc::PlayerOfflineIPC offline;
			offline.set_rid(role->GetRoleId());
			offline.set_isexit(1);
			
			string data;
			offline.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length() + 1);
			memcpy(_data,data.c_str(),data.length());

			NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::CHATSERVER,
				_MsgType_Gameserver_Chatserver::SS_PLAYER_OFFLINE_NOTIFICATION,
				_data,data.length());

			Log::Print(__func__, offline.rid());

			return ;
		}

		return ;
	}

	void ChatHost::onSendToRole(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);

		SendChat chat;
		chat.ParseFromString(data);

		ChatHost::sendToChatServer(chat,_MsgType_Gameserver_Chatserver::SS_GS_SEND_ROLE_MSG_TO_CS,
			engine::ChatType::PEER2PEER);

		Log::Print(__func__,chat.rid());

		return ;
	}

	void ChatHost::onSendToGuild(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);

		SendChat chat;
		chat.ParseFromString(data);

		ChatHost::sendToChatServer(chat,_MsgType_Gameserver_Chatserver::SS_GS_SEND_GUILD_MSG_TO_CS,
			engine::ChatType::PEER2GUILD);

		return ;
	}

	void ChatHost::onSendToWorld(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);

		SendChat chat;
		chat.ParseFromString(data);

		ChatHost::sendToChatServer(chat,_MsgType_Gameserver_Chatserver::SS_GS_SEND_WORLD_MSG_TO_CS,
			engine::ChatType::PEER2WORLD);

		return ;
	}

	void ChatHost::onSendToRoleFromChatServer(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);

		inner_ipc::ChatMsgIPC chat;
		chat.ParseFromString(data);

		ChatHost::sendToRole(chat,S_MsgType::CS_ROLE_TO_GAMESERVER_CHAT_SEND_ROLE);

		Log::Print(__func__,chat.stime());

		return ;
	}

	void ChatHost::onSendToGuildFromChatServer(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);

		inner_ipc::ChatMsgIPC chat;
		chat.ParseFromString(data);

		ChatHost::sendToRole(chat,S_MsgType::CS_ROLE_TO_GAMESERVER_CHAT_SEND_GUILD);

		return ;
	}

	void ChatHost::onSendToWorldFromChatServer(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);
		
		inner_ipc::ChatMsgIPC chat;
		chat.ParseFromString(data);

		ChatHost::sendToRole(chat,S_MsgType::CS_ROLE_TO_GAMESERVER_CHAT_SEND_GUILD);

		return ;
	}

	void ChatHost::sendToRole(inner_ipc::ChatMsgIPC chat,HEADER header){
		if(0 >= chat.size()){
			return ;
		}

		Role* drole = RoleManage::getRoleManage()->getRoleByRid(chat.did());
		if(nullptr == drole){
			return ;
		}

		RecvChat	rchat;
		rchat.set_type((int)chat.type());
		rchat.set_rid(chat.did());
		rchat.set_sid(chat.srid());
		rchat.set_tid(chat.tid());
		rchat.set_msg_type((int)chat.msg_type());
		rchat.set_msg_size(chat.size());
		rchat.set_sname(chat.sname());
		rchat.set_msg(chat.msg());

		string data;
		rchat.SerializeToString(&data);
		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetId(drole->GetNetId(),
			header,_data,data.length());

		// Log::Print(__func__, drole->GetNetId());

		NetworkManage::getNetworkManage()->Send2LogServer(ConnectType::LOGSERVER,
			LogLevel::I,__func__,rchat.sname());
		
		return ;
	}

	void ChatHost::sendToChatServer(SendChat chat,int header,
		engine::ChatType type){
		if(0 >= chat.msg_size()){
			return ;
		}

		inner_ipc::ChatMsgIPC cmsg;
		cmsg.set_srid(chat.rid());
		cmsg.set_did(chat.did());
		cmsg.set_type((int)type);
		cmsg.set_msg_type((int)chat.msg_type());
		cmsg.set_tid(0);
		cmsg.set_size(chat.msg_size());
		cmsg.set_status(0);
		cmsg.set_stime(Datetime::getTimestamp());
		cmsg.set_msg(chat.msg());

		string data;
		cmsg.SerializeToString(&data);
		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::CHATSERVER,
			header,_data,data.length());

		Log::Print(__func__,cmsg.stime());

		return ;
	}
}