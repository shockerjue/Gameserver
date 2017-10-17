#ifndef H_CHATHANDLE_H_
#define H_CHATHANDLE_H_

#include "./../../../common/ipc/ipc.h"
#include "./../playerpool/playerpool.h"
#include "./../../../common/engine/engine.h"

using namespace player;
using namespace engine;

namespace chatserver {
	class ChatHandle {
	public:
		/**
		* 广播数据给在线玩家
		*
		* @param tplid
		* @param buf 
		* @param size
		*/
		static void send2World(int msgheader,inner_ipc::ChatMsgIPC);

		/**
		* 广播信息给公会的所有玩家
		*
		* @param tplid
		* @param gid 
		* @param buf
		* @param size
		*/
		static void send2Guild(int gid,int msgheader,inner_ipc::ChatMsgIPC);

		/**
		* 发送信息给指定的玩家
		*
		* @param tplid
		* @param rid
		* @param buf
		* @param size
		*/
		static void send2Player(int rid,int msgheader,const char* buf,int size);

	private:

		static void send2(NetId netid,int msgheader,const char* buf,int size);
	};
}


#endif