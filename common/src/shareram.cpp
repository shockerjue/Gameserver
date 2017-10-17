#include "./../include/shareram.h"

namespace engine {

	namespace shareram {
		
		ShareItem::ShareItem(void){

		}

		ShareItem::ShareItem(const ShareItem& share){
			this->rid = share.rid;
			this->lock = share.lock;
			this->update = share.update;
			this->size = share.size;
			this->total = share.total;
			this->data = share.data;
			this->utime = share.utime;
			this->dirt = share.dirt;
		}

		ShareItem& ShareItem::operator=(const ShareItem& share){
			if(this == &share){
				return *this;
			}

			this->rid = share.rid;
			this->lock = share.lock;
			this->update = share.update;
			this->size = share.size;
			this->total = share.total;
			this->data = share.data;
			this->utime = share.utime;
			this->dirt = share.dirt;

			return *this;
		}

		string ShareItem::toString(void){
			char buf[128] = {0};

			sprintf(buf,"{rid: %d, lock: %d, update: %d, size: %d, \
				total: %d, utime: %d, dirt: %d}",
				this->rid,
				this->lock,
				this->update,
				this->size,
				this->total,
				this->utime,
				this->dirt);

			return string(buf);
		}

		ShareRam* 	ShareRam::s_pShareRam = nullptr;

		ShareRam::ShareRam(int mkey):
			m_shareram(nullptr),
			m_shareramfd(-1),
			m_shareramkey(mkey),
			m_shareramsize(0),
			m_shareramblk(HASH_SIZE){
	
			int i = 0;
			for (i = 0; i < HASH_SIZE; ++i){
				this->m_hash[i] = nullptr;
			}
		}

		ShareRam::~ShareRam(void){
			if(nullptr != this->m_shareram){
				shmdt(this->m_shareram);

				this->m_shareram = nullptr;
			}
		}

		ShareRam* 	ShareRam::getShareRam(int mkey){
			if(nullptr == ShareRam::s_pShareRam){
				ShareRam::s_pShareRam = new ShareRam(mkey);
			}else {
				Log::Warning(__func__, "ShareRam have already create !");
			}

			return ShareRam::s_pShareRam;
		}

		bool ShareRam::create(int size,int len){
			if(!size || !len){
				return false;
			}

			if(len > HASH_SIZE || 0 >= len) {
				len = HASH_SIZE ;
			}

			//	创建一个固定大小固定数量的共享内存
			key_t key = ftok(".",this->m_shareramkey);
			int max = size * len +  len * sizeof(struct ShareItem);

			this->m_shareramfd = shmget(key,max,0666 | IPC_CREAT);
			if(-1 == this->m_shareramfd){
				Log::Perror(__func__,"Create Share ram fail!");

				return false;
			}

			this->m_shareram = shmat(this->m_shareramfd,nullptr,0);
			if(nullptr == this->m_shareram){
				Log::Perror(__func__,"shmat get share ram fail!");

				return false;
			}

			this->m_shareramsize = max;
			this->m_shareramblk = len;
			
			return true;
		}

		bool ShareRam::onReInit(int nsize){
			if(nullptr == this->m_shareram || !nsize){
				return false;
			}

			unsigned short 	hashindex = 0;
			void* end   = (char*)this->m_shareram + this->m_shareramsize - nsize;
			struct ShareItem* begin = (struct ShareItem*)(this->m_shareram);

			while(begin < end && hashindex < this->m_shareramblk){
				begin->rid = 0;
				begin->lock = 0;
				begin->update = 0;
				begin->size = nsize;
				begin->total = nsize;
				begin->utime = 0;
				begin->dirt = 0;
				begin->data = end;
				
				this->m_hash[hashindex++] = begin;

				begin++;
				end = (char*)end - nsize;
			}

			return true;
		}

		bool ShareRam::onLoad(int nsize){
			if(nullptr == this->m_shareram || !nsize){
				return false;
			}

			unsigned short 	hashindex = 0;
			void* end   = (char*)this->m_shareram + this->m_shareramsize - nsize;
			struct ShareItem* begin = (struct ShareItem*)(this->m_shareram);

			while(begin < end){
				this->m_hash[hashindex] = begin;
				this->m_hash[hashindex]->data = end;

				if(begin->rid){
					this->m_ridhid[begin->rid] = hashindex;
				}else {
					this->m_hash[hashindex]->rid = 0;
					this->m_hash[hashindex]->lock = 0;
					this->m_hash[hashindex]->update = 0;
					this->m_hash[hashindex]->size = nsize;
					this->m_hash[hashindex]->utime = 0;
					this->m_hash[hashindex]->dirt = 0;
					this->m_hash[hashindex]->total = nsize;
				}

				begin++;
				hashindex++;
				
				end = (char*)end - nsize;
			}

			return true;
		}

		void* ShareRam::save(int rid,void* data,int size){
			if(!rid || nullptr == data || !size){
				return nullptr;
			}

			return this->save(rid,0,data,size);
		}

