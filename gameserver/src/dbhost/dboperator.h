/**********************************************************************
*	进行数据库的操作，即主要是发生数据读写操作到dataaccess服务中
*	理解为这里发出读写请求，之后当数据读写完成以后，将会异步的调
*	用dbhost中对应的响应方法
*
*	author: shockerjue
**********************************************************************/

#ifndef H_DBOPERATOR_H_
#define H_DBOPERATOR_H_

#include "./../../../common/ipc/ipc.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../msghandle/gameserver_msgheader.pb.h"

using namespace engine;
using namespace msghandle;
using namespace gameserver_proto;

namespace dbhost {
	class DbOperator {
	public:
		/**
		* 发送获取背包的请求
		*/
		static void sendRequestBag(NetId netid,int rid);

		/**
		* 发送获取角色信息请求到DB
		*
		* @param netid
		* @param rid
		* @param reqrid 	被请求的角色信息rid
		*/
		static void sendRequestRoleInfo(NetId netid,int rid,int reqrid);

		/**
		* 发送获取好友的请求到DB
		*
		* @param netid
		* @param rid
		* @param type
		*/
		static void sendRequestFriends(NetId netid,int rid,int type);

		/**
		* 发送添加好友请求到DB
		*
		* @param netid
		* @param rid		玩家rid
		* @param arid		要添加的玩家rid
		*/
		static void sendRequestAddFriends(NetId netid,int rid,int arid);

		/**
		* 发送请求离线消息请求到DB
		*
		* @param netid
		* @param rid
		* @param request
		*/
		static void sendRequestOfflineMsgs(NetId netid,int rid,RequestOfflineMsgs request);
	};
}

#endif
