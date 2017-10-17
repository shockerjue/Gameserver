#ifndef H_SYSTEMSERVER_H_
#define H_SYSTEMSERVER_H_

#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../gameserver/gameserverhandle.h"
#include "./../../../common/msgdefine/msghandle.h"

using namespace util;
using namespace engine;
using namespace msghandle;
using namespace loginhost;

class SystemServer {
public:
	/**
	* 系统服务注册回调函数
	*
	* @param netid
	* @param buf
	* @param size
	*/
	static void onServerRegister(NetId netid,const char* bud,const int size);
};

#endif