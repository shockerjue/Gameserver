#ifndef H_CHATHOST_H_
#define H_CHATHOST_H_

#include "./../role/role.h"
#include "./../role/rolemanage.h"
#include "./../msghandle/msgcode.h"
#include "./../../../common/ipc/ipc.h"
#include "./../../../common/util/util.h"
#include "./../../../common/ipc/ipc.pb.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../msghandle/gameserver_msgheader.pb.h"


using namespace role;
using namespace util;
using namespace engine;
using namespace msghandle;
using namespace gameserver;
using namespace gameserver_proto;


namespace chathost {
	class ChatHost {
	public:
		/**
		* 玩家登陆以后调用通知chatserver
		*
		* @param network
		*/
		static void notify2chatserverforlogin(Network network);

		/**
		* 玩家下线以后调用通知chatserver
		*
		* @param 	network
		*/
		static void notify2chatserverforlogout(Network network);

		/**
		* 收到客户端发送过来将通信信息发送给其中指定的玩家，同时将该请求转发到chatserver进行处理
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onSendToRole(NetId netid,const char* buf,int size);

		/**
		* 收到客户端发送过来将通信信息发送给公会的信息，同时将该请求转发到chatserver进行处理
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onSendToGuild(NetId netid,const char* buf,int size);

		/**
		* 收到客户端发送过来将通信信息发送给世界的信息，同时将该请求转发到chatserver进行处理
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onSendToWorld(NetId netid,const char* buf,int size);

		/*
		* 收到chatserver处理发送给玩家的信息，这里主要是将信息转发到具体的玩家
		*
		* @param 	netid
		* @param 	buf
		* @param 	size
		*/
		static void onSendToRoleFromChatServer(NetId netid,const char* buf,int size);

		/**
		* 收到chatserver处理的发送给公会的信息，这里主要是将信息转发到公会中具体的玩家
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onSendToGuildFromChatServer(NetId netid,const char* buf,int size);

		/**
		* 收到chatserver处理的发送给世界的信息，这里主要是将信息转发到具体的玩家
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onSendToWorldFromChatServer(NetId netid,const char* buf,int size);

	private:
		/**
		* 将聊天信息发送到指定的玩家
		* 
		* @param netid
		* @param buf
		* @param size
		*/
		static void sendToRole(inner_ipc::ChatMsgIPC chat,HEADER header);

		static void sendToChatServer(SendChat chat,HEADER header,
			engine::ChatType type);
	};
}

#endif
