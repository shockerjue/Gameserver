#ifndef H_NETWORKMANAGE_H_
#define H_NETWORKMANAGE_H_

#include "type.h"
#include "./../util/util.h"
#include "./../ipc/ipc.pb.h"
#include "./../include/type.h"
#include "./../network/network.h"
#include "./../msgdefine/msgcode.h"
#include "./../msgdefine/msgheader.h"
#include "./../include/configmodule.h"

using namespace util;
using namespace config;
using namespace network;
using namespace msghandle;

namespace engine {
	class NetworkManage {
	public:
		~NetworkManage(void);

		/**
		* 通过网络id获取玩网络对象
		*
		* @param netid
		*/
		Network 	getNetwork(NetId netid);

		/**
		* 检查对应的netid是否已经存在
		*/
		bool 		isHavNetwork(NetId netid);

		/**
		* 通过连接类型获取网络对象
		*
		* @para type
		*/
		Network 	getNetworkByType(ConnectType type);

		/**
		* 更新网络连接的类型
		* 
		* @param network
		* @param type
		*/
		void 		updateConnectType(Network network,ConnectType type);
		
		/**
		* send data to machine for type
		*
		* @param type
		* @param msgheader
		* @param buf
		* @param size
		*/
		void		SendByNetworkType(ConnectType type,int msgheader,
			const char* buf,int size);

		/**
		* send data to machine for type
		*
		* @param network
		* @param msgheader
		* @param buf
		* @param size
		*/
		void		SendByNetwork(Network network,int msgheader,const char* buf,
			int size);

		/**
		* send data to machine for type
		*
		* @param netid
		* @param msgheader
		* @param buf
		* @param size
		*/
		void		SendByNetId(NetId netid,int msgheader,const char* buf,
			int size);

		/**
		* 将信息发送到日志服务器中
		*
		* @param level 	日志的级别
		* @param _func	日志产生的函数
		* @param msg 	日志信息
		*/
		void 		Send2LogServer(ConnectType type,LogLevel level,string _func,string msg);


		static NetworkManage* getNetworkManage(void);

		/**
		* remove network form manage with netid
		*
		* @param netid
		*/
		static void removeNetwork(NetId netid);
		/**
		* remove network from manage
		*
		* @param network
		*/
		static void	 removeNetwork(Network network);

		/**
		* add network to manager
		*
		* @param network
		*/
		static void	 addNetwork(Network network);

	private:
		NetworkManage(void);
		NetworkManage(const NetworkManage&){}
		NetworkManage& operator=(const NetworkManage&){ return *this; }

		void addNetType(NetId netid,ConnectType type);
		void remvoeNetType(NetId netid,ConnectType type);
		NetId getNetworkId(ConnectType type);
		void freeNetType(void);
		void initNetType(void);

	private:
		// default network
		Network		m_network;
		
		static map<NetId,Network>	s_manager;
		static NetworkManage* 	s_networkManage;

		struct TypeToNetwork{
			NetId 					m_netid;
			struct TypeToNetwork* 	m_pNext;
		};

		struct TypeToNetwork* m_t2network[TypeLen];
	};

}

#endif