/*******************************************************
* 读取数据消息处理
******************************************************/
#ifndef H_READ_H_
#define H_READ_H_

#include "common.h"

namespace io_thread {
    class Read {
    public:
        Read(void);
        ~Read(void);

        Read(const Read&);
        Read& operator=(const Read&);

        /**
        * 有新消息到达，根据消息类型进行处理
        * 
        * @param msg  消息对象
        */
        virtual void new_msg(const void* param,Message msg);

        /**
        * 有新消息到达
        *
        * @param msgs 	消息列表对象
        */
        virtual void new_msg(const void* param,vector<Message> msgs);
    };
}

#endif