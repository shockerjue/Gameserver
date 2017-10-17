#ifndef H_COMMON_H
#define H_COMMON_H

#include "./../dbhost/dbhost.h"
#include "./../dbrw/mysqlslave.h"
#include "./../chathost/chathost.h"
#include "./../msghandle/inner_code.h"
#include "./../../../common/include/thread.h"

using namespace dbrw;
using namespace dbhost;
using namespace engine;
using namespace chathost;
using namespace inner_code;
using namespace engine::thread;

namespace io_thread {

    /**
    * 定义线程消息回调方法类型
    *
    * @param param      数据库操作句柄
    * @param message    线程消息体
    */
    typedef  void (*Message_Func)(const void* param,Message message);

    class Common {
    public:

        /**
        * 注册线程消息回调方法
        *
        * 主要注册读线程和写线程中的回调方法
        */
        static void RegisterCall(void);

        /**
        * 执行消息回调方法
        *
        * @param what       消息类型
        * @param param      回调方法的第一个参数
        * @param message    接受到的消息
        */
        static void Excute(int what,const void* param,Message message);

    private:
        static map<int,Message_Func> s_what_call;
    };
}

#endif