#include "event_manage.h"

namespace chatserver {
	namespace event{
		void EventManage::registerMsgHandle(void){
			RegistMsgHandle::registerMsgHandle(S_SystemType::SS_SERVER_LOGIN,
				SystemServer::onServerRegister);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Chatserver::SS_PLAYER_ONLINE_NOTIFICATION,
				GsHost::onPlayerOnline);
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Chatserver::SS_PLAYER_OFFLINE_NOTIFICATION,
				GsHost::onPlayerOffline);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Chatserver::SS_GS_SEND_ROLE_MSG_TO_CS,
				GsHost::onSendToRole);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Chatserver::SS_GS_SEND_GUILD_MSG_TO_CS,
				GsHost::onSendToGuild);
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Chatserver::SS_GS_SEND_WORLD_MSG_TO_CS,
				GsHost::onSendToWorld);
		}

		void EventManage::unregisterMsgHandle(ProtocolHeader msg_type){
			RegistMsgHandle::unregisterMsgHandle(msg_type);
		}

		void EventManage::registerThreadMsgHandle(void){
			//To Do RegistMsgHandle::registerThreadMsgHandle(thread_what,func);
		}

		void EventManage::unregisterThreadMsgHandle(ThreadWhat thread_what){
			RegistMsgHandle::unregisterThreadMsgHandle(thread_what);
		}
	}
}