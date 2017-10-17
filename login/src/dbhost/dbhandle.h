/********************************************************************
* 作为访问数据存储服务器的实现
* 发送读取数据库的请求以及接收访问的数据
*
* author : shockerjue
*********************************************************************/
#ifndef H_DBHANDLE_H_
#define H_DBHANDLE_H_

#include "./../../../common/ipc/ipc.h"
#include "./../gateway/gatewayhandle.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../gameserver/gameserverhandle.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../../../common/include/cachemanage.h"


using namespace util;
using namespace engine;
using namespace msghandle;
using namespace engine::cache;


namespace loginhost {
	class DbHandle {
	public:
		/**
		* 像db发送验证信息
		*
		* @param buf
		* @param size
		*/
		static void GetVerification(inner_ipc::Gateway2LoginIPC to_login);

		/**
		* 像db发送创建角色的信息
		*/
		static void CreateRole(const char* buf,int size);

		/**
		* db返回验证登陆数据以后回调
		* 选择负载最小的服务响应玩家
		* 将玩家登陆请求通知到对应gameserver中
		* @param netid
		* @param buf
		* @param size
		*/
		static void onGetVerification(NetId netid,const char* buf,int size);

		/**
		* db返回创建角色时回调
		* 创建成功的话选择负载最小服务信息响应玩家
		* 创建成功将玩家创建的登录信息通知到对应的gameserver
		* @param netid
		* @param buf
		* @param size
		*/
		static void onCreateRole(NetId netid,const char* buf,int size);
	private:

	};
}

#endif