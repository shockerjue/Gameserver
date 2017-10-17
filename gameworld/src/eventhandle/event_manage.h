#ifndef H_EVENT_MANAGE_H_
#define H_EVENT_MANAGE_H_

#include "./../gamehost/gamehost.h"
#include "./../threadhandle/threadhandle.h"
#include "./../system_server/systemserver.h"
#include "./../../../common/include/thread.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../../../common/msgdefine/msghandle.h"

using namespace gamehost;
using namespace msghandle;
using namespace engine::thread;

namespace gameworld{

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