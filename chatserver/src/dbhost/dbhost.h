#ifndef H_DBHOST_H_
#define H_DBHOST_H_

#include "./../../../common/ipc/ipc.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/msgdefine/msgcode.h"


using namespace util;
using namespace engine;
using namespace msghandle;

namespace dbhost {
    class DbHost {
    public:
        /**
        * 将离线消息发送到DB存储
        *
        * @param chat
        */
        static void saveOfflineMsgIPC(inner_ipc::ChatMsgIPC chat);

        /**
        * 发送请求到DB获取离线消息
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void getOfflineMsgIPC(NetId netid,const char* buf,int size);
    };
}

#endif 