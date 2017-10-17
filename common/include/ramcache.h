/*****************************************************
	内存缓存管理，提前分配一定数量的内存区，
	在需要写入缓存的时候调用对应的方法。

	注：当分配的缓存满了以后，之后找最早更新的区域，然
	后替换该区域。
*******************************************************/

#ifndef H_RAMCACHE_H
#define H_RAMCACHE_H

#include <iostream>

#ifdef _cplusplus
    extern "C"{
#endif
	#include <string.h>
#ifdef _cplusplus
    }
#endif
    
#include "cachemodule.h"
#include "./../util/datetime.h"

using namespace util;
using namespace std;

namespace engine {
	
	namespace cache {
		/**
		* 内存缓存项的最大数量
		*/ 
		const unsigned short 	RAMCACHE_LEN = 3000;

		/**
		* 内存缓存管理类
		*/ 
		class RamCache: public CacheModule {
		public:
			RamCache(void);
			~RamCache(void);

			/**
			* 复制内存缓存对象
			* 
			* @param  cache
			*/
			RamCache(const RamCache& cache);

			/**
			* 赋值一个缓存类对象
			*
			* @param 	cache
			*/
			RamCache &operator=(const RamCache& cache);

			virtual void onInit(void);
			virtual void onStart(void);
			virtual void onStop(void);

			/**
			* 清除所有的缓存数据
			*/ 
			virtual void cleanCache(void);
			
			/**
			* 获取内存缓存的尺寸
			*
			* @return unsigned int
			*/
			virtual unsigned int cacheSize(void);

			/**
			* 添加数据到缓存中
			* 
			* @param key
			* @param cache
			* @param size
			*/
			virtual void addCache(string key,char* cache,unsigned short size) final;

			/**
			*
			*/
			virtual void addCache(int key,char* cache,unsigned short size);

			/**
			* 更新缓存中对应key的数据
			* 
			* @param key
			* @param cache
			* @param size
			*/
			virtual void updateCache(string key,char* cache,unsigned short size) final;

			/**
			* 更新缓存中对应key的数据
			* 
			* @param key
			* @param cache
			* @param size
			*/
			virtual void updateCache(int key,char* cache,unsigned short size);

			/**
			* 如果对应key的缓存项不存在则添加，如果存在则更新
			*
			* @param key
			* @param cache
			* @param size
			*/
			virtual void addOrUpdateCache(string key,char* cache,unsigned short size) final;

			/**
			* 如果对应key的缓存项不存在则添加，如果存在则更新
			*
			* @param key
			* @param cache
			* @param size
			*/
			virtual void addOrUpdateCache(int key,char* cache,unsigned short size);

			/**
			* 获取对应key的缓存项
			*
			* @param key
			* @param cache
			*/
			virtual unsigned short getCache(string key,char* cache) final;

			/**
			* 获取对应key的缓存项
			*
			* @param key
			* @param cache
			*/
			virtual unsigned short getCache(int key,char* cache);

			/**
			* 从缓存对象中移除对应key的缓存
			*
			* @param key
			*/
			virtual void removeCache(string key) final;

			/**
			* 从缓存对象中移除对应key的缓存
			*
			* @param key
			*/
			virtual void removeCache(int key);

			/**
			* 设置缓存失效时间,默认为5分钟
			*
			* @parma timeout
			*/
			void 	setTimeout(int timeout = 300);

			/**
			* 获取缓存超时的时间
			*/
			int		getTimeout(void);
		private:
			/**
			* 判断对应的key值的缓存是否存在，如果存在并返回对应的索引值
			*
			* @param key 	
			* @param index
			*/
			bool IsExists(string key,int* index);

			/**
			* 获取空闲的缓存项索引
			*/
			unsigned short 	emptyIndex(void);

			/**
			* 判断对应的key值的缓存是否存在，如果存在并返回对应的索引值
			*
			* @param key 	
			* @param index
			*/
			bool IsExists(int key,int* index);
			

			/**
			* 根据缓存key获取缓存的索引
			*
			* @param key  
			*/
			short	indexByKey(string key);

			/**
			* 根据缓存key获取缓存的索引
			*
			* @param key  
			*/
			short	indexByKey(int key);

		private:
			int 	m_timeout;
			struct RamItem {
				string 			m_key;			//	缓存项对应的key
				int 			m_rid;			//	缓存对应的签名key
				int 			m_dirt;			//	缓存项是否是是有效的
				unsigned int 	m_size;			//	缓存项存储数据的大小
				char*			m_data;			//	缓存项存储的数据
				struct RamItem*	m_free;			//	
				struct RamItem*	m_next;			//	
				unsigned long 	m_uptime;		//	缓存更新时间	
			};

			typedef struct RamItem 	CacheItem;
			CacheItem				m_cache[RAMCACHE_LEN];
		};
	}

}

#endif