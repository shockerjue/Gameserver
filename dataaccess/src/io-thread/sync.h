#ifndef H_SYNC_H_
#define H_SYNC_H_

#include "./../dbrw/mysqlmaster.h"
#include "./../../../common/ipc/ipc.h"
#include "./../sharecache/sharecache.h"
#include "./../../../common/util/util.h"

using namespace dbrw;
using namespace util;
using namespace sharecache;

namespace io_thread {
    class Sync {
    public:
        Sync(void);
        ~Sync(void);

        Sync(const Sync&);
        Sync& operator=(const Sync&);

        /**
        * 同步共享内存中的数据，共享内存中有数据更新，则将数据库同步到对应的数据库中
        * 
        * @param shareCache     共享内存管理指针
        * @param mysqlMaster    写数据库操作的指针
        */
        void sync(void* shareCache,void* mysqlMaster);
    };
}

#endif