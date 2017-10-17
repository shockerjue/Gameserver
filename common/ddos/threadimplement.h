#ifndef H_THREADIMPLMENET_H_
#define H_THREADIMPLMENET_H_

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

#include "./../util/util.h"
#include "./../include/type.h"
#include "./../include/thread.h"
#include "./../network/network.h"


using namespace util;
using namespace engine;
using namespace engine::thread;
using namespace engine::network;

namespace engine {

	namespace ddos {
		/**
		* 处理客户只连接不请求数据的操作，当某个用户仅仅是与服务器进行连接，
		* 而不发送任何请求数据，超过5s以后就会自动断开这个连接
		* @param ddos 	线程方法的名字
		*/
		Thread_Func(ddos);

		/**
		* 处理接收数据超时，当在ns以后没有收到数据，这个连接就断开，并触发
		* onTimeout回调
		* @param timeout 	线程方法的名字
		*/
		Thread_Func(timeout);

	}

}

#endif