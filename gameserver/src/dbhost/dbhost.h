/*********************************************************
*	数据读写操作的响应，与dboperator中的读写方法对应
*	这里主要是dboperator中读写完成的回到操作
*
* 	author:shockerjue
************************************************************/

#ifndef H_DBHOST_H_
#define H_DBHOST_H_

#include "./../keymanage.h"
#include "./../role/rolemanage.h"
#include "./../chathost/chathost.h"
#include "./../../../common/ipc/ipc.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../msghandle/gameserver_msgheader.pb.h"

using namespace role;
using namespace engine;
using namespace chathost;
using namespace msghandle;
using namespace gameserver;
using namespace gameserver_proto;

namespace dbhost {
	class DbHost {
	public:
		/**
		* 当db返回玩家登陆数据时回调
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRequestInfo(NetId netid,const char* buf,int size);

		/**
		* 获取背包数据回调方法
		*
		*/
		static void onRequestBag(NetId netid,const char* buf,int size);

		/**
		* 当请求好友信息数据返回时回调
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRequestFriends(NetId netid,const char* buf,int size);

		/**
		* 当添加好友时，DB返回调用
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRequestAddFriend(NetId netid,const char* buf,int size);

		/**
		* 收到chatserver返回来的离线消息
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onResponseOfflineMsg(NetId netid,const char* buf,int size);
	};
}

#endif