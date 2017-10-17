#include "chathost.h"

namespace chathost {
    void ChatHost::onSaveOfflineMsg(NetId netid,const char* buf,int size) {
        if(!netid || !size ||
            nullptr == buf) {
            return ;
        }

        // 将信息发送到写数据库线程
        Message message;
        message.setsize(size);
        message.setwhat(Inner_What::INNER_WRITE_OFFLINE_MSG);
        message.setobject(buf,size);

        Thread::ssend_message_to_thread("io_write1",message);

        Log::Print(__func__, "ChatHost::onSaveOfflineMsg");
    }

    THREAD_CALLBACK_FUNC void ChatHost::onSaveOfflineMsg(const void* param,Message message) {
        if(nullptr == param) {
            return ;
        }

        int size = 0;
        char buf[4096] = {0};
        void* temp = message.getobject(buf,&size);
        if(nullptr == temp || 0 == size) {
            return ;
        }

        string data;
        data.assign(buf,size);

        inner_ipc::OfflineMsgIPC req;
        req.ParseFromString(data);
        if(0 == req.srid() || 0 == req.did() ||
            0 == req.sname().size()) {
            return ;
        }

        MysqlMaster* mysqlMaster = (MysqlMaster*)param;
        mysqlMaster->add_offline_msg(req);
    }

    void ChatHost::onSaveEmailMsg(NetId netid,const char* buf,int size) {
        if(!netid || !size ||
            nullptr == buf) {
            return ;
        }

        string data;
        data.assign(buf,size);

        // 将信息发送到写数据库线程

        Log::Print(__func__,"ChatHost::onSaveEmailMsg");
    }

    THREAD_CALLBACK_FUNC void ChatHost::onSaveEmailMsg(const void* param,Message message) {
        if(nullptr == param) {
            return ;
        }

        MysqlMaster* mysqlMaster = (MysqlMaster*)param;
    }

    void ChatHost::onSaveOffineGoods(NetId netid,const char* buf,int size) {
        if(!netid || !size ||
            nullptr == buf) {
            return ;
        }

        string data;
        data.assign(buf,size);

        // 将信息发送到写数据库线程

        Log::Print(__func__,"ChatHost::onSaveOffineGoods");
    }

    THREAD_CALLBACK_FUNC void ChatHost::onSaveOffineGoods(const void* param,Message message) {
        if(nullptr == param) {
            return ;
        }

        MysqlMaster* mysqlMaster = (MysqlMaster*)param;
    }
}