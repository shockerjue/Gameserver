#include "event_manage.h"

namespace gateway {
	
	namespace event{
		void EventManage::registerMsgHandle(void){
			//To Do RegistMsgHandle::registerMsgHandle(msg_typs,func);
			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_ROLE_TO_GATEWAY,
				RoleHost::onLoginVerifiticaion);
			
			RegistMsgHandle::registerMsgHandle(S_MsgType::CS_ROLE_TOGATEWAY_CREATEROLE,
				RoleHost::onCreateRole);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gateway_Login::SS_GATEWAY_REQUEST_LOGIN_VERI,
				LoginHost::onLoginVeritification);

			RegistMsgHandle::registerMsgHandle(_MsgType_Gateway_Login::SS_GATEWAYTO_LOGIN_CREATEROLE,
				LoginHost::onCreateRole);
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