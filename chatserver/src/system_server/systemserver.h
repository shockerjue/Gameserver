#ifndef H_SYSTEMSERVER_H_
#define H_SYSTEMSERVER_H_

#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msghandle.h"

using namespace util;
using namespace engine;
using namespace msghandle;

class SystemServer {
public:
	/**
	* 系统服务注册回调函数,即当有服务连接到该服务的时候回调，用于
	* 处理注册服务器信息，一般是在连接以后的第一个数据包
	*
	* @param netid		注册的句柄
	* @param buf		注册的数据
	* @param size		数据的尺寸
	*/
	static void onServerRegister(NetId netid,const char* buf,const int size);
};

#endif