/************************************************************************
* 所有与gateway的交互都在这里处理(接收来自gateway的请求以及响应请求)
*
* author : shockerjue
*************************************************************************/

#ifndef H_GATEWAYHANDLE_H
#define H_GATEWAYHANDLE_H

#include "./../dbhost/dbhandle.h"
#include "./../../../common/ipc/ipc.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../gameserver/gameserverhandle.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../../../common/include/cachemanage.h"


using namespace msghandle;
using namespace engine::cache;


namespace loginhost {
	class GatewayHandle {
	public:
		/**
		* 当收到gateway发送请求时调用，有缓存则直接响应，没有缓存时
		* 则发送请求到db服务
		* 
		* @param 	network 	请求的gateway对应Netwotk
		* @param 	buf			请求的数据
		* 
		* @return   true:像db发送验证请求	
		*/
		static void onLoginVerification(NetId netid,const char* buf,int size);

		/**
		* 收到gateway创建角色的请求时回调
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onCreateRole(NetId netid,const char* buf,int size);

		/**
		* 验证通过时，调用通知gatweway验证状态
		*
		* @param buf
		* @param size
		* @param key	玩家登陆需要使用的key
		* @param ip 	玩家登陆游戏连接的服务器ip
		* @param port	玩家登陆游戏连接的服务器端口
		*/
		static bool responseWithLogin(const char* buf,int size,char key[],
			const char* ip,unsigned short port);

		/**
		* 验证通过时，调用通知gatweway创建状态
		*
		* @param buf
		* @param size
		* @param key	玩家登陆需要使用的key
		* @param ip 	玩家登陆游戏连接的服务器ip
		* @param port	玩家登陆游戏连接的服务器端口
		*/
		static void responseWithCreateRole(const char* buf,int size,char key[],
			const char* ip,unsigned short port);
	private:

	};
}

#endif