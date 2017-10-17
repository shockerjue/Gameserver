/***********************************************************
    从数据库中读取数据	
**************************************************************/
#ifndef H_IOTHREAD_H_
#define H_IOTHREAD_H_

#ifdef _cplusplus
    extern "C"{
#endif
#ifdef __linux
	#include <fcntl.h>
	#include <sys/select.h>

#elif __APPLE__
	#include <fcntl.h>
	#include <sys/select.h>

#endif
#ifdef _cplusplus
    }
#endif

#include "read.h"
#include "sync.h"
#include "write.h"
#include "./../dbrw/mysqlslave.h"
#include "./../dbrw/mysqlmaster.h"
#include "./../../../common/ipc/ipc.h"
#include "./../sharecache/sharecache.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/include/value.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/include/thread.h"


using namespace dbrw;
using namespace util;
using namespace engine;
using namespace sharecache;
using namespace engine::thread;

namespace io_thread {

	/**
	* 从数据库中读取数据,当收到玩家
	*/
	Thread_Func(io_read);

	/**
	* 写数据库或者是更新到共享缓存中
	*/
	Thread_Func(io_write);

	/**
	* 同步共享内存中的数据到数据库中
	*/
	Thread_Func(io_sync);
}

#endif