#ifndef H_SHARECACHE_H_
#define H_SHARECACHE_H_

#include "./../../../common/util/util.h"
#include "./../../../common/include/shareram.h"

using namespace util;
using namespace engine::shareram;

namespace sharecache {
    struct RoleAttr {
        RoleAttr():attrack(0),defense(0),aliglity(0),life(0){}

        int     attrack;
        int     defense;
        int     aliglity;
        int     life;
    };

    struct Role {
        Role():rid(0),level(0),vip(0){
            memset(name,0,32);
        }

        int                 rid;
        int                 level;
        int                 vip;
        char                name[32];
        struct  RoleAttr    attr;
    };

    /**
    * 缓存中存储的数据结构，主要用于分配共享内存使用
    * 如果需要在共享内存中存储数据，需要在这里定义
    */
    struct Cache {
        struct Role     role;
    };

    class ShareCache {
    public:
        ~ShareCache(void);

        /**
        * 打开使用缓存,如果要使用缓存
        * 必须在使用前调用这个方法
        */
        inline void set_use(void) {
            this->is_use = true;
        }

        /**
        * 初始化共享内存
        *
        * @param n      共享内存中的块数量
        * @param id     共享内存产生key的id
        */
        void initShareCache(int n,int id);

        /**
        * 读取玩家信息
        * 
        * @param key
        * @param role
        */
        int read_role_info(int key,struct Role* role);

        /**
        * 写入玩家信息到缓存中
        *
        * @param key
        * @param role
        */
        int write_role_info(int key,struct Role* role);

        /**
        * 获取需要同步的数据，获取的是一个缓冲块
        *
        * @param hashindex  缓存的哈希索引
        * @param size       获取对应hashindex缓存的尺寸
        * @param is_dirt    是否需要同步到数据库
        */
        void* get_sync_cache(int* hashindex,int* size,bool* is_dirt);

        static ShareCache* getShareCache(void);

    private:
        ShareCache(void);
        ShareCache(const ShareCache&){}
        ShareCache& operator=(const ShareCache&){ return *this; }

    private:
        bool    is_use;
        ShareRam* m_pShare;

        static ShareCache* s_pShareCache;
    };

}

#endif