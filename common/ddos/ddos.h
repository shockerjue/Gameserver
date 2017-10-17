/****************************************************************************
	ddos 预防操作
	author: shockerjue
****************************************************************************/

#ifndef H_DDOS_H
#define H_DDOS_H

#include "ddostype.h"

namespace engine {

	namespace ddos {
		
		class DDos : public DDosBase{
		public:
			DDos(void);
			~DDos(void);

			/**
			* 检查队列中的消息是否超时，
			* 如果超时发送通知到主线程中
			*
			* @param thread
			*/
			virtual void check(Thread thread);

			/**
			* 有新消息到达，根据消息类型进行处理
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
			* 主线程接收到超时消息回调函数
			*
			* @param message
			*/
			static void on_noti_timeout(Message message);

			/**
			* 主线程通知ddos线程有新消息到达
			*
			* @param thread 	需要通知的线程
			* @param netid 		通知的数据
			* @param timeout 	通知的超时时间
			* @param type 		通知的类型
			*/
			static void noti_ddos_thread(Thread thread,int netid,
				const int timeout,S_WhatType type);


			/**
			* 主线程通过线程名发送通知到ddos线程有新消息到达
			*
			* @param name 		通知的线程名
			* @param netid		通知的数据	
			* @param timeout	通知的超时时间
			* @param tyep		通知的类型
			*/
			static void noti_ddos_thread(const string name,int netid,
				const int timeout,S_WhatType type);
		};

	}

}

#endif