/**
* 先读取缓存中是否有对应的数据，如果没有则发送读请求到读数据线程中
*
* 
*/
#ifndef H_DBHOST_H_
#define H_DBHOST_H_

#include "./../dbrw/mysqlslave.h"
#include "./../dbrw/mysqlmaster.h"
#include "./../../../common/ipc/ipc.h"
#include "./../msghandle/inner_code.h"
#include "./../msghandle/thread_ipc.h"
#include "./../sharecache/sharecache.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/include/value.h"
#include "./../../../common/include/thread.h"
#include "./../../../common/ipc/gs_db_ipc.pb.h"
#include "./../../../common/include/shareram.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../../../common/include/cachemanage.h"
#include "./../../../common/include/configmodule.h"


using namespace util;
using namespace dbrw;
using namespace engine;
using namespace msghandle;
using namespace sharecache;
using namespace inner_code;
using namespace thread_ipc;
using namespace engine::cache;
using namespace engine::thread;
using namespace engine::config;
using namespace engine::shareram;


namespace dbhost {
	class DbHost {
	public:
		//////////下面的方法在主线程受客户的请求时如果有需要就将请求发送到Io线程中//////////////////////
		/**
		* gateway发送登陆请求到db验证是否有对应的玩家信息
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onLoginRequest(NetId netid,const char* buf,int size);

		/**
		* 当玩家请求创建角色的时候回调
		*
		* @param netid
		* @param buf
		* @parma size
		*/
		static void onCreateRole(NetId netid,const char* buf,int size);

		/**
		* gameserver请求获取玩家的信息{这里是已经登陆验证通过之后获取数据}
		* 读取所有玩家相关的信息
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRequestLoginInfo(NetId netid,const char* buf,int size);

		/**
		* 请求好友列表,读数据通过读线程读取
		*
		* @param netid
		* @param buf ReqFriendsIPC
		* @param size
		*/
		static void onRequestFriends(NetId netid,const char* buf,int size);

		/**
		* 添加好友
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRequestAddFriend(NetId netid,const char* buf,int size);

		/**
		* 保存玩家基本的信息
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onSaveRequestLoginInfo(NetId netid,const char* buf,int size);

		/**
		* 请求消息
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onRequestMsg(NetId netid,const char* buf,int size);

		//////////下面的方法是在读写线程中调用///////////////////////////////
		/**
		* 线程中收到请求登录的请求是调用
		*
		* @param param 
		* @param msg
		*/
		static THREAD_CALLBACK_FUNC void onLoginRequest(const void* param,Message msg) ;

		/**
		* 请求创建角色的时候调用
		*
		* @param param 
		* @param message
		*/
		static THREAD_CALLBACK_FUNC void onCreateRole(const void* param ,Message message);

		/**
		* 请求角色信息并响应请求，该方法是在读取缓存中不存在对应的数据时才会被回调
		*
		* @param param 
		* @param message
		*/
		static THREAD_CALLBACK_FUNC  void onRequestLoginInfo(const void* param,Message message);

		/**
		* 请求获取好友列表并响应请求，好友列表不进行缓存
		*
		* @param param 
		* @param message
		*/
		static THREAD_CALLBACK_FUNC void onRequestFriends(const void* param,Message message);

		/**
		* 请求添加好友
		*
		* @param param 
		* @param message
		*/
		static THREAD_CALLBACK_FUNC void onRequestAddFriend(const void* param,Message message);

		/**
		* 添加好友时，写线程收到写入好友信息时回调
		*
		* @param param 
		* @param message
		*/
		static THREAD_CALLBACK_FUNC void onAddFriendWrite(const void* param,Message message);

		/**
		* 请求消息时，发送消息到读线程中读
		*
		* @param param
		* @param message
		*/
		static THREAD_CALLBACK_FUNC void onRequestMsg(const void* param,Message message);
	};
}

#endif