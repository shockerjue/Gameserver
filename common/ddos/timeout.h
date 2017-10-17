/********************************************************************
	超时处理操作，主要是用于接收数据超时的处理，在设置时间内超时以后触发超时
	回调函数，并将超时的网络释放

	author : shockerjue
*********************************************************************/
#ifndef H_TIMEOUT_H_
#define H_TIMEOUT_H_

#include "ddostype.h"
#include "./../util/util.h"
#include "./../include/thread.h"
#include "./../msgdefine/msgcode.h"


using namespace util;
using namespace msghandle;
using namespace engine::thread;

namespace engine {

	namespace ddos {
		
		class Timeout: public DDosBase{
		public:
			Timeout(void);
			~Timeout(void);

			/**
			* 检查队列中的消息是否超时，超时通知主线程
			*
			* @param thread
			*/
			virtual void check(Thread thread);

			/**
			* 有新的消息到达，根据消息类型进行处理
			* 
			* @param msg  消息对象
			*/
			virtual void new_msg(Message msg);

			/**
			* 有新消息到达
			*
			* @param msgs 	消息列表对象
			*/
			virtual void new_msg(vector<Message> msgs);

		public:
			/**
			* 主线程接收到超时消息回调函数，需要注册回调，如：
			* RegistMsgHandle::registerThreadMsgHandle(S_WhatType::W_NOTI_RECV_TIMEOUT,
			*	Timeout::on_noti_timeout);
			* 在当前线程中调用线程对象的send_message_to_main方法
			*
			* @param message
			*/
			static void on_noti_timeout(Message message);

			/**
			* 主线程通知timeout线程有新消息到达
			*
			* @param thread 	需要通知的线程
			* @param netid 		通知的数据
			* @param timeout 	通知的超时时间
			* @param type 		通知的类型
			*/
			static void noti_ddos_thread(Thread thread,int netid,
				const int timeout,S_WhatType type);


			/**
			* 主线程通知timeout线程有新消息到达
			*
			* @param name 	通知的线程名
			* @param netid
			* @param timeout
			* @param tyep
			*/
			static void noti_ddos_thread(const string name,int netid,
				const int timeout,S_WhatType type);
		};

	}

}

#endif