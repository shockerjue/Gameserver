#ifndef H_GAMEHOST_H_
#define H_GAMEHOST_H_

#include "./../../../common/ipc/ipc.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/engine.h"

using namespace engine;

namespace gamehost {
    class GameHost {
    public:
        /**
        * 玩家进入游戏时调用
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void onEnterGame(NetId netid,const char* buf,int size);

        /**
        * 玩家退出游戏时调用
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void onExitGame(NetId netid,const char* buf,int size);

        /**
        * 玩家断开连接时调用(被动断开)
        *
        * @param netid 
        * @param buf
        * @param size
        */
        static void onDisconnected(NetId netid,const char* buf,int size);

        /**
        * 玩家重新连接回调
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void onReconnected(NetId netid,const char* buf,int size);

        /**
        * 请求游戏环境中的玩家信息，主要是表示地信息，{rid,name,position,head}
        *
        * @param netid
        * @param buf
        * @param size
        */
        static void onRequestRoles(NetId netid,const char* buf,int size);
    };
}

#endif