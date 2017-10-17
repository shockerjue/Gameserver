#include "./../include/filecache.h"

namespace engine {

	namespace cache {
		
		FileCache::FileCache(string path){
			this->m_path = path;
			this->m_fd = 0;
			this->m_size = 0;
			this->m_pHead = this->m_pLast = new CacheItem();
			this->m_pHead->m_pNext = nullptr;

			this->loadCache();
		}

		FileCache::~FileCache(){
			this->updateToFile();

			if(0 < this->m_fd){
				close(this->m_fd);
			}

			if(nullptr != this->m_pHead){
				delete this->m_pHead;

				this->m_pHead = nullptr;
				this->m_pLast = nullptr;
			}
		}

		void FileCache::onInit(void){
			
		}

		void FileCache::loadCache(void){
			this->m_fd = open((this->m_path + CACHE_FILE_PREFFIX + 
				INDEX_CACHE_SUFFIX).c_str(),O_RDWR|O_CREAT,0644);
			if(-1 == this->m_fd){
				Log::Perror(__func__, "Create cache index file fail!");

				return ;
			}

			int i = 0;
			int ret = 0;

			//	每次从文件中读出完整的索引项，所以必须是管理项的整数倍，这里每次读取100个管理项
			int cache_size = sizeof(CacheItem) * 100;
			char* buf = new char[cache_size + 1];
			if(nullptr == buf){
				Log::Perror(__func__, "loadCache allocate fail!");

				return ;
			}

			CacheItem*	p = this->m_pHead;

			do {
				ret = read(this->m_fd,buf,cache_size);
				if(0 >= ret){
					break;
				}

				CacheItem* fq = (CacheItem*)buf;

				// 计算读取出的管理项数
				int number = ret / (sizeof(CacheItem));
				for(i = 0; i < number && nullptr != fq; i++,fq++){
					CacheItem* temp = new CacheItem;
					if(nullptr == temp){
						continue;
					}

					strcpy(temp->m_key,fq->m_key);
					strcpy(temp->m_file,fq->m_file);

					temp->m_offset = fq->m_offset;
					temp->m_size = fq->m_size;
					temp->m_uptime = fq->m_uptime;
					temp->m_dirt = 0;
					temp->m_pNext = nullptr;

					p->m_pNext = temp;

					p = temp;

					this->m_pLast = p;
					this->m_size = this->m_size + fq->m_size;

				}

				memset(buf,0,cache_size);
			}while(1);

			delete[] buf;
			buf = nullptr;

			return ;
		}

		void FileCache::onStart(void){

		}

		void FileCache::onStop(void){

		}

		void FileCache::cleanCache(void){
			CacheItem* p = this->m_pHead->m_pNext;

			//	释放链表
			while(nullptr != p){
				CacheItem* temp = p->m_pNext;
				delete p;

				p = temp;
			}

			this->m_size = 0;
			this->m_pLast = nullptr;
			this->m_pHead->m_pNext = nullptr;

			if(0 < this->m_fd){
				ftruncate(this->m_fd,0);
	    		lseek(this->m_fd,0,SEEK_SET);
			}

			return ;
		}

		unsigned int FileCache::cacheSize(void){
			return this->m_size;
		}

		bool FileCache::isExsits(string key,CacheItem* cache){
			if(0 == key.size()){
				cache = nullptr;

				return false;
			}

			CacheItem* p = this->m_pHead->m_pNext;
			while(nullptr != p){
				string _key(p->m_key);
				if(0 == _key.compare(key)){
					cache = p;

					return true;
				}

				p = p->m_pNext;
			}

			cache = nullptr;

			return false;
		}

		bool FileCache::hasEmpty(CacheItem* cache){
			CacheItem* p = this->m_pHead->m_pNext;
			while(nullptr != p){
				if(1 == p->m_dirt){
					cache = p;

					return true;
				}

				p = p->m_pNext;
			}

			cache = nullptr;

			return false;
		}

