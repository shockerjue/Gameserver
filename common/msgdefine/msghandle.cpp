#include "msghandle.h"

namespace msghandle {
	void RegistMsgHandle::registerMsgHandle(ProtocolHeader msg_type,
		EngineFunc func){
		if(!msg_type || nullptr == func){
			return ;
		}

		EventHandle::g_msg_call[msg_type] = func;
	}

	void RegistMsgHandle::unregisterMsgHandle(ProtocolHeader msg_type){
		if(!msg_type){
			return ;
		}

		EventHandle::g_msg_call.erase(msg_type);
	}

	void RegistMsgHandle::registerThreadMsgHandle(ThreadWhat thread_what,
		ThreadMsgFunc func){
		if(!thread_what || nullptr == func){
			return ;
		}

		EventHandle::g_thread_msg_call[thread_what] = func;
	}

	void RegistMsgHandle::unregisterThreadMsgHandle(ThreadWhat thread_what){
		if(!thread_what){
			return ;
		}

		EventHandle::g_thread_msg_call.erase(thread_what);
	}
}