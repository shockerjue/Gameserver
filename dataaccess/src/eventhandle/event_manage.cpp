#include "event_manage.h"

namespace dataaccess {
	namespace event{
		void EventManage::registerMsgHandle(void){
			RegistMsgHandle::registerMsgHandle(S_SystemType::SS_SERVER_LOGIN,
				SystemServer::onServerRegister);
			
			RegistMsgHandle::registerMsgHandle(_MsgType_Login_Db::SS_LOGIN_VERI_TO_DB,
				DbHost::onLoginRequest);
			RegistMsgHandle::registerMsgHandle(_MsgType_Login_Db::SS_LOGIN_TO_DB_CREATEROLE,
				DbHost::onCreateRole);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Db::SS_REQUEST_LOGIN_INFO,
				DbHost::onRequestLoginInfo);
			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Db::SS_SAVE_LOGIN_INFO,
				DbHost::onSaveRequestLoginInfo);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Db::SS_REQUEST_FRIENDS_LIST,
			DbHost::onRequestFriends);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Db::SS_REQUEST_ADD_FRIENDS,
				DbHost::onRequestAddFriend);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gameserver_Db::SS_REQUEST_MSG,
				DbHost::onRequestMsg);

			// chatserver
			RegistMsgHandle::registerMsgHandle(_MsgType_Chatserver_Dataaccess::SS_SAVE_OFFLINE_MSG,
				ChatHost::onSaveOfflineMsg);
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