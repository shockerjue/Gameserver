#ifndef H_ETYPE_H
#define H_ETYPE_H

#include <map>
#include <iostream>
#include "./../util/util.h"
#include "./../include/thread.h"
#include "./../network/network.h"

using namespace std;
using namespace util;
using namespace engine::thread;
using namespace engine::network;


namespace engine {

	typedef int	ProtocolHeader;
	typedef void(*EngineFunc)(NetId,const char*,int);
	
	/**
	* 服务器之间注册信息
	*/
	struct NetworkType {
		unsigned int	m_type;
		unsigned int	m_port;
		char			m_ip[16];
		char 			m_name[32];

		NetworkType(void){
			this->m_type = 0;
			this->m_port = 0;
			memset(this->m_ip,0,16);
			memset(this->m_name,0,32);
		}

		NetworkType(const NetworkType& type){
			this->m_type = type.m_type;
			this->m_port = type.m_port;
			strncpy(this->m_ip,type.m_ip,16);
			strncpy(this->m_name,type.m_name,32);
		}

		NetworkType& operator=(const NetworkType& type){
			if(this == &type){
				return *this;
			}

			this->m_type = type.m_type;
			this->m_port = type.m_port;
			strncpy(this->m_ip,type.m_ip,16);
			strncpy(this->m_name,type.m_name,32);

			return *this;
		}

		string toString(void){
			char	buf[128] = {0};

			sprintf(buf,"{m_type: %d,m_port: %d, m_ip:%s ,m_name: %s}",
				this->m_type,
				this->m_port,
				this->m_ip,
				this->m_name);

			return string(buf);
		}
	};

	class EventHandle {
	public:
		static map<ProtocolHeader,EngineFunc>  	g_msg_call;
		static map<ThreadWhat,ThreadMsgFunc>  	g_thread_msg_call;
	};
}

#endif