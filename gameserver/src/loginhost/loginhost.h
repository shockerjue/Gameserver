#ifndef H_LOGINHOST_H_
#define H_LOGINHOST_H_

#include "./../keymanage.h"
#include "./../../../common/ipc/ipc.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msgcode.h"


using namespace engine;
using namespace msghandle;
using namespace gameserver;


namespace loginhost {
	class LoginHost {
	public:
		/**
		* 客户端连接服务器时通知login服务器
		*
		* @param network
		*/
		static void notify2loginforconnect(Network network);

		/**
		* 客户端断开连接时通知login服务器
		*
		* @param network
		*/
		static void notify2loginforclose(Network network);

		/**
		* 玩家请求登陆验证时通知{主要是玩家请求gateway，通过时由login通知}
		* 接收到请求时的回调函数
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onNotifyfromlogin(NetId netid,const char* buf,int size);
	};
}

#endif