/**
* 接收来自游戏服的数据记录请求
*/

#ifndef H_GAMEHOST_H_
#define H_GAMEHOST_H_

#include "./../dbrw/mysqlslave.h"
#include "./../dbrw/mysqlmaster.h"
#include "./../msghandle/inner_code.h"
#include "./../msghandle/thread_ipc.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/include/value.h"

using namespace util;
using namespace dbrw;
using namespace engine;
using namespace msghandle;
using namespace inner_code;

namespace gamehost {
    class GameHost {
    public:
        //////////下面的方法在主线程受客户的请求时将请求发送到Io线程中//////////////////////
        /**
        * 保存来自gameworld服务的战斗信息
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void onSaveBattleInfo(NetId netid,const char* buf,int size);

        /**
        * 保存gameworld战斗中掉落的物品信息
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void onSaveDropInfo(NetId netid,const char* buf,int size);

        //////////下面的方法是在读写线程中调用,需要注册///////////////////////////////
        /**
        * 线程收到保存战斗信息的消息，将信息保存到数据库中
        *
        * @param param      MysqlMaster
        * @parma message    消息体
        */
        static THREAD_CALLBACK_FUNC void onSaveBattleInfo(const void* param, Message message);

        /**
        * 线程收到保存掉落信息的消息时，将信息保存到数据库中
        *
        * @param param      MysqlMaster
        * @param message    消息体
        */
        static THREAD_CALLBACK_FUNC void onSaveDropInfo(const void* param,Message message);
    };
}

#endif