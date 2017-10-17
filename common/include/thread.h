/*******************************************************
* 线程的定义、线程与主线程的通信方法、线程与线程之间的消息传递
*
* athor: shockerjue
**********************************************************/

#ifndef H_THREAD_H_
#define H_THREAD_H_

#ifdef _cplusplus
    extern "C"{
#endif
#ifdef __linux
	#include <stdio.h>
	#include <string.h>
	#include <errno.h>
	#include <sys/types.h>
	#include <signal.h>
	#include <pthread.h>
    #include <sys/socket.h>
    #include <stdlib.h>

	typedef		pthread_t	ThreadId;
#elif __APPLE__
	#include <stdio.h>
	#include <string.h>
	#include <errno.h>
	#include <sys/types.h>
	#include <signal.h>
	#include <pthread.h>
    #include <sys/socket.h>
    #include <stdlib.h>
    
	typedef 	pthread_t	ThreadId;
#endif
#ifdef _cplusplus
    }
#endif

#include <set>
#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "./../util/util.h"

using namespace std;
using namespace util;

namespace engine {

	namespace thread {

		/**
		* 线程消息对象的定义，线程之间的消息通信就是传递该对象的过程
		*/
		class Message {
		public:
			Message(void);
			~Message(void);

			Message(const Message&);
			Message& operator=(const Message&);
			
			void 	setwhat(const int what);
			void 	setsize(const int size);

			int 	getwhat(void);
			int 	getsize(void);

			/**
			* 获取消息中的对象
			*
			* @param object
			*/
			void* getobject(void* object,int* size);

			/**
			* 获取消息实体数据
			*
			* @param object 消息实体
			* @param size
			*/
			void* 	getobject(void* object,int size);

			/**
			* 设置消息数据对象
			*
			* @param object	消息实体
			* @param size 	消息对象大小
			*/
			void 	setobject(const void* object,int size);

			string 	toString(void);
		private:
			/**
			* 消息类型
			*/
			int 	what; 	

			/**
			* 消息对象{实体}
			*/
			char* 	obj;

			/**
			* 消息大小，也就是obj的长度
			*/
			int 	size;

			friend class Thread;
		};

		typedef 	int 	MessagePipe;

		typedef		int 	ThreadWhat;

		typedef 	void(*ThreadMsgFunc)(Message message);

		/**
		* 线程执行实体调用宏
		*/
		#define Thread_Call(n) thread_func_##n
		
		/**
		* 线程实体定义宏
		*/
		#define Thread_Func(n) void* thread_func_##n(void* _thread)

		typedef  void (*Message_Func)(Message message); 

		/**
		* 线程执行实体指针
		*/
		typedef void*(*ThreadFunc)(void* thread);
		
		struct CounterItem {
			int _send;
			int _recv;

			CounterItem(void):_send(0),_recv(0){}

			CounterItem(const CounterItem& item){
				this->_send = item._send;
				this->_recv = item._recv;
			}

			CounterItem& operator=(const CounterItem& item){
				if(this == &item){
					return *this;
				}

				this->_send = item._send;
				this->_recv = item._recv;

				return *this;
			}
		};

		class Thread {
		public:
			Thread(void);
			Thread(const string name);
			~Thread(void);

			Thread(const Thread&);
			Thread& operator=(const Thread&);

			string get_name(void);
			void set_name(const string name);

			/**
			* 获取线程id
			*/
			ThreadId getthreadid(void);

			/**
			* 线程实体运行，由于线程对象在实体中也是存在的，为了
			* 爆炸线程能正常执行，在线程未结束以前不应该销毁线程
			* 对象，直到线程结束或者是销毁线程
			*
			* @param 	runnable
			*/
			void run(ThreadFunc runnable);

			/**
			* 运行线程，并指定传递参数
			*
			* @param runnable
			* @param obj
			*/
			void run(ThreadFunc runnable,void* obj);

			/**
			* 指定线程名启动线程
			*
			* @param name 	线程名
			* @param runnable
			*/
			void run(const string name,ThreadFunc runnable);

			/**
			* 指定线程名和参数启动线程
			*
			* @param name
			* @param runnable
			* @param obj
			*/
			void run(const string name,ThreadFunc runnable,void* obj);

			/**
			* 发送消息到主线程，如果主线程需要接收线程消息，
			* 需要注册
			*
			* @param  	message
			*/
			bool 			 send_message_to_main(Message message);

			/**
			* 从外部发送消息到线程内部
			*
			* @param 	message
			*/
			bool 			 send_message_to_thread(Message message);

			/**
			* 同步接收线程消息
			*
			* @param 	s  		消息数
			* @return vector 	消息列表
			*/
			vector<Message>  recv_message_in_thread(int* s);

			/**
			* 结束线程
			*/
			void kill(void);

			/**
			* 等待线程执行结束
			*/
			void wait(void);

			/**
			* 线程外部获取的IPC句柄，主要用于线程消息的处理
			* 运用在select、poll、epoll中
			*/
			int 	getrecvfd(void);

			/**
			* 线程内部获取接受和发送消息的句柄
			* 主要用于异步接收外部发送来的消息
			*/
			int 	getsendfd(void);

			/**
			* 检查name的线程是否存在
			*
			* @param name
			*/
			bool isexsits(const string name);

		private:
			void add_counter(void);

			void sub_counter(void);

			/**
			* 检验线程是否在执行状态
			*/
			bool alive(void);

		private:
			MessagePipe		m_send;					//	消息管道
			MessagePipe 	m_recv;					//	消息管道
			bool 			m_runnable;				//	线程是在运行
			ThreadId 		m_thread_id;			//	线程id
			string 			m_thread_name;			//	线程名

			static map<string,Thread> 					s_threads;
			static map<ThreadId,struct CounterItem > 	s_thread_counter;

		public:
			/**
			* 通过指定线程对象和线程实体运行线程
			*
			* @param thread
			* @param runnable
			*/
			static void srun(Thread thread,ThreadFunc runnable);

			/**
			* 使用名字为name的线程对象运行线程
			*
			* @param name
			* @param runnable
			*/
			static void srun(const string name,ThreadFunc runnable);

			/**
			* 指定线程对象和实体需要的参数运行线程
			*
			* @param thread
			* @param runnable
			* @param obj
			*/
			static void srun(Thread thread,ThreadFunc runnable,void* obj);

			/**
			* 通过线程名和指定要传递的参数运行线程
			*
			* @param name
			* @param runnable
			* @param obj
			*/
			static void srun(const string name,ThreadFunc runnable,void* obj);

			static void skill(Thread thread);
			static void swait(Thread thread);

			static void	skill(const string name);
			static void swait(const string name);

			/**
			* 通过线程名获取线程对象，如果没有对应的进程
			* 返回的是一个默认的线程对象
			*
			* @param 	name
			*/
			static Thread sget_thread_by_name(const string name);

			/**
			* 发送消息到指定的线程中
			*
			* @param thread
			* @param message
			*/
			static bool ssend_message_to_thread(Thread thread,Message message);

			/**
			* 发送消息到指定的线程中
			*
			* @param 	线程名
			* @param 	message
			*/
			static bool ssend_message_to_thread(const string name,Message message);

			/**
			* 同步接收线程消息，该方法会阻塞
			*
			* @param 	thread
			* @param 	s  消息数
			*/
			static vector<Message>  srecv_message_in_thread(Thread thread,int* s);

			/**
			* 同步接收线程消息,该方法会阻塞
			*
			* @param name
			* @param s
			*/
			static vector<Message>  srecv_message_in_thread(const string name,int* s);
		};
	}
}

#endif