/**
* 用于网络事件和线程事件的注册以及取消
* 只有在这里注册过才会发送
*/

#ifndef H_EVENT_MANAGE_H_
#define H_EVENT_MANAGE_H_

#include "./../msghandle/msgcode.h"
#include "./../rolehost/rolehost.h"
#include "./../loginhost/loginhost.h"
#include "./../../../common/ddos/ddos.h"
#include "./../../../common/ddos/timeout.h"
#include "./../../../common/include/thread.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../../../common/msgdefine/msghandle.h"

using namespace engine::ddos;
using namespace engine::thread;

namespace gateway{

	namespace event{

		class EventManage {
		public:
			/**
			* 注册网络事件
			*/
			static void registerMsgHandle(void);

			/**
			* 注册线程消息
			*/
			static void registerThreadMsgHandle(void);

			
			/**
			* 取消网络事件
			*
			* @param msg_type
			*/
			static void unregisterMsgHandle(ProtocolHeader);
			
			/**
			* 取消线程消息事件
			*
			* @param msg_type
			*/
			static void unregisterThreadMsgHandle(ThreadWhat );
		};
	}

}

#endif