		void FileCache::addCache(string key,char* cache,unsigned short size){
			if(0 == key.size() || nullptr == cache){
				return ;
			}

			CacheItem* p = this->m_pHead->m_pNext;
			while(nullptr != p){
				string _key(p->m_key);
				if(0 == _key.compare(key)){
					break ;
				}

				p = p->m_pNext;
			}

			// 如果对应的key已经存在，则立即返回
			if(nullptr != p){
				return ;
			}

			CacheItem*	q;
			CacheItem* item;
			int offset = 0;
			int where = SEEK_END;

			string filename = this->m_path + CACHE_FILE_PREFFIX + CONTE_CACHE_SUFFIX;
			if((127) < filename.size()){
				Log::Perror(__func__, "FileCache filename too length!");

				return ;
			}

			//	已经有空闲的管理结构则可以直接使用
			//	同时更新管理项,且缓存的大小不能超过原来的大小
			if(this->hasEmpty(q)){
				if(q->m_size < size){
					Log::Perror(__func__,"Cachr is too big!");

					return ;
				}

				q->m_dirt = 0;
				q->m_size = size;
				strcpy(q->m_key,key.c_str());
				q->m_uptime = Datetime::getTimestamp();
				offset = q->m_offset;

				where = SEEK_SET;

			}else {
				item = new CacheItem();
				if(nullptr == item){
					return ;
				}

				strcpy(item->m_key,key.c_str());
				item->m_size = size;
				item->m_dirt = 0;
				item->m_uptime = Datetime::getTimestamp();
				item->m_offset = this->m_size;
				item->m_pNext = nullptr;

				strcpy(item->m_file,filename.c_str());

				if((this->m_size + size) > MAX___CACHE___SIZE){
					return ;
				}
				
				this->m_size = this->m_size + size;

			}

			int fd = open(filename.c_str(),O_RDWR|O_CREAT,0644);
			if(-1 == fd){
				return ;
			}

			lseek(fd,offset,where);
			int ret = write(fd,cache,size);
			if(0 >= ret){
				close(fd);

				return ;
			}

			if(nullptr == q){
				if(nullptr != this->m_pLast){
					this->m_pLast->m_pNext = item;
					this->m_size = this->m_size + size;
					this->m_pLast = item;

					close(fd);

					return ;
				}

				delete item;

				item = nullptr;
			}

			close(fd);

			return ;
		}

		void FileCache::updateCache(string key,char* cache,unsigned short size){
			if(0 == key.size() || nullptr == cache){
				return ;
			}

			CacheItem* p = this->m_pHead->m_pNext;
			while(nullptr != p){
				string _key(p->m_key);
				if(0 == _key.compare(key)){
					break ;
				}

				p = p->m_pNext;
			}

			if(nullptr == p){
				return ;
			}

			int fd = open(p->m_file,O_RDWR);
			if(-1 == fd){
				return ;
			}

			p->m_size = size;
			p->m_uptime = Datetime::getTimestamp();
			p->m_dirt = 0;

			char* buf = new char[size + 1];
			if(nullptr == buf){
				close(fd);

				return ;
			}

			memcpy(buf,cache,size);

			lseek(fd,p->m_offset,SEEK_SET);
			int ret = write(fd,buf,size);

			close(fd);

			delete[] buf;
			buf = nullptr;

			return ;
		}

		void FileCache::addOrUpdateCache(string key,char* cache,unsigned short size){
			if(0 == key.size() || nullptr == cache){
				return ;
			}

			CacheItem* p = this->m_pHead->m_pNext;
			while(nullptr != p){
				string _key(p->m_key);
				if(_key.compare(key)){
					break;
				}

				p = p->m_pNext;
			}

			if(nullptr == p){
				this->addCache(key,cache,size);
			}else {
				this->updateCache(key,cache,size);
			}

			return ;
		}

		unsigned short FileCache::getCache(string key,char* cache){
			if(0 == key.size()){
				cache = nullptr;

				return 0;
			}

			CacheItem* p = this->m_pHead->m_pNext;
			while(nullptr != p){
				string _key(p->m_key);
				if(0 == _key.compare(key)){
					break;
				}

				p = p->m_pNext;
			}

			if(nullptr == p){
				cache = nullptr;

				return 0;
			}

			int fd = open(p->m_file,O_RDONLY);
			if(-1 == fd){
				cache = nullptr;

				return 0;
			}

			char* buf = new char[p->m_size + 1];
			if(nullptr == buf){
				close(fd);

				cache = nullptr;

				return 0;
			}

			lseek(fd, p->m_offset, SEEK_SET);
			int ret = read(fd,buf,p->m_size);
			if(0 >= ret){
				cache = nullptr;
				close(fd);

				delete[] buf;
				buf = nullptr;

				return 0;
			}

			memcpy(cache,buf,p->m_size);

			delete[] buf;
			buf = nullptr;

			close(fd);

			return p->m_size;
		}

		void FileCache::removeCache(string key){
			if(0 == key.size()){
				return ;
			}

			CacheItem* p = this->m_pHead->m_pNext;
			while(nullptr != p){
				string _key(p->m_key);
				if(0 == _key.compare(key)){
					break;
				}

				p = p->m_pNext;
			}

			if(nullptr == p){
				return ;
			}

			p->m_dirt = 1;
			memset(p->m_key,0,20);

			return ;
		}

		void FileCache::updateToFile(void){
			if(0 >= this->m_fd){
				return ;
			}

			ftruncate(this->m_fd,0);
	    	lseek(this->m_fd,0,SEEK_SET);

	    	CacheItem* p = this->m_pHead->m_pNext;
	    	while(nullptr != p){
	    		CacheItem temp;

	    		strcpy(temp.m_key,p->m_key);
	    		strcpy(temp.m_file,p->m_file);

	    		temp.m_size = p->m_size;
	    		temp.m_dirt = p->m_dirt;
	    		temp.m_uptime = p->m_uptime;
	    		temp.m_offset = p->m_offset;
	    		temp.m_pNext = nullptr;

	    		int ret = write(this->m_fd,(char*)&temp,sizeof(CacheItem));

	    		p = p->m_pNext;
	    	}

	    	return ;
		}
	}

}