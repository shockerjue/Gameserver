/************************************************************
* 主要是定义主线程发送给IO线程的消息结构
*
* 由于是进程和线程之间发送，所以不需要使用proto来定义
************************************************************/
#ifndef H_THREADIPC_H_
#define H_THREADIPC_H_

namespace thread_ipc {

    /**
    * 玩家请求登陆时，主线程通知IO线程的结构
    */
    struct ReqLogin {
        int     requestId;
        int     netId;
        int     loginId;
        char    username[64];
    };

    /**
    * 请求获取角色消息体
    */
    struct ReqRoleInfo {
        int     rid;
        int     netId;
        int     loginId;
        int     requestId;
        int     reqrid;
    };

    /**
    * 创建角色的线程消息体
    */
    struct CreateRole {
        int     netId;
        int     requestId;
        char    username[64];
    };

    /**
    * 获取好友列表
    */
    struct FriendsList {
        int     rid;
        int     netId;
        int     requestId;
        int     type;
    };

    /**
    * 添加好友
    */
    struct AddFriend {
        int     rid;
        int     arid;
        int     netId;
        int     requestId;
    };

    struct Msg {
        int 	rid;	//	玩家rid
        int		type;	//	请求的类型
        int 	rrid;	//	请求的玩家rid
        int     count;	//	请求的数量
        int     requestId;	//	请求的网络id
        int     netId;
    };
}

#endif