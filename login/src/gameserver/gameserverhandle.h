/********************************************************************
* 作为gameserver服务器的接收请求和响应请求,所有与gameserver的交互都在这里处理
*
* athor: shockerjue
********************************************************************/
#ifndef H_GAMESERVERHANDLE_H_
#define H_GAMESERVERHANDLE_H_

#include "./../../../common/ipc/ipc.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../../../common/include/cachemanage.h"

using namespace util;
using namespace engine;
using namespace msghandle;
using namespace engine::cache;


namespace loginhost {
	struct LightestIpInfo {
		unsigned int 		m_port;
		char 				m_ip[16];
		char 				m_name[32];

		LightestIpInfo(void){
			this->m_port = 0;
			memset(this->m_ip,0,16);
			memset(this->m_name,0,32);
		}

		LightestIpInfo(const LightestIpInfo& info){
			this->m_port = info.m_port;
			strncpy(this->m_ip,info.m_ip,16);
			strncpy(this->m_name,info.m_name,32);
		}

		LightestIpInfo& operator=(const LightestIpInfo& info){
			if(this == &info){
				return *this;
			}

			this->m_port = info.m_port;
			strncpy(this->m_ip,info.m_ip,16);
			strncpy(this->m_name,info.m_name,32);

			return *this;
		}

		string toString(void){
			char buf[64] ={0};

			sprintf(buf, "{m_ip: %s , m_port: %d}",
				this->m_ip,this->m_port);

			return string(buf);
		}
	};

	class GameserverHandle {
	public:
		/**
		* 处理gameserver发送过来的通知，主要是通知有玩家上线/下线
		* 用于设置均衡相关信息
		*
		* @param netid
		* @param buf		通知的数据
		* @param size
		*/
		static void onHasIOServer(NetId netid,const char* buf,int size);

		/**
		* 获取玩家在线的统计信息
		* 
		* @param netid
		* @param buf
		* @param size
		*/
		static void onGetOnlineInfo(NetId netid,const char* buf,int size);

		/**
		* 通知gameserver信息，主要是通知玩家请求登录
		*
		* @param netid
		* @param buf 	
		* @param size
		* @param key 	
		*/
		static void notifyToIoServer(NetId netid,const char* buf,int size,char key[]);

		/**
		* 当创建以后，初始化玩家连接计数器
		*
		* @param netid 	gameserver's netid
		*/
		static void initCounter(NetId netid);

		/**
		* 增加gameserver计数器
		*
		* @param netid  gameserver's netid
		*/
		static void addCounter(NetId netid);

		/**
		* 获取服务器连接数
		*
		* @param netid
		*/
		static int  getCounter(NetId netid);

		/**
		* gameserver计数器减1
		*
		* @param netid   gameserver's netid
		*/
		static void subCounter(NetId netid);

		/**
		* 通过netid移除计数器
		*
		* @param netid 	gameserver's netid
		*/
		static void removeCounter(NetId netid);

		/**
		* 设置对应的netid信息
		*
		* @param netid
		* @param info
		*/
		static void setGameserverInfo(NetId netid,struct LightestIpInfo info);

		/**
		* 得到对应netid信息
		*
		* @param netid
		*/
		static struct LightestIpInfo getGameserverInfo(NetId netid);

		/**
		* 通过ip和port获取LightestIpInfo
		*
		* @param ip
		* @param port
		*/
		static struct LightestIpInfo getLightestByIpAPort(const char* ip,const int port);

		/**
		* 通过ip和port获取网络id
		*
		* @param ip
		* @param port
		*/
		static NetId getNetIdByIpAPort(const char* ip,const int port);

		/**
		* 得到负载最小的gameserver
		*
		* @param void
		*/
		static NetId getLightest(void);
	private:
		/**
		* counter
		*/
		static map<NetId,int> s_counter;

		/**
		* 存储对应NetId的主机信息，主要是用来通知玩家登陆的信息
		*/
		static map<NetId,struct LightestIpInfo> s_ipinfo;
	};
}

#endif