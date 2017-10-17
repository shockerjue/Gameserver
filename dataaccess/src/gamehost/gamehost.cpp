#include "gamehost.h"

namespace gamehost {
    void GameHost::onSaveBattleInfo(NetId netid,const char* buf,int size) {
        if(!netid || !size ||
         nullptr == buf) {
             return ;
         }

         string to_data;
         to_data.assign(buf,size);

         Log::Print(__func__, "GameHost::onSaveBattleInfo");
    }

    THREAD_CALLBACK_FUNC void GameHost::onSaveBattleInfo(const void* param, Message message) {
        if(nullptr == param) {
            return ;
        }

        MysqlMaster* mysqlMaster = (MysqlMaster*)param;
    }

    void GameHost::onSaveDropInfo(NetId netid,const char* buf,int size) {
        if(!netid || !size ||
            nullptr == buf) {
            return ;
        }

        string to_data;
        to_data.assign(buf,size);

        Log::Print(__func__, "GameHost::onSaveDropInfo");
    }

    THREAD_CALLBACK_FUNC void GameHost::onSaveDropInfo(const void* param,Message message) {
        if(nullptr == param) {
            return ;
        }

        MysqlMaster* mysqlMaster = (MysqlMaster*)param;
    }
}