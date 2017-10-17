/**************************************************************
* 玩家请求登录时回调的类，其中定义了玩家不同请求的不同调用的处理方法
*
* author : shockerjue
***************************************************************/

#ifndef H_ROLEHOST_H_
#define H_ROLEHOST_H_

#include "./../platom/platom.h"
#include "./../msghandle/msgcode.h"
#include "./../loginhost/loginhost.h"
#include "./../../../common/ipc/ipc.h"
#include "./../../../common/ddos/ddos.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msgcode.h"
#include "./../msghandle/gateway_msgheader.pb.h"
#include "./../../../common/engine/networkmanage.h"


using namespace util;
using namespace platom;
using namespace engine;
using namespace gateway_proto;


namespace gateway {

	class RoleHost {
	public:
		/**
		* Role请求登录时回调，主要是进行平台认证以及将登录请求发送到login服务器
		* 步骤：先验证平台有效性;如果平台验证通过，则将请求发送到login进行信息
		* 验证,同时将该请求的句柄放入看管线程中进行管理。
		*
		* @param  netid
		* @param  buf
		* @param  size
		*/
		static void onLoginVerifiticaion(NetId netid,const char* buf,int size);

		/**
		* 响应玩家登陆请求
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void responseLogin(NetId netid,const char* buf,const int size);

		/**
		* 创建玩家时回调，里面主要进行平台认证以及将创建角色请求发送到login
		*
		* @param netid
		* @param buf
		* @param size
		*/
		static void onCreateRole(NetId netid,const char* buf,int size);

		/**
		* 响应创建玩家
		*
		* @param netid
		* @param buf
		* @parma size
		*/
		static void responseCreateRole(NetId netid,const char* buf,const int size);
	};
}

#endif