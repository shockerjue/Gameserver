#include "sync.h"

namespace io_thread {
    Sync::Sync(void) {

    }

    Sync::~Sync(void) {

    }

    Sync::Sync(const Sync& sync) {

    }

    Sync& Sync::operator=(const Sync& sync) {
        if(this == &sync) {
            return *this;
        }

        return *this;
    }

    void Sync::sync(void* shareCache,void* mysqlMaster) {
        if(nullptr == shareCache || 
            nullptr == mysqlMaster) {
            return ;
        }
        
        int hashindex = 0;
        int cachesize = 0;
        bool is_dirt = false;

        void* temp = nullptr;
        ShareCache* share = (ShareCache*)shareCache;
        MysqlMaster* master = (MysqlMaster*)mysqlMaster;

        int sync_count = 0;
        while(nullptr != (temp = share->get_sync_cache(&hashindex,&cachesize,&is_dirt))){
            if(false == is_dirt) {
                continue ;
            }

            struct Role role;
            memcpy(&role,temp,sizeof(struct Role));

            inner_ipc::RoleInfo roleInfo;
            roleInfo.set_rid(role.rid);
            roleInfo.set_name(string(role.name));
            master->sync_role_info(&roleInfo);

            temp = nullptr;
            sync_count++;
        }

        Log::Print(__func__, "Sync data excute  sync count: " + Util::int2str(sync_count));
    }
}