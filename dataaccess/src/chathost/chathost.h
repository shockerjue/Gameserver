/**
* 接收来聊天服的数据记录请求
*/
#ifndef H_CHATHOST_H_
#define H_CHATHOST_H_


#include "./../dbrw/mysqlslave.h"
#include "./../dbrw/mysqlmaster.h"
#include "./../msghandle/inner_code.h"
#include "./../msghandle/thread_ipc.h"
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
using namespace thread_ipc;

namespace chathost {
    class ChatHost {
    public:
        //////////下面的方法在主线程受客户的请求时将请求发送到Io线程中//////////////////////
        /**
        * 保存来自聊天服务器的玩家发送的离线消息
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void onSaveOfflineMsg(NetId netid,const char* buf,int size);

        /**
        * 保存来自聊天服务器的玩家发送的邮件消息
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void onSaveEmailMsg(NetId netid,const char* buf,int size);

        /**
        * 保存来自聊天服务器的物品直接的发送{赠送、购买..}
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void onSaveOffineGoods(NetId netid,const char* buf,int size);

        //////////下面的方法是在读写线程中调用///////////////////////////////
        /**
        * 线程收到离线消息保存时调用，主要是将消息保存到数据库中
        *
        * @param param      MysqlMaster
        * @param message    消息体
        */
        static THREAD_CALLBACK_FUNC void onSaveOfflineMsg(const void* param,Message message);

        /**
        * 线程收到发送邮件时，将邮件保存到数据库中
        *
        * @param param      MysqlMaster
        * @param message    消息体
        */ 
        static THREAD_CALLBACK_FUNC void onSaveEmailMsg(const void* param,Message message);

        /**
        * 保存来自聊天服务器的物品直接的发送{赠送、购买..}到数据库中
        *
        * @param param      MysqlMaster
        * @param message    消息体
        */
        static THREAD_CALLBACK_FUNC void onSaveOffineGoods(const void* param,Message message);
    };
}

#endif