#include "event_manage.h"

namespace login {
	
	namespace event{
		void EventManage::registerMsgHandle(void){
			RegistMsgHandle::registerMsgHandle(S_SystemType::SS_SERVER_LOGIN,
				SystemServer::onServerRegister);
			
			RegistMsgHandle::registerMsgHandle(_MsgType_Gateway_Login::SS_GATEWAY_REQUEST_LOGIN_VERI,
				GatewayHandle::onLoginVerification);
			RegistMsgHandle::registerMsgHandle(_MsgType_Gateway_Login::SS_GATEWAYTO_LOGIN_CREATEROLE,
				GatewayHandle::onCreateRole);

			RegistMsgHandle::registerMsgHandle(_MsgType_Login_Gameserver::SS_GAMESERVER_NOTI_CONNECT,
				GameserverHandle::onHasIOServer);
			RegistMsgHandle::registerMsgHandle(_MsgType_Login_Gameserver::SS_GAMESERVER_NOTI_CLOSED,
				GameserverHandle::onHasIOServer);
			
			RegistMsgHandle::registerMsgHandle(_MsgType_Login_Db::SS_LOGIN_VERI_TO_DB,
				DbHandle::onGetVerification);
			RegistMsgHandle::registerMsgHandle(_MsgType_Login_Db::SS_LOGIN_TO_DB_CREATEROLE,
				DbHandle::onCreateRole);

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