#include "gamehost.h"

namespace gamehost {
    void GameHost::onEnterGame(NetId netid,const char* buf,int size) {
        if(!netid || !size ||
            nullptr == buf) {
            return ;
        }

        string data;
        data.assign(buf,size);

        inner_ipc::EnterGameIPC game;
        game.ParseFromString(buf);

        return ;
    }

    void GameHost::onExitGame(NetId netid,const char* buf,int size) {
        if(!netid || !size ||
            nullptr == buf) {
            return ;
        }

        string data;
        data.assign(buf,size);

        inner_ipc::ExitGameIPC game;
        game.ParseFromString(data);

        return ;
    }

    void GameHost::onDisconnected(NetId netid,const char* buf,int size) {
        if(!netid || !size||
            nullptr == buf){
            return ;
        }

        string data;
        data.assign(buf,size);

        inner_ipc::DisconnectIPC game;
        game.ParseFromString(buf);

        return ;
    }

    void GameHost::onReconnected(NetId netid,const char* buf,int size) {
        if(!netid || !size ||
         nullptr ==buf){
            return ;
        }

        string data;
        data.assign(buf,size);

        inner_ipc::ReConnectIPC game;
        game.ParseFromString(buf);

        return ;
    }

    void GameHost::onRequestRoles(NetId netid,const char* buf,int size) {
        if(!netid || !size ||
            nullptr == buf){
            return ;
        }

        string data;
        data.assign(buf,size);
        
        inner_ipc::RequestRolesIPC game;
        game.ParseFromString(data);

        return ;
    }
}