#include "./../include/cachemanage.h"

namespace engine {

	namespace cache {
		
		RamCache* 		CacheManage::s_pRamCache = nullptr;
		FileCache* 		CacheManage::s_pFileCache = nullptr;
		CacheManage*	CacheManage::s_pCacheManage = nullptr;
		

		CacheManage::CacheManage(void){

		}

		CacheManage::~CacheManage(void){
			if(nullptr != CacheManage::s_pRamCache){
				delete CacheManage::s_pRamCache;

				CacheManage::s_pRamCache = nullptr;
			}

			if(nullptr != CacheManage::s_pFileCache){
				delete CacheManage::s_pFileCache;

				CacheManage::s_pFileCache = nullptr;
			}
		}

		CacheManage*	CacheManage::getCacheManage(void){
			if(nullptr == CacheManage::s_pCacheManage){
				CacheManage::s_pCacheManage = new CacheManage();
			}

			return CacheManage::s_pCacheManage;
		}

		RamCache* 	CacheManage::getRamCache(void){
			if(nullptr == CacheManage::s_pRamCache){
				CacheManage::s_pRamCache = new RamCache();
			}

			return CacheManage::s_pRamCache;
		}

		RamCache 	CacheManage::getNewRamCache(void){
			RamCache cache;

			return cache;
		}

		FileCache* 	CacheManage::getFileCache(string path){
			if(nullptr == CacheManage::s_pFileCache){
				CacheManage::s_pFileCache = new FileCache(path);
			}

			return CacheManage::s_pFileCache;
		}
	}
}