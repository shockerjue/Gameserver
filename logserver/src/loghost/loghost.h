#ifndef H_LOGHOST_H_
#define H_LOGHOST_H_

#include "./../logrw/logrw.h"
#include "./../../../common/ipc/ipc.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"


using namespace util;
using namespace engine;
using namespace inner_ipc;

namespace logserver {
	class LogHost {
	public:
		static void onRecvLog(NetId netid,const char* buf, const int size);
	};

}

#endif