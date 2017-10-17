#ifndef H_EVENT_MANAGE_H_
#define H_EVENT_MANAGE_H_

#include "./../role/rolehost.h"
#include "./../dbhost/dbhost.h"
#include "./../msghandle/msgcode.h"
#include "./../chathost/chathost.h"
#include "./../loginhost/loginhost.h"
#include "./../../../common/ddos/ddos.h"
#include "./../../../common/ddos/timeout.h"
#include "./../../../common/include/thread.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../../../common/msgdefine/msghandle.h"

using namespace dbhost;
using namespace engine;
using namespace chathost;
using namespace rolehost;
using namespace loginhost;
using namespace engine::thread;
using namespace engine::ddos;


namespace gameserver{

	namespace event{

		class EventManage {
		public:
			/**
			* 注册网络事件回调函数
			*/
			static void registerMsgHandle(void);

			/**
			* 注册线程消息回调函数
			*/
			static void registerThreadMsgHandle(void);
			
			/**
			* 取消网络事件回调函数
			*
			* @param msg_type
			*/
			static void unregisterMsgHandle(ProtocolHeader );

			/**
			* 取消线程消息事件回调函数
			*/
			static void unregisterThreadMsgHandle(ThreadWhat );
		};
	}

}

#endif