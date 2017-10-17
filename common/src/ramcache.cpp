#include "./../include/ramcache.h"

namespace engine {

	namespace cache {
		
		RamCache::RamCache(void){
			this->onInit();
			this->m_timeout = 300;
		}

		RamCache::~RamCache(void){
			this->cleanCache();
		}

		RamCache::RamCache(const RamCache& cache){
			this->cleanCache();
			this->m_timeout = 300;

			int i = 0;
			for(i = 0; i < RAMCACHE_LEN; i++){
				if(nullptr != cache.m_cache[i].m_data){
					this->m_cache[i].m_rid = cache.m_cache[i].m_rid;
					this->m_cache[i].m_key = cache.m_cache[i].m_key;
					this->m_cache[i].m_dirt = cache.m_cache[i].m_dirt;
					this->m_cache[i].m_uptime = cache.m_cache[i].m_uptime;
					this->m_cache[i].m_size = cache.m_cache[i].m_size;
					
					this->m_cache[i].m_data = new char[cache.m_cache[i].m_size + 1];
					if(nullptr == this->m_cache[i].m_data){
						this->m_cache[i].m_size = 0;

						continue;
					}

					memcpy(this->m_cache[i].m_data,cache.m_cache[i].m_data,cache.m_cache[i].m_size);
				}
			}
		}

		RamCache& RamCache::operator=(const RamCache& cache){
			if(this == &cache){
				return *this ;
			}

			this->cleanCache();

			int i = 0;
			for(i = 0; i < RAMCACHE_LEN; i++){
				if(nullptr != cache.m_cache[i].m_data){
					this->m_cache[i].m_rid = cache.m_cache[i].m_rid;
					this->m_cache[i].m_key = cache.m_cache[i].m_key;
					this->m_cache[i].m_dirt = cache.m_cache[i].m_dirt;
					this->m_cache[i].m_uptime = cache.m_cache[i].m_uptime;
					this->m_cache[i].m_size = cache.m_cache[i].m_size;

					this->m_cache[i].m_data = new char[cache.m_cache[i].m_size + 1];
					if(nullptr == this->m_cache[i].m_data){
						this->m_cache[i].m_size = 0;

						continue;
					}

					memcpy(this->m_cache[i].m_data,cache.m_cache[i].m_data,cache.m_cache[i].m_size);
				}
			}

			return *this;
		}

		void RamCache::onInit(void){
			int i = 0;
			for(i = 0; i < RAMCACHE_LEN; i++){
				this->m_cache[i].m_data = nullptr;
				this->m_cache[i].m_free = nullptr;
				this->m_cache[i].m_next = nullptr;
				this->m_cache[i].m_dirt = 0;
				this->m_cache[i].m_size = 0;
				this->m_cache[i].m_rid = 0;
				this->m_cache[i].m_uptime = 0;
			}
		}

		void RamCache::onStart(void){

		}

		void RamCache::onStop(void){

		}

		void RamCache::cleanCache(void){
			int i = 0;
			for(i = 0; i < RAMCACHE_LEN; i++){
				if(nullptr != this->m_cache[i].m_data){
					delete this->m_cache[i].m_data;

					this->m_cache[i].m_data = nullptr;
				}

				this->m_cache[i].m_rid = 0;
				this->m_cache[i].m_key = "";
				this->m_cache[i].m_dirt = 0;
				this->m_cache[i].m_uptime = 0;
				this->m_cache[i].m_size = 0;
			}
		}

		unsigned int RamCache::cacheSize(void){
			int i = 0;
			int num = 0;
			for(i = 0; i < RAMCACHE_LEN; i++){
				if(nullptr != this->m_cache[i].m_data){
					num = num + 1;
				}
			}

			return num;
		}

		bool RamCache::IsExists(string key,int* index){
			if(0 >= key.size()){
				*index = -1;

				return false;
			}

			int i = 0;

			for(i = 0; i < RAMCACHE_LEN; i++){
				if(nullptr != this->m_cache[i].m_data){
					if(0 == key.compare(this->m_cache[i].m_key)){
						*index = i;

						return true;
					}
				}
			}

			*index = -1;

			return false;
		}

		bool RamCache::IsExists(int key,int* index){
			if(0 >= key){
				*index = -1;

				return false;
			}

			int i = 0;

			for(i = 0; i < RAMCACHE_LEN; i++){
				if(nullptr != this->m_cache[i].m_data){
					if(this->m_cache[i].m_rid == key){
						*index = i;

						return true;
					}
				}
			}

			*index = -1;

			return false;
		}

