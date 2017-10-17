#ifndef H_ROLEHOST_H_
#define H_ROLEHOST_H_

#include "./../keymanage.h"
#include "./../dbhost/dboperator.h"
#include "./../chathost/chathost.h"
#include "./../msghandle/msgcode.h"
#include "./../../../common/ipc/ipc.h"
#include "./../../../common/ddos/ddos.h"
#include "./../../../common/engine/engine.h"
#include "./../msghandle/gameserver_msgheader.pb.h"

using namespace dbhost;
using namespace engine;
using namespace chathost;
using namespace gameserver;
using namespace gameserver_proto;

namespace rolehost {
	class RoleHost{
	public:
		/**
		* 玩家登陆
		* 
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRoleLogin(NetId netid,const char* buf,int size);

		/**
		* 玩家离线
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRoleLogout(NetId netid,const char* buf,int size);

		/**
		* 请求玩家信息
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRequestRoleInfo(NetId netid,const char* buf,int size);
		
		/**
		* 请求好友列表
		*
		* @param netid
		* @param buf	RequestFrineds
		* @param size 
		*/ 
		static void onRequestFriends(NetId netid,const char* buf,int size);

		/**
		* 请求添加好友
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRequestAddFriend(NetId netid,const char* buf,int size);

		/**
		* 请求离线消息
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRequestOfflineMsg(NetId netid,const char* buf, int size);
	};
}

#endif