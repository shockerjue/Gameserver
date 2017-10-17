#ifndef H_GDHOST_H_
#define H_GDHOST_H_

#include "./../dbhost/dbhost.h"
#include "./../../../common/ipc/ipc.h"
#include "./../chathandle/chathandle.h"
#include "./../playerpool/playerpool.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msgcode.h"


using namespace util;
using namespace dbhost;
using namespace player;
using namespace engine;
using namespace msghandle;
using namespace chatserver;

namespace gshost {
	class GsHost {
	public:
		/**
		* 玩家上线通知,主要是将玩家添加到在线队列中
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onPlayerOnline(NetId netid,const char* buf,int size);

		/**
		* 玩家下线通知
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onPlayerOffline(NetId netid,const char* buf,int size);

		/**
		* 发送消息到指定的玩家时调用
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onSendToRole(NetId netid,const char* buf,int size);

		/**
		* 将信息发送到指定的公会时调用，主要是找出对应公会的在线成员，之后将消息分发到所有玩具
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onSendToGuild(NetId netid,const char* buf,int size);

		/**
		* 收到世界公告时调用，主要是将消息发送到所有在线的玩家
		* 
		* @param netid
		* @param buf
		* @param size
		*/
		static void onSendToWorld(NetId netid,const char* buf,int size);

	private:
		/**
		* 将数据发送到gameserver
		*
		* @param chat
		* @param msg
		* @param header
		*/
		static void sendToGameserver(inner_ipc::ChatMsgIPC chat,HEADER header);
	};
}

#endif