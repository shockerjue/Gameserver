#ifndef H_THRAEDHANDLE_H_
#define H_THRAEDHANDLE_H_

#include "./../../../common/util/util.h"
#include "./../../../common/include/thread.h"

using namespace util;
using namespace engine::thread;

class ThreadHandle {
public:
	static void Msg_10000(Message);
	static void Msg_20000(Message);
	static void Msg_30000(Message);
};

#endif