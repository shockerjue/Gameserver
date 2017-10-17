#include "dbhost.h"

namespace dbhost {

    void DbHost::saveOfflineMsgIPC(inner_ipc::ChatMsgIPC chat) {
        if(0 == chat.sname().size()) {
            return ;
        }

        if(0 == chat.srid() || 0 == chat.did()) {
            return ;
        }

        inner_ipc::OfflineMsgIPC req;
        req.set_srid(chat.srid());
        req.set_did(chat.did());
        req.set_type(chat.type());
        req.set_msg_type(chat.msg_type());
        req.set_tid(chat.tid());
        req.set_size(chat.size());
        req.set_stime(chat.stime());
        req.set_msg(chat.msg());
        req.set_sname(chat.sname());

        string data;
        req.SerializeToString(&data);

        char _data[data.length() + 1];
        memset(_data,0,data.length() + 1);
        memcpy(_data,data.c_str(),data.length());

        NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::DB2,
            _MsgType_Chatserver_Dataaccess::SS_SAVE_OFFLINE_MSG,_data,
            data.length());
        
        Log::Print(__func__, req.msg());

        return ;
    }

    void DbHost::getOfflineMsgIPC(NetId netid,const char* buf,int size) {

    }
}