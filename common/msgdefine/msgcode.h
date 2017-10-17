/**
* 定义了各种消息的协议头部
*/
#ifndef H_MSGCODE_H_
#define H_MSGCODE_H_

/**
* 内部的协议编号，主要是在不通服务器之间使用
*/
namespace msghandle {
	enum {
        SS_SYSTEM_CODE_NBEGIN				=	1,			//	系统消息编码开始
		SS_SYSTEM_CODE_END					=	100,		//	系统消息编码结束

		SS_THREAD_WHAT_BEGIN 				=	500,		//	线程通信what开始协议

		SS_LOGIN_TO_DB_BEGIN				=	1000,		//	login和db通信开始的协议号
		SS_GATEWAY_LOGIN_BEGINE				=	2000,		//	gateway和login通信开始的协议号
		SS_GAMESERVER_TO_DB_BEGIN			=	3000,		//	gameserver和db通信开始的协议号
		SS_GAMESERVER_TO_LOGIN_BEGIN		=	4000,		//	gameserver和login通信开始的协议
		SS_GAMESERVER_TO_GAMEWORLD_BEGIN	=	5000,		//	gameserver和gameworld通信开始的协议
		SS_GAMESERVER_TO_CHATSERVER_BEGIN	=	6000,		//	gameserver和chatserver通信开始的协议
		SS_GAMEWORLD_TO_DATAACCESS2_BEGIN	=	10000,		//	gameserver和dataacess通信起始协议
		SS_CHATSERVER_TO_DATAACCESS_BEGIN	=	11000,		//	chatserver和dataaccess通信的起始协议
		SS_CHATSERVER_TO_DATAACCESS2_BEGIN	=	12000,		//	chatserver和dataaccess2通信起始协议
	};

    enum S_SystemType {
        SS_SERVER_LOGIN         =   SS_SYSTEM_CODE_NBEGIN,              //  服务器注册
		SS_SERVER_LOG,													//	服务器系统日志，服务-->日志服务
		SS_SERVER_NOTI 													//	服务之间的通知
    };

    /**
    * 定义了线程消息的what code
    */
	enum S_WhatType {
		/**
		* 定义当网络连接以后检测该连接是否属于正常连接
		*/
		W_NOTI_NETID_INSERT					=	SS_THREAD_WHAT_BEGIN,	//	主线程通知ddos线程有新的连接
		W_NOTI_NETID_TIMEOUT,											//	ddos线程通知主线程对应的连接已经超时（ns后没有收到数据）
		W_NOTI_NETID_NORMAL,											//	主线程通知ddos线程，对应的netid正常
		W_NOTI_NETID_CLOSED,											//	主线程通知ddos线程，对应的netid被关闭
	
		/**
		* 定义监测接收数据超时信号
		*/
		W_NOTI_RECV_NORMAL,												//	接受到正常数据通知				
		W_NOTI_RECV_TIMEOUT,											//	接受数据超时通知
		W_NOTI_RECV_CLOSED,												//	接受数据被关闭	
	};

	/**
	* Gateway与Login服务直接的通信协议定义
	* [2000 ... 3000)
	*/
	enum _MsgType_Gateway_Login {
		SS_GATEWAY_REQUEST_LOGIN_VERI		=	SS_GATEWAY_LOGIN_BEGINE,

		SS_GATEWAYTO_LOGIN_CREATEROLE
	};

	/**
	* Login与DB服务直接的通信协议定义
	* [1000 .... 2000)
	*/
	enum _MsgType_Login_Db {
		SS_LOGIN_VERI_TO_DB					=	SS_LOGIN_TO_DB_BEGIN,				//	登录验证

		SS_LOGIN_TO_DB_CREATEROLE
	};

	/**
	* Login与gameserver之间的通信协议定义
	* [4000 .... 5000)
	*/
	enum _MsgType_Login_Gameserver {
		SS_LOGIN_NOTI_GAMESERVER			=	SS_GAMESERVER_TO_LOGIN_BEGIN,		//	登录通过时login服务通知gameserver服务

		SS_GAMESERVER_NOTI_CONNECT,													//	玩家连接gameserver时通知login
		SS_GAMESERVER_NOTI_CLOSED													//	玩家从gameserver断线时通知login
	};

	/**
	* Gameserver与Chatserver之间的通信协议定义
	* [6000 .... ]
	*/
	enum _MsgType_Gameserver_Chatserver {
		SS_PLAYER_ONLINE_NOTIFICATION		=	SS_GAMESERVER_TO_CHATSERVER_BEGIN,	//	当玩家上线的时候，gameserver通知chatserver服务
		SS_PLAYER_OFFLINE_NOTIFICATION,												//	当玩家下线的时候，gameserver通知chatserver服务
		SS_NOTI_PLAYER_ADD_FRIEND,													//	通知chatserver添加好友

		SS_GS_SEND_ROLE_MSG_TO_CS,													//	
		SS_GS_SEND_GUILD_MSG_TO_CS,
		SS_GS_SEND_WORLD_MSG_TO_CS
	};

	/**
	* gameserver与DB之间的通信协议定义
	* [3000 ... 4000)
	*/
	enum _MsgType_Gameserver_Db {
		SS_REQUEST_LOGIN_INFO				=	SS_GAMESERVER_TO_DB_BEGIN,			//	登陆的时候获取登陆信息

		SS_REQUEST_FRIENDS_LIST,													//	请求好友列表
		SS_REQUEST_ADD_FRIENDS,														//	添加好友

		SS_SAVE_LOGIN_INFO,															//	保存玩家登陆信息
		SS_REQUEST_MSG																//	消息{离线，系统}
	};

	/**
	* gameserver与gameworld1之间的通信协议定义
	* [5000 ... 6000]
	*/
	enum _MsgType_Gameserver_Gameworld1 {
		// SS_GAMESERVER_TO_GAMEWORLD_BEGIN
		SS_ROLE_ONENTER_GAMEWORLD 				=	SS_GAMESERVER_TO_GAMEWORLD_BEGIN,		//	玩家进入游戏
		SS_ROLE_ONEXIT_GAMEWORLD ,															//	玩家退出游戏
		SS_ROLE_ONDISCONNECTED,																//	玩家断开连接
		SS_ROLE_ONRECONNECTED,																//	玩家重新连接
		SS_ROLE_ONREQUEST_ROLES,															//	请求游戏服中的玩家信息
	};

	/**
	* gameserver与dataaccess2之间的通信协议定义
	* [10000 ... 11000]
	*/
	enum _MsgType_Gameserver_Dataaccess2 {
		// SS_GAMEWORLD_TO_DATAACCESS2_BEGIN
	};

	/**
	* chatserver与dataaccess之间通信协议的定义
	* [11000 ... 12000]
	*/
	enum _MsgType_Chatserver_Dataaccess {
		// SS_CHATSERVER_TO_DATAACCESS_BEGIN
		SS_SAVE_OFFLINE_MSG						=	SS_CHATSERVER_TO_DATAACCESS_BEGIN,		//	保存离线消息
	};

	/**
	* chatserver与dataaccess2之间通信的协议定义
	* [12000 ... 13000]
	*/
	enum _MsgType_Chatserver_Dataaccess2 {
		// SS_CHATSERVER_TO_DATAACCESS2_BEGIN
	};
}

#endif