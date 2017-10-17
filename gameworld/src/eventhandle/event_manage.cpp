#include "event_manage.h"

namespace gameworld {
	namespace event{
		void EventManage::registerMsgHandle(void){
			//To Do RegistMsgHandle::registerMsgHandle(msg_type,func);
			RegistMsgHandle::registerMsgHandle(S_SystemType::SS_SERVER_LOGIN,
				SystemServer::onServerRegister);
			
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Gameworld1::SS_ROLE_ONENTER_GAMEWORLD,
				GameHost::onEnterGame);
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Gameworld1::SS_ROLE_ONEXIT_GAMEWORLD,
				GameHost::onExitGame);
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Gameworld1::SS_ROLE_ONDISCONNECTED,
				GameHost::onDisconnected);
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Gameworld1::SS_ROLE_ONRECONNECTED,
				GameHost::onReconnected);
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Gameworld1::SS_ROLE_ONREQUEST_ROLES,
				GameHost::onRequestRoles);
		}

		void EventManage::unregisterMsgHandle(ProtocolHeader msg_type){
			RegistMsgHandle::unregisterMsgHandle(msg_type);
		}

		void EventManage::registerThreadMsgHandle(void){
			//To Do RegistMsgHandle::registerThreadMsgHandle(thread_what,func);
			RegistMsgHandle::registerThreadMsgHandle(10000,ThreadHandle::Msg_10000);
			RegistMsgHandle::registerThreadMsgHandle(20000,ThreadHandle::Msg_20000);
			RegistMsgHandle::registerThreadMsgHandle(30000,ThreadHandle::Msg_30000);
		}

		void EventManage::unregisterThreadMsgHandle(ThreadWhat thread_what){
			RegistMsgHandle::unregisterThreadMsgHandle(thread_what);
		}
	}
}