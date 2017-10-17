#include "event_manage.h"

namespace gameserver {
	namespace event{
		void EventManage::registerMsgHandle(void){
			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_REQUEST_LOGIN_TO_GAMESERVER,
				RoleHost::onRoleLogin);

			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_ROLE_TO_GAMESERVER_CHAT_SEND_ROLE,
				ChatHost::onSendToRole);

			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_ROLE_TO_GAMESERVER_CHAT_SEND_GUILD,
				ChatHost::onSendToGuild);

			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_ROLE_TO_GAMESERVER_CHAT_SEND_WORLD,
				ChatHost::onSendToWorld);

			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_ROLE_TO_GAMESERVER_CHAT_SEND_MSG,
				RoleHost::onRequestOfflineMsg);

			RegistMsgHandle::registerMsgHandle(_MsgType_Login_Gameserver::SS_LOGIN_NOTI_GAMESERVER,
				LoginHost::onNotifyfromlogin);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Db::SS_REQUEST_LOGIN_INFO,
				DbHost::onRequestInfo);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Chatserver::SS_GS_SEND_ROLE_MSG_TO_CS,
				ChatHost::onSendToRoleFromChatServer);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Chatserver::SS_GS_SEND_GUILD_MSG_TO_CS,
				ChatHost::onSendToGuildFromChatServer);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Chatserver::SS_GS_SEND_WORLD_MSG_TO_CS,
				ChatHost::onSendToWorldFromChatServer);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Db::SS_REQUEST_FRIENDS_LIST,
				DbHost::onRequestFriends);
			
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Db::SS_REQUEST_ADD_FRIENDS,
				DbHost::onRequestAddFriend);
			
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Db::SS_REQUEST_MSG,
				DbHost::onResponseOfflineMsg);
			
			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_REQUEST_FRIENDS_LIST,
				RoleHost::onRequestFriends);

			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_REQUEST_ROLE_INFO,
				RoleHost::onRequestRoleInfo);
			
			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_REQUEST_ADD_FRIEND,
				RoleHost::onRequestAddFriend);
			
		}

		void EventManage::unregisterMsgHandle(ProtocolHeader msg_type){
			RegistMsgHandle::unregisterMsgHandle(msg_type);
		}

		void EventManage::registerThreadMsgHandle(void){
			//To Do RegistMsgHandle::registerThreadMsgHandle(thread_what,func);
			RegistMsgHandle::registerThreadMsgHandle(S_WhatType::W_NOTI_NETID_TIMEOUT,
				DDos::on_noti_timeout);
			RegistMsgHandle::registerThreadMsgHandle(S_WhatType::W_NOTI_RECV_TIMEOUT,
				Timeout::on_noti_timeout);
		}

		void EventManage::unregisterThreadMsgHandle(ThreadWhat thread_what){
			RegistMsgHandle::unregisterThreadMsgHandle(thread_what);
		}
	}
}