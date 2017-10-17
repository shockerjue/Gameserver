#ifndef H_CACHEMODULE_H_
#define H_CACHEMODULE_H_

#include <iostream>

using namespace std;

namespace engine {

	namespace cache {
		
		class CacheModule{
		public:
			virtual void onStop(void) = 0;
			virtual void onInit(void) = 0;
			virtual void onStart(void) = 0;
			virtual void cleanCache(void) = 0;
			virtual void removeCache(string key) = 0;
			virtual unsigned int cacheSize(void) = 0;
			virtual unsigned short getCache(string key,
				char* cache) = 0;
			virtual void addCache(string key,char* cache,
				unsigned short size) = 0;
			virtual void updateCache(string key,char* cache,
				unsigned short size) = 0;
			virtual void addOrUpdateCache(string key,char* cache,
				unsigned short size) = 0;
		};
	}
}

#endif 