		void* ShareRam::save(int rid,int pos,void* data,int size){
			if(!rid || nullptr == data || !size){
				return nullptr;
			}

			if(!pos) {
				pos = 0;
			}

			int i = 0;
			for (i = 0; i < this->m_shareramblk; ++i){
				if(nullptr != this->m_hash[i]){
					if(0 == this->m_hash[i]->rid){
						break;
					}
				}
			}

			if(i == this->m_shareramblk){
				Log::Perror(__func__, "Didn't empty ram!");

				return nullptr;
			}

			if((pos + size) > this->m_hash[i]->total){
				Log::Perror(__func__, "Save data more than!");

				return nullptr;
			}

			this->m_ridhid[rid] = i;
			this->m_hash[i]->rid = rid;
			memcpy((void*)((char*)(this->m_hash[i]->data) + pos) ,data,size);
			this->m_hash[i]->lock = 0;
			this->m_hash[i]->utime = 0;
			this->m_hash[i]->dirt = 0;

			return data;
		}

		bool ShareRam::remove(int rid){
			if(!rid){
				return false;
			}

			map<int,int>::iterator it = this->m_ridhid.find(rid);
			if(it == this->m_ridhid.end()){
				return false;
			}

			int index = it->second;
			if(!index) {
				return false;
			}

			memset(this->m_hash[index]->data,0,this->m_hash[index]->total);

			this->m_hash[index]->rid = 0;
			this->m_hash[index]->lock = 0;
			this->m_hash[index]->update = 0;
			this->m_hash[index]->utime = 0;
			this->m_hash[index]->dirt = 0;

			this->m_ridhid.erase(rid);

			return true;
		}

		void* ShareRam::getram(int rid,bool isupdate){
			return this->getram(rid,0,isupdate);
		}

		void* ShareRam::getram(int rid,int pos,bool isupdate){
			if(!rid){
				return nullptr;
			}

			map<int,int>::iterator it = this->m_ridhid.find(rid);
			if(it == this->m_ridhid.end()){
				return nullptr;
			}

			int hindex = it->second;
			if(0 > hindex){
				return nullptr;
			}

			if(!pos){
				pos = 0;
			}

			this->m_hash[hindex]->update = 1;

			return (void*)((char*)(this->m_hash[hindex]->data) + pos);
		}

		void* ShareRam::getram(int rid,int pos,void* data,int size,bool isupdate){
			if(!rid || !size || nullptr == data){
				data = nullptr;

				return nullptr;
			}

			map<int,int>::iterator it = this->m_ridhid.find(rid);
			if(it == this->m_ridhid.end()){
				return nullptr;
			}

			int hindex = it->second;
			if(0 > hindex){
				return nullptr;
			}

			if(!pos){
				pos = 0;
			}

			if((pos + size) > this->m_hash[hindex]->total){
				data = nullptr;

				return nullptr;
			}

			memcpy(data,(void*)((char*)(this->m_hash[hindex]->data) + pos),size);

			return data;
		}

		int ShareRam::update(int rid,void* data,int size){
			return this->update(rid,0,data,size);
		}

		int ShareRam::update(int rid,int pos,void* data,int size){
			if(!rid || nullptr == data || !size){
				return 1;
			}

			if(!pos){
				pos = 0;
			}

			map<int,int>::iterator it = this->m_ridhid.find(rid);
			if(it == this->m_ridhid.end()){
				Log::Warning(__func__, "Cache not exsits!");

				void* temp = this->save(rid,pos,data,size);
				if(nullptr != temp) {
					return 0;
				}

				return -1; 
			}

			int index = it->second;
			if(0 > index || index >= this->m_shareramblk){
				Log::Warning(__func__, "Cache index Error! index: " + Util::int2str(index));

				return 1;
			}

			if((pos + size) > this->m_hash[index]->total){
				Log::Perror(__func__, "Save data more than!");

				return 1;
			}

			this->m_hash[index]->dirt = 1;
			memset((void*)((char*)(this->m_hash[index]->data) + pos),0,size);
			memcpy((void*)((char*)(this->m_hash[index]->data) + pos),data,size);

			return 0;
		}

		void* ShareRam::get(int* hashindex,int *size,bool* is_dirt){
			if(nullptr == hashindex || nullptr == size || 
				nullptr == is_dirt) {
				return nullptr;
			}

			if(0 > *hashindex || this->m_shareramblk <= *hashindex) {
				return nullptr;
			}

			*size = this->m_hash[*hashindex]->total;

			if(this->m_hash[*hashindex]->dirt == 1) {
				*is_dirt = true;
				this->m_hash[*hashindex]->dirt = 0;
			}else {
				*is_dirt = false;
			}

			void* temp = this->m_hash[*hashindex]->data;
			*hashindex = *hashindex + 1;

			return temp;
		}
	}
}