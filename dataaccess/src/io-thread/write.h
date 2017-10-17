
/*****************************************************
*   写数据到数据库中，如果需要写到缓存中，则也直接写到缓存中
******************************************************/
#ifndef H_WRITE_H_
#define H_WRITE_H_

#include "common.h"

namespace io_thread {
    class Write {
    public:
        Write(void);
        ~Write(void);

        Write(const Write&);
        Write& operator=(const Write&);

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