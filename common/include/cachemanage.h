/**
* 缓存管理器
*/

#ifndef H_CACHEMANAGE_H_
#define H_CACHEMANAGE_H_

#include "ramcache.h"
#include "filecache.h"

namespace engine{

	namespace cache {
		
		class CacheManage{
		public:
			~CacheManage(void);

			/**
			* 获取内存缓存管理器
			*/
			static RamCache* 	getRamCache(void);

			/**
			* 创建一个新的ramcache对象
			*/
			static RamCache 	getNewRamCache(void);

			/**
			* 获取cachemanage对象
			*/
			static CacheManage* getCacheManage(void);

			/**
			* 获取文件缓存管理对象
			*
			* @param path
			*/
			static FileCache* 	getFileCache(string path);

		private:
			CacheManage(void);
			CacheManage(const CacheManage&){}
			CacheManage& operator=(const CacheManage& cache){ return *this; }

			static CacheManage*		s_pCacheManage;
			static RamCache* 		s_pRamCache;
			static FileCache* 		s_pFileCache;
		};
	}
}
#endif