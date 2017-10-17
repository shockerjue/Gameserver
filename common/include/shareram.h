/***************************************************************
* 主要用于缓存数据，如玩家的数据，
* 可以使用这个来存储，同时定时同步到数据库中
* 共享内存的使用，可以用于在dataaccess中同步数据，即用单独的线程或者
* 进程来同步数据，如dataaccess进程仅仅是读写共享的内存区域，而另一个
* 线程将数据同步写到数据库或者文件中。
*
* author: shockerjue
****************************************************************/

#ifndef H_SHARERAM_H_
#define H_SHARERAM_H_

#include <map>
#include <iostream>

#ifdef _cplusplus
    extern "C"{
#endif
	#include <string.h>
	#include <sys/shm.h>
	#include <sys/ipc.h>
	#include <sys/types.h>
#ifdef _cplusplus
    }
#endif
    
#include "./../util/log.h"
#include "./../util/util.h"

using namespace std;
using namespace util;

namespace engine {

	namespace shareram{
		
		//	哈希表的大小
		const unsigned short HASH_SIZE = 3000;

		/**
		* 共享内存中存储的项信息
		*/
		struct ShareItem {
			int 	rid;			//	存储玩家对应的id,用于标识数据的有效性{key}
			int 	lock;			//	标识数据是否被使用（未使用）
			int 	update;			//	标识数据是否可以更新
			int		size;			//	数据的尺寸
			int 	total;			//	数据的大小
			int 	utime;			//	内存更新时间
			int 	dirt;			//	是否属脏数据{是否能够同步数据}
			void* 	data;			//	数据存储指针

			ShareItem(void);
			ShareItem(const ShareItem& share);
			ShareItem& operator=(const ShareItem& share);
			string toString(void);
		};

		class ShareRam {
		public:
			~ShareRam(void);

			/**
			* 创建共享内存
			*
			* @param nsize 	每一个数据块的大小
			* @param len 	数据的块数量	
			*/
			bool create(int nsize,int len);

			/**
			* 初始化共享内存，采用两头分配的方式
			* [共享内存的起始是管理项，共享内存的尾部开始的是数据]
			*
			* @param nsize 		共享内存中每一项的大小
			*/
			bool onReInit(int nsize);

			/**
			* 从已有的共享内存中获取数据
			*
			* @param nsize 	共享内存中每一项的大小
			*/
			bool onLoad(int nsize);

			/**
			* 保存数据到共享内存中
			*
			* @param rid 	数据存储的key
			* @param data 	要存储的数据
			* @param size 	存储数据的大小
			*/
			void* save(int rid,void* data,int size);

			/**
			* 保存数据到pos开始的位置
			*
			* @param rid
			* @param pos
			* @param data
			* @param size
			*/
			void* save(int rid,int pos,void* data,int size);

			/**
			* 移除对应rid的共享数据
			* 
			* @param rid
			*/
			bool remove(int rid);

			/**
			* 获取rid对应的内存
			*
			* @param rid
			* @param isupdate 		标记是否需要更新
			*/
			void* getram(int rid,bool isupdate);

			/**
			* 获取rid对应的pos位置之后的数据
			*
			* @param rid
			* @param pos		读取数据的位置
			* @param isupdate
			*/
			void* getram(int rid,int pos,bool isupdate);

			/**
			* 读取pos位置处的后面size字节的数据
			*
			* @param rid		数据key
			* @param pos		读取数据的位置
			* @param size		读取数据的大小
			* @param data		数据存储的空间
			* @param isupdate
			*/
			void* getram(int rid,int pos,void* data,int size,bool isupdate);
			
			/**
			* 更新共享内存的数据{整体更新}
			*
			* @param rid
			* @param data
			* @param size
			*/
			int update(int rid,void* data,int size);

			/**
			* 更新数据到共享内存中具体位置{部分更新}
			*
			* @param rid	数据存储的key
			* @param pos	存储数据的位置（相对于key的共享内存区域）
			* @param data	需要存储的数据
			* @param size	数据的大小
			*/
			int update(int rid,int pos,void* data,int size);

			/**
			* 获取索引对应的共享内存
			*/
			void* get(int* hashindex,int* size,bool* is_dirt);

			/**
			* 通过Key获取对应的ShareRam
			*
			* @param  mkey
			*/
			static ShareRam* getShareRam(int mkey);

		private:
			ShareRam(int mkey);
			ShareRam(const ShareRam&){}
			ShareRam& operator=(const ShareRam&){ return *this; }

		private:
			int 				m_shareramfd;
			int					m_shareramblk;		//	共享内存存储的数据的块数
			int 				m_shareramkey;
			int 				m_shareramsize;		//	共享内存的大小
			void*				m_shareram;
			map<int,int>		m_ridhid;
			struct ShareItem*	m_hash[HASH_SIZE];

			static ShareRam* 	s_pShareRam;
		};
	}

}

#endif