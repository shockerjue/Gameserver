#ifndef H_TYPE_H_
#define H_TYPE_H_

#define EPOLL_ERROR -1

#define THREAD_CALLBACK_FUNC	//	说明对应的方法是在线程中执行

namespace engine {
	/**
	* socket的类型
	*/
	enum ConnectType {
		MIN				= 1,

		NONEDEFAULT 	= 0,		//	默认句柄标识
		DB 				= 1,		// 	数据库服务句柄标识
		DB2 			= 2,		//	游戏和聊天服回写数据服务器
		LOGIN 			= 3,		//	登录服务器句柄标识
		GATEWAY 		= 4,		//	网关服务器句柄标识
		IOSERVER 		= 5,		//	IO服务器句柄标识
		GAMEWORLD 		= 6,		//	游戏服务器句柄标识
		CHATSERVER 		= 7,		//	聊天服务器句柄标识
		PLATOMSERVER	= 8,		//	平台验证服务器句柄标识
		LOGSERVER 		= 9,		//	日志服务器标识

		THREAD_TYPE 	= 10,		//	线程消息句柄标识

		MAX 			= 10
	};

	/**
	* 启动的服务器程序的类型
	*/
	enum ServerType {
		ST_DEFAULT				= 0,
		ST_DBSERVER 			= 1,
		ST_GAMEWORLD 			= 2,
		ST_CHATSERVER 			= 3,
		ST_GAMESERVER			= 4,
		ST_LOGINSERVER			= 5,
		ST_GATEWAYSERVER 		= 6,
		ST_LOGSERVER 			= 7
	};

	/**
	* chat's type
	*/
	enum ChatType {
		PEER2PEER			= 1,	// 玩家之间发生消息
		PEER2GUILD			= 2,	// 社团消息
		PEER2WORLD			= 3		// 世界消息
	};

	/**
	* msg type
	*/
	enum MsgType {
		AUDIO				= 1,
		EMOJI				= 2,
		TEXT				= 3
	};

	enum STATUS {
		_SUCCESS	= 200,
		_FAILED		=	400
	};

	/**
	* 日志等级
	*/ 
	enum LogLevel {
		I 		=	1,		//	正常信息
		W 		=	2,		//	警告信息
		E 		= 	3		//	错误信息
	};

	const unsigned short TypeLen = ConnectType::MAX + 1;

	typedef int 	HEADER;
}


#endif