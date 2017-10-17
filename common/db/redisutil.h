/*******************************************************************
* redis数据库操作，主要进行读写操作{set，get}，redis数据库操作使用hiredis库
*
* author: shockerjue
********************************************************************/

#ifndef H_REDISUTIL_H_
#define H_REDISUTIL_H_

#include "dbmodule.h"
#include "./../include/value.h"
#include "./../../3tdarty/hiredis/hiredis.h"

namespace engine {

	namespace db {
		
		class RedisUtil : public DBModule {
		public:
			RedisUtil(void);
			~RedisUtil(void);

			/**
			* 连接到redis服务器
			*
			* @return bool
			*/
			virtual bool InitConnection(void);

			/**
			* 使用主机地址和对应的端口链接redis
			* 
			* @param host
			* @param port
			* @return bool
			*/
			virtual bool InitConnection(const string,const int);	

			/**
			* 通过key获取对应的值
			*
			* @param key
			*/
			Value 	Get(string key);

			/**
			* 通过key和value将值设置到redis中
			*
			* @param key
			* @param value
			*/
			void  	Set(string , string );
			
			virtual bool can_connect(void);

		private:
			RedisUtil(const RedisUtil&){}
			RedisUtil& operator=(const RedisUtil&){ return *this; }
			
		private:
			redisContext* 	m_pConnect;
		};
	}

}
#endif