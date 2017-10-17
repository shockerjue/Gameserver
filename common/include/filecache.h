/************************************************************************
文件缓冲处理

存储的方式是：索引文件 + 内容文件

索引：{
	char 				m_key[20];		//	缓存项的key
	char				m_file[128];	//	缓存内容所存储的位置文件
	int 				m_offset;		//	内容在文件中的位置
	int 				m_size;			//	内容的大小
	int 				m_dirt;			//	如果等于1，那说明这是无效数据，已经被移除
	long 				m_uptime;		//	缓存项的更新时间
}

内容：
	{
		内如；
	}


内存中只存储索引。

由于移除数据会导致性能的影响，所以采用非有效数据的方
式重置数据，将该数据设置为无效状态。

注：移除缓存项仅仅是更新缓存索引，对于缓存内容的文件不进行操作
*************************************************************************/

#ifndef H_FILECACHE_H_
#define H_FILECACHE_H_

#ifdef _cplusplus
    extern "C"{
#endif
	#include <string.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <unistd.h>
#ifdef _cplusplus
    }
#endif

#include <iostream>
#include "cachemodule.h"
#include "./../util/log.h"
#include "./../util/datetime.h"

using namespace std;
using namespace util;

namespace engine {

	namespace cache {
		
		const string	CACHE_FILE_PREFFIX = "/login";
		const string	INDEX_CACHE_SUFFIX = "_index.bin";
		const string 	CONTE_CACHE_SUFFIX = "_conte.bin";
		const int 		MAX___CACHE___SIZE = 1024 * 1024 * 4;	// 4MB 一个文件最大缓存数 

		class FileCache : public CacheModule {
		private:
			struct FileItem {
				char 				m_key[20];		//	缓存项的key
				char				m_file[128];	//	缓存内容所存储的位置文件
				int 				m_offset;		//	内容在文件中的位置
				int 				m_size;			//	内容的大小
				int 				m_dirt;			//	如果等于1，那说明这是无效数据，已经被移除
				long 				m_uptime;		//	缓存项的更新时间
				struct FileItem* 	m_pNext;		//	指向缓存项的下一项
			};

			typedef struct FileItem CacheItem;

		public:
			/**
			* @param path
			*/
			FileCache(string path);
			~FileCache();

			virtual void onInit(void);
			virtual void onStart(void);
			virtual void onStop(void);

			/**
			* 清除索引文件的内容以及内存中的管理结构
			*/
			virtual void cleanCache(void);

			/**
			* 获取缓存项的数量
			*/
			virtual unsigned int cacheSize(void);

			/**
			* 这里不操作文件的内容，仅仅是将对应的索引项标记为脏状态
			*/
			virtual void removeCache(string key) final;

			/**
			* 获取缓存项
			* 
			* @param 	key 	缓存键值
			* @param 	cache 	缓存内容
			*/
			virtual unsigned short getCache(string key,char* cache) final;


			/**
			* 将需要缓存的信息存储到缓存文件中
			*
			* @param	key 	缓存的键值
			* @param 	cache 	缓存的内容
			* @param 	size 	缓存项的大小
			*/
			virtual void addCache(string key,char* cache,unsigned short size) final;

			/**
			* 如果对应的缓存项存在，则更新该缓存项
			*
			* @param 	key 	缓存项的键值
			* @param 	cache 	缓存的内容
			* @param 	size 	缓存项的尺寸
			*/
			virtual void updateCache(string key,char* cache,unsigned short size) final;

			/**
			* 如果缓存项存在则更新，如果不存在则添加
			*
			* @param 	key 	缓存对应的键值
			* @param 	cache 	缓存的内容
			* @param 	size 	缓存的大小
			*/
			virtual void addOrUpdateCache(string key,char* cache,unsigned short size) final;

		private:
			/**
			* 从文件中加载缓存管理项（索引项）到内存中
			*/
			void loadCache(void);

			/**
			* 将缓存索引存储到文件中，数据不更新的原
			* 因是因为缓存数据是实时更新的
			*/
			void updateToFile(void);

			/**
			* 检查有没废弃的缓存域,如果有则将其赋值给cache
			*
			* @param cache
			*/
			bool hasEmpty(struct FileItem* cache);

			/**
			* 检查对应key的缓存是否存在，如果存在则赋值给cache
			* 
			* @param key
			* @param cache
			*/
			bool isExsits(string key,CacheItem* cache);

			FileCache(const FileCache& cache){}
			FileCache& operator=(const FileCache& cache){return *this;}

		private:
			int 		m_fd;
			int 		m_size;
			string 		m_path;

			//	缓存项链表的头指针
			CacheItem* 		m_pHead;
			//	缓存项链表的尾指针	
			CacheItem*		m_pLast;
		};
	}

}

#endif