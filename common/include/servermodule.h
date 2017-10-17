#ifndef H_SERVERMODULE
#define H_SERVERMODULE

#include <map>
#include <iostream>

#include "imodule.h"
#include "thread.h"
#include "./../engine/engine.h"
#include "./../msgdefine/msghandle.h"

using namespace std;
using namespace thread;
using namespace msghandle;

namespace engine {
	
	class ServerModule : public Imodule {
	public:
		ServerModule(void);
		virtual ~ServerModule(void);

		void Run(void) final;
		void onInit(void) final;
		void onStop(void) final;
		void onStart(void) final;

		/**
		* 注册事件
		*/
		virtual void registerHandler(void) = 0;

		/**
		* 装载配置文件
		*/
		void onLoadConfig(const string file) final;

		inline void addThreadMsgEvent(Thread thread){
			this->m_pEngine->AddThreadEvent(thread);
		}

		inline void setNetworkCallback(NetworkCallback* callback){
			if(nullptr != callback){
				this->m_pNetworkCallback = callback;
			}
		}

		inline void setEngineCallback(EngineCallback* callback){
			if(nullptr != callback){
				this->m_pEngineCallback = callback;
			}
		}

		/**
		* 根据配置文件中的主机标识连接服务器
		*
		* @param hostsign 		服务器地址标识
		* @param portsign 		服务器端口标识
		* @param type 			服务器连接的类型
		* @param peerType 		通知对端连接类型
		*/
		void ConnectHostBySign(const char* hostsign,const char* portsign,
			ConnectType type,ConnectType peerType);

		/**
		* 使用服务器地址和端口连接服务
		*
		* @param ip 		服务器地址
		* @param port 		服务器端口
		* @param type 		连接的类型
		* @param peerType 	通知对端连接的类型
		*/
		void ConnectHostByIp(const char* ip,unsigned short port,ConnectType type,
			ConnectType peerType);

	protected:
		Engine* 	        m_pEngine;
		NetworkCallback* 	m_pNetworkCallback;
		EngineCallback* 	m_pEngineCallback;
	};

}
#endif