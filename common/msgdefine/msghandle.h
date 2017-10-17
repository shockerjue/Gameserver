/***********************************************************
* 注册消息或者是事件回调函数
* Author:shockerjue
*************************************************************/

#ifndef H_MSGHANDLE_H_
#define H_MSGHANDLE_H_

#include <map>
#include "./../include/type.h"
#include "./../engine/engine.h"
#include "./../include/thread.h"

using namespace std;
using namespace engine;
using namespace engine::thread;

namespace msghandle {
	class RegistMsgHandle {
	public:
		/**
		* 取消网络事件回调函数
		*
		* @param msg_type
		*/
		static void unregisterMsgHandle(ProtocolHeader );

		/**
		* 取消线程消息事件回调函数
		*
		* @param thread_what
		*/
		static void unregisterThreadMsgHandle(ThreadWhat );

		/**
		* 注册网络事件回调函数
		* 
		* @param msg_type 	事件类型（ID）
 		* @param func 		事件回调函数
		*/
		static void registerMsgHandle(ProtocolHeader ,EngineFunc );
		
		/**
		* 注册线程消息回调函数
		*
		* @parm thread_what 	线程消息what(类型)
		* @param func 			线程消息对应的回调函数
		*/
		static void registerThreadMsgHandle(ThreadWhat ,ThreadMsgFunc );
	};
}

#endif