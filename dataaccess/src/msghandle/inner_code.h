/*******************************************************************
* 定义进程与线程之间的消息what
*******************************************************************/
#ifndef H_INNERCODE_H_
#define H_INNERCODE_H_

namespace inner_code {
    enum Inner_What {
        /**
        * 定义读线程的消息what
        */
        INNER_READ_ROLE_FOR_LOGIN           =   1,      //  读取角色信息
        INNER_READ_ROLE_FOR_REQ             =   2,      //  请求角色信息    
        INNER_READ_ROLE_FRIENDS             =   3,      //  请求好友列表

        INNER_READ_ADD_FRIEND               =   4,      //  添加好友时进行读取

        /**
        * 定义写线程的消息what
        */
        INNER_WRITE_ROLE_FOR_CREATE         =   100,    //  创建角色信息
        INEER_WRITE_ADD_FRIEND              =   101,     //  添加好友时写入数据
        
        INNER_WRITE_OFFLINE_MSG             =   200,     //  保存离线消息
        INNER_READ_OFFLINE_MSG              =   201,    //  读取离线消息
    };
}

#endif