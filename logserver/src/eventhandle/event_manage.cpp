#include "event_manage.h"

namespace logserver {
	namespace event{
		void EventManage::registerMsgHandle(void){
			RegistMsgHandle::registerMsgHandle(S_SystemType::SS_SERVER_LOGIN,
				SystemServer::onServerRegister);
			RegistMsgHandle::registerMsgHandle(S_SystemType::SS_SERVER_LOG,
				LogHost::onRecvLog);
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