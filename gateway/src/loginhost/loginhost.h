/**************************************************************
* login服务器返回时调用的处理类，里面定义了login响应的回调方法，登录验证
*
* author : shockerjue
***************************************************************/

#ifndef H_LOGINHOST_H_
#define H_LOGINHOST_H_

#include "./../rolehost/rolehost.h"
#include "./../msghandle/msgcode.h"
#include "./../../../common/ipc/ipc.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../msghandle/gateway_msgheader.pb.h"


using namespace util;
using namespace engine;
using namespace gateway_proto;


namespace gateway {
	class LoginHost {
	public:
		/**
		* 发送登陆验证请求到login服务器
		*
		* @param buf
		* @param size
		*/
		static void sendLoginVeritification(const char* buf,const int size);

		/**
		* Login服务器返回登录请求回调
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onLoginVeritification(NetId netid,const char* buf,int size);

		/**
		* 发送创建玩家请求到login服务器
		*
		* @param buf
		* @param size
		*/
		static void sendCreateRole(const char* buf,const int size);

		/**
		* 创建角色时从login返回请求之后调用
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void  onCreateRole(NetId netid,const char* buf,int size);
	};
}

#endif