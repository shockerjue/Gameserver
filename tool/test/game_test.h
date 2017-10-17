#ifndef H_GAMETEST_H_
#define H_GAMETEST_H_

#ifdef _cplusplus
extern "C"{
#endif
#ifdef __linux

#include <time.h>
#include <fcntl.h>
#include <sys/select.h>

#elif __APPLE__
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>

#endif
#ifdef _cplusplus
}
#endif


#include "./../../common/util/util.h"
#include "./../../common/include/type.h"
#include "./../../common/include/thread.h"
#include "./../../common/network/network.h"
#include "./../../common/msgdefine/msgheader.h"
#include "./../../gateway/src/msghandle/msgcode.h"
#include "./../../gateway/src/msghandle/gateway_msgheader.pb.h"
#include "./../../gameserver/src/msghandle/gameserver_msgheader.pb.h"

#include <map>
#include <iostream>

using namespace std;
using namespace util;
using namespace engine;
using namespace gateway;
using namespace msghandle;
using namespace gateway_proto;
using namespace engine::thread;
using namespace engine::network;
using namespace gameserver_proto;

/**
* 事件回调函数类型定义
*/
typedef void(*CALLBACK)(Network network,const char* msg,int size);

class GameTest {
public:
    GameTest(void);
    ~GameTest(void);

    /**
    * 登录之前获取登录验证信息
    *
    * @param ip         服务器地址
    * @param port       服务器端口
    * @param name       用户名
    * @param reponse    验证返回信息
    *
    * @return  bool     返回是否验证成功
    */
    bool authri(const char* ip,const int port,const char * name,
        RoleLoginResponse* response);

    /**
    * 创建角色，在登录时，如果角色没有被创建时则调用该接口创建
    *
    * @param name       角色名
    * @param pwd        密码
    * @param reponse    响应数据
    *
    * @return bool
    */
    bool createRole(Network network,const char* name,const char* pwd,
        RoleLoginResponse* response);

    /**
    * 根据验证信息登录服务器
    *
    * @param reponse
    */
    bool login(RoleLoginResponse* reponse);

    /**
    * 显示菜单
    */
    void menu(void);

    /**
    * 请求玩家信息
    */
    void requestRoleInfo(void);

    /**
    * 请求好友信息
    */
    void requestFriends(void);

    /**
    * 运行游戏，开启事件处理线程
    *
    * @param thread
    */
    void game_run(Thread* thread);

public:
    /**
    * 传送聊天信息到玩家
    *
    * @param network
    * @param rid        玩家rid
    * @param did        接收消息的rid
    * @param msg        发送的消息
    */
    static void sendChat(Network network,int rid,int did,string msg);

    /**
    * 添加好友
    *
    * @param    network
    * @param    arid
    */
    static void sendAddFriend(Network network,int arid);

    /**
    * 请求消息{离线、系统}
    *
    * @param network
    * @param rid
    * @param srid
    */
    static void sendRequestMsg(Network network,int rrid);

    /**
    * 收到聊天通信信息
    *
    * @param msg
    * @param size
    */
    static void onRecvChat(Network network,const char* msg,int size);

    /**
    * 收到添加好友反馈
    * 
    * @param network
    * @param msg
    * @param size
    */
    static void onAddFriend(Network network,const char* msg,int size);

    /**
    * 收到角色信息
    *
    * @param msg
    * @param size
    */
    static void onRecvRoleInfo(Network network,const char* msg,int size);

    /**
    * 收到好友列表信息
    *
    * @param msg
    * @param size
    */
    static void onRecvFriends(Network network,const char* msg,int size);

    /**
    * 获取消息的响应
    *
    * @param network
    * @param msg
    * @param size
    */
    static void onRequestMsg(Network network,const char* msg,int size);

private:
    Network     m_network;
    string      m_rolename;
    int         m_rid;
};

/**
* 线程声明
*/
Thread_Func(game);

/**
* 游戏测试
*/
void game_test(const char* ip,const int port,const char * name);

#endif