		unsigned short 	RamCache::emptyIndex(void){
			unsigned int i = 0;
			int uptime = 0x0fffffff;
			int index = 0;

			for(i = 0; i < RAMCACHE_LEN; i++){
				if(nullptr == this->m_cache[i].m_data){
					return i;
				}

				//	寻找最早的缓存项
				if(this->m_cache[i].m_uptime < uptime){
					uptime = this->m_cache[i].m_uptime;
					index = i;
				}
			}

			if(nullptr != this->m_cache[index].m_data){
				delete this->m_cache[index].m_data;

				this->m_cache[index].m_size = 0;
				this->m_cache[index].m_data = nullptr;
			}

			return index;
		}

		short	RamCache::indexByKey(string key){
			if(0 == key.size()){
				return -1;
			}

			int i = 0;
			for(i = 0; i < RAMCACHE_LEN; ++i){
				if(nullptr != this->m_cache[i].m_data){
					if(0 == key.compare(this->m_cache[i].m_key)){
						return i;
					}
				}
			}

			return -1;
		}

		short	RamCache::indexByKey(int key){
			if(0 >= key){
				return -1;
			}

			int i = 0;
			for(i = 0; i < RAMCACHE_LEN; ++i){
				if(nullptr != this->m_cache[i].m_data){
					if(this->m_cache[i].m_rid == key){
						return i;
					}
				}
			}

			return -1;
		}

		void RamCache::setTimeout(int timeout /* = 300 */) {
			if(0 >= timeout) {
				timeout = 300;
			}

			this->m_timeout = timeout;
		}

		int RamCache::getTimeout(void) {
			return this->m_timeout;
		}

		void RamCache::addCache(string key,char* cache,unsigned short size){
			if (nullptr == cache || 0 >= key.size()){
				return ;
			}

			int _index = 0;
			if(this->IsExists(key,&_index)){
				return ;
			}

			unsigned int index = this->emptyIndex();
			this->m_cache[index].m_key = key;
			this->m_cache[index].m_dirt = 1;
			this->m_cache[index].m_size = 0;
			
			this->m_cache[index].m_data = new char[size + 1];
			if(nullptr == this->m_cache[index].m_data){
				this->m_cache[index].m_dirt = 0;

				return ;
			}

			this->m_cache[index].m_size = size;
			this->m_cache[index].m_uptime = Datetime::getTimestamp();

			memcpy(this->m_cache[index].m_data,cache,size);

			return ;
		}

		void RamCache::addCache(int key,char* cache,unsigned short size){
			if (nullptr == cache || 0 >= key){
				return ;
			}

			int _index = 0;
			if(this->IsExists(key,&_index)){
				return ;
			}

			unsigned int index = this->emptyIndex();
			this->m_cache[index].m_rid = key;
			this->m_cache[index].m_dirt = 1;
			this->m_cache[index].m_size = 0;
			
			this->m_cache[index].m_data = new char[size + 1];
			if(nullptr == this->m_cache[index].m_data){
				this->m_cache[index].m_dirt = 0;

				return ;
			}

			this->m_cache[index].m_size = size;
			this->m_cache[index].m_uptime = Datetime::getTimestamp();

			memcpy(this->m_cache[index].m_data,cache,size);

			return ;
		}

		void RamCache::updateCache(string key,char* cache,unsigned short size){
			if(nullptr == cache || 0 >= key.size()){
				return ;
			}

			int index = this->indexByKey(key);
			if(-1 == index){
				return ;
			}

			delete this->m_cache[index].m_data;
			this->m_cache[index].m_data = nullptr;

			this->m_cache[index].m_key = key;
			this->m_cache[index].m_size = 0;
			this->m_cache[index].m_data = new char[size + 1];
			if(nullptr == this->m_cache[index].m_data){
				return ;
			}

			this->m_cache[index].m_size = size;
			this->m_cache[index].m_uptime = Datetime::getTimestamp();

			memcpy(this->m_cache[index].m_data,cache,size);

			return ;
		}

		void RamCache::updateCache(int key,char* cache,unsigned short size){
			if(nullptr == cache || 0 >= key){
				return ;
			}

			int index = this->indexByKey(key);
			if(-1 == index){
				return ;
			}

			delete this->m_cache[index].m_data;
			this->m_cache[index].m_data = nullptr;

			this->m_cache[index].m_rid = key;
			this->m_cache[index].m_size = 0;
			this->m_cache[index].m_data = new char[size + 1];
			if(nullptr == this->m_cache[index].m_data){
				return ;
			}

			this->m_cache[index].m_size = size;
			this->m_cache[index].m_uptime = Datetime::getTimestamp();

			memcpy(this->m_cache[index].m_data,cache,size);

			return ;
		}

