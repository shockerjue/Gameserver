#include "sharecache.h"

namespace sharecache {

    ShareCache* ShareCache::s_pShareCache = nullptr;

    ShareCache::ShareCache(void){
        this->is_use = false;
        this->m_pShare = nullptr;
    }

    ShareCache::~ShareCache(void) {
    }

    ShareCache* ShareCache::getShareCache(void){
        if(nullptr == ShareCache::s_pShareCache){
            ShareCache::s_pShareCache = new ShareCache();
        }

        return ShareCache::s_pShareCache;
    }

    void ShareCache::initShareCache(int n,int id) {
        if(nullptr != this->m_pShare) {
            Log::Warning(__func__, "ShareRam already create ---- ");

            return ;
        }

        if(!n) {
            return ;
        }

        this->m_pShare = ShareRam::getShareRam(id);
        bool ret =  this->m_pShare->create(sizeof(struct Cache),n);
        if(!ret){
            Log::Print(__func__, "shareram->create fail!");
        }

        ret =  this->m_pShare->onReInit(sizeof(struct Cache));
        if(!ret){
            Log::Print(__func__, "shareram->onInit fail!");
        }
    }

    int ShareCache::read_role_info(int key,struct Role* role){
        if(!key || nullptr == this->m_pShare ||
        nullptr == role) {

            return -1;
        }

        if(!this->is_use) {
            Log::Warning(__func__, "Didn't set use cache!");
            
            return -1;
        }

        void* temp = this->m_pShare->getram(key,0,role,sizeof(struct Role),false);
        if(nullptr == temp) {
            return -1 ;
        }

        return 0;
    }

    int ShareCache::write_role_info(int key,struct Role* role) {
        if(!key || nullptr == role || 
            nullptr == this->m_pShare){
            return -1 ;
        }

        if(!this->is_use) {
            Log::Warning(__func__, "Didn't set use cache!");
            
            return -1;
        }

        int ret = this->m_pShare->update(key,0,role,sizeof(struct Role));
        if(0 == ret) {
            return 0;
        }

        return -1 ;
    }

    void* ShareCache::get_sync_cache(int* hashindex,int* size,bool* is_dirt){
        if(nullptr == this->m_pShare) {
            return nullptr;
        }

        if(!this->is_use) {
            Log::Warning(__func__, "Didn't set use cache!");
            
            return nullptr;
        }

        return this->m_pShare->get(hashindex,size,is_dirt);
    }
}