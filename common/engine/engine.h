#ifndef H_ENGINE_H_
#define H_ENGINE_H_

#ifdef _cplusplus
    extern "C"{
#endif
	#include <signal.h>
#ifdef _cplusplus
    }
#endif

#include "type.h"
#include "callback.h"
#include "networkmanage.h"
#include "./../util/util.h"
#include "./../ddos/timeout.h"
#include "./../include/type.h"
#include "./../include/thread.h"
#include "./../include/imodule.h"
#include "./../network/network.h"
#include "./../msgdefine/msgcode.h"
#include "./../msgdefine/msgheader.h"
#include "./../include/configmodule.h"

using namespace ddos;
using namespace util;
using namespace config;
using namespace thread;
using namespace network;
using namespace msghandle;

namespace engine {
	
	class Engine: public Imodule{
	public:
		~Engine();

		virtual void onStart(void);
		virtual void onStop(void);
		virtual void onInit(void);
		/**
		* 制定启动服务器的类型启动引起服务
		*
		* @param stype
		*/
		virtual void Run(void);

		/**
		* 加载配置文件
		*
		* @para file
		*/
		virtual void onLoadConfig(const string file);

		/**
		* 连接到其他服务
		*
		* @param ip 		连接ip地址
		* @param port 		连接端口
		* @param type 		连接的类型
		* @param peerType	通知对端类型（告诉对端服务器连接它的服务器类型）
		*
		* @return bool
		*/
		bool ConnectByIp(const char* ip,unsigned short port,ConnectType type, 
			ConnectType peerType);

		/**
		* 通过配置文件中的名字连接到主机
		*
		* @param hostsign 	配置文件中具体的服务器host
		* @param portsign 	配置文件中具体的服务器port
		* @param type 		连接的类型
		* @param peerType 	通知对端这个连接的类型
		*/
		bool ConnectByName(const char* hostsign,const char* portsign,ConnectType type,
			ConnectType peerType);

		/**
		* 通过域名连接到主机
		* @param domain 	域名
		* @param port 		端口
		* @param type 		连接的类型
		* @param peerType 	通知对端这个连接的类型
		*/
		bool ConnectByDomain(const char* domain,unsigned short port,ConnectType type, 
			ConnectType peerType);

		/**
		* 通过NetworkAddress地址连接到主机
		* @param address 	NetworkAddress对象
		* @param port 		端口
		* @param type 		连接的类型
		* @param peerType 	通知对端这个连接的类型
		*/
		bool ConnectByAddress(NetworkAddress address,unsigned short port,ConnectType type, 
			ConnectType peerType);

		/**
		* 设置事件回调
		*
		* @param callback
		*/
		void SetMsgCallback(NetworkCallback* callback);
		void SetEngineCallback(EngineCallback* callback);

		/**
		* 添加线程消息事件句柄，调用此方法以后主线程就可以接
		* 收线程发送的消息,如果希望主线程收到线程的消息，那
		* 必须调用该方法将线程注册到主线程中
		*
		* @param thread
		*/
		void 	AddThreadEvent(Thread thread);

        static Engine* 			getEngine(void);
		static ServerType 	  	getServerType(void);
		static ConfigModule*  	getConfigModule(void);
        static void 			setServerType(ServerType type);

	private:
    	Engine(void);
		Engine(const Engine& engine){}
		Engine& operator=(const Engine& engine){ return *this; }
		
		/**
		* 接收网络连接请求时回调
		*/
		virtual void onAccept(void);

		/**
		* 接收网络数据时回调
		* 
		* @param netid 	有数据的网络ID
		*/
		virtual void onRecv(NetId netid);

		/**
		* 网络被关闭时回调
		*
		* @param netid 	被关闭的网络ID
		*/
		virtual void onClose(NetId netid);

		/**
		* 添加网络到事件列表中
		*
		* @param network
		*/
		virtual void addEvent(Network network);

		/**
		* 得到服务器名字
		*/
		virtual string getName(void);

		/**
		* 添加对端关闭的信号处理函数
		*
		* @param sig
		*/
		static void onSigpipe(int sig);
	private:
		int 					m_eventfd;
		Network 				m_network;

		BaseCallback*			m_pBaseCallback;
		EngineCallback* 		m_pEngineCallback;
		NetworkCallback*		m_pNetCallback;

        static Engine* 			s_pEngine;
		static char 			s_bindip[16];
		static unsigned short 	s_bindport;
		static ServerType 		s_servertype;
	};
}

#endif 