		void RamCache::addOrUpdateCache(string key,char* cache,unsigned short size){
			if(nullptr == cache || 0 >= key.size()){
				return ;
			}

			int index = 0;
			if(this->IsExists(key,&index)){
				if(-1 == index){
					return ;
				}

				delete this->m_cache[index].m_data;

				this->m_cache[index].m_data = nullptr;
			}else {
				index = this->emptyIndex();
			}

			this->m_cache[index].m_key = key;
			this->m_cache[index].m_size = 0;
			this->m_cache[index].m_data = new char[size + 1];
			if(nullptr == this->m_cache[index].m_data){
				return ;
			}

			this->m_cache[index].m_size = size;
			this->m_cache[index].m_dirt = 1;
			this->m_cache[index].m_uptime = Datetime::getTimestamp();

			memcpy(this->m_cache[index].m_data,cache,size);
		}

		void RamCache::addOrUpdateCache(int key,char* cache,unsigned short size){
			if(nullptr == cache || 0 >= key){
				return ;
			}

			int index = 0;
			if(this->IsExists(key,&index)){
				if(-1 == index){
					return ;
				}

				delete this->m_cache[index].m_data;

				this->m_cache[index].m_data = nullptr;
			}else {
				index = this->emptyIndex();
			}

			this->m_cache[index].m_rid = key;
			this->m_cache[index].m_size = 0;
			this->m_cache[index].m_data = new char[size + 1];
			if(nullptr == this->m_cache[index].m_data){
				return ;
			}

			this->m_cache[index].m_size = size;
			this->m_cache[index].m_dirt = 1;
			this->m_cache[index].m_uptime = Datetime::getTimestamp();

			memcpy(this->m_cache[index].m_data,cache,size);
		}

		unsigned short RamCache::getCache(string key,char* cache){
			if(nullptr == cache || 0 >= key.size()){
				cache = nullptr;

				return 0;
			} 

			int index = 0;
			if(this->IsExists(key,&index)){
				if(-1 == index || nullptr == this->m_cache[index].m_data){
					cache = nullptr;

					return 0;
				}

				//	缓存项仅仅存在5分钟
				if(this->m_cache[index].m_uptime < (Datetime::getTimestamp() - this->m_timeout)){
					this->removeCache(key);
					
					cache = nullptr;

					return 0;
				}
				
				memcpy(cache,this->m_cache[index].m_data,this->m_cache[index].m_size);

				return this->m_cache[index].m_size;
			}

			cache = nullptr;

			return 0;
		}

		unsigned short RamCache::getCache(int key,char* cache){
			if(nullptr == cache || 0 >= key){
				cache = nullptr;

				return 0;
			} 

			int index = 0;
			if(this->IsExists(key,&index)){
				if(-1 == index || nullptr == this->m_cache[index].m_data){
					cache = nullptr;

					return 0;
				}

				//	缓存项仅仅存在5分钟
				if(this->m_cache[index].m_uptime < (Datetime::getTimestamp() - this->m_timeout)){
					this->removeCache(key);
					
					cache = nullptr;

					return 0;
				}
				
				memcpy(cache,this->m_cache[index].m_data,this->m_cache[index].m_size);

				return this->m_cache[index].m_size;
			}

			cache = nullptr;

			return 0;
		}

		void RamCache::removeCache(string key){
			if(0 >= key.size()){
				return ;
			}
			
			int index = 0;
			if(this->IsExists(key,&index)){
				if(-1 == index){
					return ;
				}

				if(nullptr != this->m_cache[index].m_data){
					delete this->m_cache[index].m_data;
					
					this->m_cache[index].m_data = nullptr;

					this->m_cache[index].m_rid = 0;
					this->m_cache[index].m_key = "";
					this->m_cache[index].m_size = 0;
					this->m_cache[index].m_dirt = 0;
					this->m_cache[index].m_uptime = 0;
				}
			}

			return ;
		}

		void RamCache::removeCache(int key){
			if(0 >= key){
				return ;
			}
			
			int index = 0;
			if(this->IsExists(key,&index)){
				if(-1 == index){
					return ;
				}

				if(nullptr != this->m_cache[index].m_data){
					delete this->m_cache[index].m_data;
					
					this->m_cache[index].m_data = nullptr;

					this->m_cache[index].m_rid = 0;
					this->m_cache[index].m_key = "";
					this->m_cache[index].m_size = 0;
					this->m_cache[index].m_dirt = 0;
					this->m_cache[index].m_uptime = 0;
				}
			}

			return ;
		}
	}
}