#ifndef H_ECALLBACK_H_
#define H_ECALLBACK_H_

#include "type.h"
#include "networkmanage.h"
#include "./../util/util.h"
#include "./../ddos/timeout.h"
#include "./../include/type.h"
#include "./../network/network.h"
#include "./../msgdefine/msgcode.h"
#include "./../msgdefine/msgheader.h"

using namespace ddos;
using namespace util;
using namespace msghandle;
using namespace engine::network;

namespace engine {
	
	/**
	* 网络回调函数
	*/
	class NetworkCallback {
	public:
		/**
		* 连接关闭调用
		*
		* @param network
		*/
		virtual void onClose(Network network);

		/**
		* 网络超时回调（接收数据超时）
		*
		* @param network
		*/
		virtual void onTimeout(Network network);

		/**
		* 有新的连接回调
		*
		* @param network
		*/
		virtual void onConnect(Network network);
	};

	/**
	* 基本功能触发回调类
	*/
	class BaseCallback {
	public:
		/**
		* 接收线程消息时调用
		*
		* @param buf
		* @param len
		* @param callback = nullptr
		*/
		void onThreadMsg(const char* buf,const int len,
			NetworkCallback* callback = nullptr);

		/**
		* 收到数据时调用
		* 
		* @param network 		接收数据的network 
		* @param buf 			接收到的数据
		* @param size 			数据尺寸
		*/
		void onRecv(Network network,const char* buf,
			const int size);
	};

	/**
	* 引擎基本回调函数
	*/
	class EngineCallback {
	public:
		/**
		* 引擎初始化成功以后回调
		*/
		virtual void onInit(void);

		/**
		* 引擎停止回调
		*/
		virtual void onStop(void);

		/**
		* 引擎开始回调
		*/
		virtual void onStart(void);

		/**
		* 引擎装载配置文件回调
		*
		* @param _config 	配置文件名
		*/
		virtual void onLoadConfig(const string _config);
	};

}

#endif