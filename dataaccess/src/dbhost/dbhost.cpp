#include "dbhost.h"

namespace dbhost {
    void DbHost::onLoginRequest(NetId netid,const char* buf,int size){
        if(nullptr == buf || !netid){
            return ;
        }

        if(!size){
            return ;
        }

        string to_data;
        to_data.assign(buf,size);
        inner_ipc::RequestLoginIPC login;
        login.ParseFromString(to_data);

        ReqLogin t_login;
        t_login.netId = netid;
        t_login.requestId = login.requestid();
        strcpy(t_login.username,login.username().c_str());
        
        Message msg;
        msg.setwhat(Inner_What::INNER_READ_ROLE_FOR_LOGIN);
        msg.setobject(&t_login,sizeof(ReqLogin));

        Thread::ssend_message_to_thread("io_read1",msg);

        return ;
    }

    THREAD_CALLBACK_FUNC void DbHost::onLoginRequest(const void* param,Message msg) {
        if(nullptr == param) {
            return ;
        }

        ReqLogin t_login;

        void* temp = msg.getobject(&t_login,sizeof(t_login));
        if(nullptr == temp) {
            return ;
        }

        inner_ipc::ResponseLoginIPC db;
        db.set_status(0);
        db.set_requestid(t_login.requestId);
        db.set_msg("Login success!");

        inner_ipc::RoleInfo role;
        MysqlSlave* mysqlSlave = (MysqlSlave*)param;
        bool ret = mysqlSlave->read_role_info(string(t_login.username),&role,true);
        if(!ret) {
            db.set_rid(0);
            db.set_status(1);
            db.set_msg("Username not exists!");
            db.set_username(string(t_login.username));
        }else {
            db.set_rid(role.rid());
            db.set_username(role.name());
        }

        string data;
        db.SerializeToString(&data);

        char _data[data.length() + 1];
        memset(_data,0,data.length() + 1);
        memcpy(_data,data.c_str(),data.length());

        NetworkManage::getNetworkManage()->SendByNetId(t_login.netId,
                _MsgType_Login_Db::SS_LOGIN_VERI_TO_DB,
                _data,data.length());

        Log::Print(__func__, db.username() + "  " + db.msg());
    }

    void DbHost::onCreateRole(NetId netid,const char* buf,int size){
        if(!netid || nullptr == buf){
            return ;
        }

        string to_data;
        to_data.assign(buf,size);
        inner_ipc::RequestLoginIPC login;
        login.ParseFromString(to_data);

        if(!login.requestid()){
            return ;
        }

        struct CreateRole c_role;
        c_role.netId = netid;
        c_role.requestId = login.requestid();
        strcpy(c_role.username,login.username().c_str());

        Message message;
        message.setwhat(Inner_What::INNER_WRITE_ROLE_FOR_CREATE);
        message.setobject(&c_role,sizeof(c_role));

        Thread::ssend_message_to_thread("io_write1",message);
    }

    THREAD_CALLBACK_FUNC  void DbHost::onCreateRole(const void* param ,Message message) {
        if(nullptr == param) {
            return ;
        }

        struct CreateRole c_role;
        void* temp = message.getobject(&c_role,sizeof(c_role));
        if(nullptr == temp) {
            return ;
        }

        inner_ipc::RoleInfo role;
        // set role's value
        role.set_name(string(c_role.username));

        MysqlMaster* mysqlMaster = (MysqlMaster*)param;
        int ret = mysqlMaster->create_role_info(&role,true);
        if(0 > ret) {
            return ;
        }

        inner_ipc::ResponseLoginIPC dblogin;
        dblogin.set_status(0);
        dblogin.set_requestid(c_role.requestId);
        
        if(0 == ret) {
            dblogin.set_status(1);
        }else {
            dblogin.set_username(role.name());
            dblogin.set_rid(role.rid());
        }

        string data;
        dblogin.SerializeToString(&data);
        char _data[data.length() + 1];
        memset(_data,0,data.length() + 1);
        memcpy(_data,data.c_str(),data.length());

        NetworkManage::getNetworkManage()->SendByNetId(c_role.netId,
            _MsgType_Login_Db::SS_LOGIN_TO_DB_CREATEROLE,
            _data,data.length());

        Log::Print(__func__,dblogin.username());
    }

    void DbHost::onRequestLoginInfo(NetId netid,const char* buf,int size){
        if(!netid || nullptr == buf){
            return ;
        }

        string to_data;
        to_data.assign(buf,size);

        inner_ipc::ReqRoleInfoIPC req;
        req.ParseFromString(to_data);

        struct Role role;
        int ret = ShareCache::getShareCache()->read_role_info(req.reqrid(),&role);
        if(0 == ret) {
            inner_ipc::ResRoleInfoIPC roleInfo;
            roleInfo.set_rid(role.rid);
            roleInfo.set_reqrid(req.reqrid());
            roleInfo.set_name(string(role.name));
            roleInfo.set_requestid(req.requestid());
            
            string data;
            roleInfo.SerializeToString(&data);
            char _data[data.length() + 1];
            memset(_data,0,data.length() + 1);
            memcpy(_data,data.c_str(),data.length());

            NetworkManage::getNetworkManage()->SendByNetId(netid,
                    _MsgType_Gameserver_Db::SS_REQUEST_LOGIN_INFO,
                    _data,data.length());

            Log::Print(__func__,roleInfo.name() + " from ShareCache data!");

            return ;
        }

        struct ReqRoleInfo r_role;
        r_role.rid = req.rid();
        r_role.netId = netid;
        r_role.requestId = req.requestid();
        r_role.reqrid = req.reqrid();

        Message msg;
        msg.setwhat(Inner_What::INNER_READ_ROLE_FOR_REQ);
        msg.setobject(&r_role,sizeof(r_role));

        Thread::ssend_message_to_thread("io_read2",msg);
    }

    THREAD_CALLBACK_FUNC void DbHost::onRequestLoginInfo(const void* param,Message message) {
        if(nullptr == param) {
            return ;
        }

        struct ReqRoleInfo reqrole;
        void* temp = message.getobject(&reqrole,sizeof(reqrole));
        if(nullptr == temp) {
            return ;
        }

        inner_ipc::ResRoleInfoIPC roleInfo;
        inner_ipc::RoleInfo role;

        MysqlSlave* mysqlSlave = (MysqlSlave*)param;
        bool ret = mysqlSlave->read_role_info(reqrole.rid,&role,true);
        if(!ret) {
            return ;
        }

        roleInfo.set_rid(role.rid());
        roleInfo.set_name(role.name());
        roleInfo.set_requestid(reqrole.requestId);
        roleInfo.set_reqrid(reqrole.reqrid);

        string data;
        roleInfo.SerializeToString(&data);
        char _data[data.length() + 1];
        memset(_data,0,data.length() + 1);
        memcpy(_data,data.c_str(),data.length());

        NetworkManage::getNetworkManage()->SendByNetId(reqrole.netId,
                _MsgType_Gameserver_Db::SS_REQUEST_LOGIN_INFO,
                _data,data.length());

        Log::Print(__func__,roleInfo.name());
    }

    void DbHost::onSaveRequestLoginInfo(NetId netid,const char* buf,int size){
        if(!netid || nullptr == buf){
            return ;
        }

        string to_data;
        to_data.assign(buf,size);
        
    }

    void DbHost::onRequestFriends(NetId netid,const char* buf,int size) {
        if(!netid || nullptr == buf) {
            return ;
        }

        string data;
        data.assign(buf,size);

        inner_ipc::ReqFriendsIPC request;
        request.ParseFromString(data);

        struct FriendsList friends;
        friends.rid = request.rid();
        friends.requestId = request.requestid();
        friends.netId = netid;
        friends.type = request.type();

        Message message;
        message.setwhat(Inner_What::INNER_READ_ROLE_FRIENDS);
        message.setobject(&friends,sizeof(friends));
     
        Thread::ssend_message_to_thread("io_read2",message);
    }

    THREAD_CALLBACK_FUNC void DbHost::onRequestFriends(const void* param,Message message) {
        if(nullptr == param) {
            return ;
        }

        struct FriendsList req;
        void* temp = message.getobject(&req,sizeof(req));
        if(nullptr == temp) {
            return ;
        }

        inner_ipc::ResFriendsIPC response;
        MysqlSlave* mysqlSlave = (MysqlSlave*)param;

        response.set_rid(req.rid);
        response.set_requestid(req.requestId);

        bool ret = mysqlSlave->read_role_friends(req.rid,&response,false);
        if(!ret) {
            Log::Print(__func__, "Friends is empty!");
        }

        string data;
        response.SerializeToString(&data);
        char _data[data.length() + 1];
        memset(_data,0,data.length() + 1);
        memcpy(_data,data.c_str(),data.length());

        NetworkManage::getNetworkManage()->SendByNetId(req.netId,
            _MsgType_Gameserver_Db::SS_REQUEST_FRIENDS_LIST,
            _data,data.length());
        
        Log::Print(__func__, "get read list for friends!");
    }

    void DbHost::onRequestAddFriend(NetId netid,const char* buf,int size) {
        if(!netid || nullptr == buf) {
            return ;
        }

        string data;
        data.assign(buf,size);

        inner_ipc::ReqAddFriendIPC req;
        req.ParseFromString(data);

        struct AddFriend add;
        add.rid = req.rid();
        add.arid = req.arid();
        add.netId = netid;
        add.requestId = req.requestid();

        Message message;
        message.setwhat(Inner_What::INNER_READ_ADD_FRIEND);
        message.setobject(&add,sizeof(add));
     
        Thread::ssend_message_to_thread("io_read1",message);

        return ;
    }

    THREAD_CALLBACK_FUNC void DbHost::onRequestAddFriend(const void* param,Message message) {
        if(nullptr == param) {
            return ;
        }
        
        struct AddFriend add;
        void* temp = message.getobject(&add, sizeof(add));
        if(nullptr == temp) {
            return ;
        }

        int rid = add.rid;
        int arid = add.arid;

        inner_ipc::ResAddFriendIPC response;
        response.set_rid(rid);
        response.set_status(0);
        response.set_requestid(add.requestId);
        
        inner_ipc::RoleInfo role;
        MysqlSlave* mysqlSlave = (MysqlSlave*)param;
        bool ret = mysqlSlave->read_role_info(arid,&role,false);
        if(!ret) {
            response.set_status(1);
        }else {
            ret = mysqlSlave->isFriend(rid,arid);
            if(ret) {
                response.set_status(2);
            }else {
                inner_ipc::FriendItem* item = response.mutable_friend_();
                item->set_rid(role.rid());
                item->set_username(role.name());
            }
        }

        string data;
        response.SerializeToString(&data);
        char _data[data.length() + 1];
        memset(_data,0,data.length() + 1);
        memcpy(_data,data.c_str(),data.length());

        NetworkManage::getNetworkManage()->SendByNetId(add.netId,
            _MsgType_Gameserver_Db::SS_REQUEST_ADD_FRIENDS,
            _data,data.length());

        if(0 == response.status()){
             // 发送消息到写线程，将好友信息写入数据库中
            Message message;
            message.setwhat(Inner_What::INEER_WRITE_ADD_FRIEND);
            message.setobject(&add,sizeof(add));
            
            Thread::ssend_message_to_thread("io_write1",message);
        }

        return ;
    }

    THREAD_CALLBACK_FUNC void DbHost::onAddFriendWrite(const void* param,Message message) {
        if(nullptr == param) {
            return ;
        }

        struct AddFriend add;
        void* temp = message.getobject(&add,sizeof(add));
        if(nullptr == temp) {
            return ;
        }

        if(!add.rid || !add.arid) {
            return ;
        }

        MysqlMaster* mysqlMaster = (MysqlMaster*)param;
        mysqlMaster->add_friend(add.rid,add.arid);

        Log::Print(__func__, "Addfriend is write to DB!");
    }

    void DbHost::onRequestMsg(NetId netid,const char* buf,int size) {
        if(!netid || nullptr == buf || 
            !size){
            return ;
        }

        string data;
        data.assign(buf,size);

        inner_ipc::ReqOfflineMsgIPC request;
        request.ParseFromString(data);

        struct Msg msg;
        msg.netId = netid;
        msg.rid = request.rid();
        msg.type = request.type();
        msg.rrid = request.rrid();
        msg.count = request.count();
        msg.requestId = request.requestid();

        Message message;
        message.setwhat(Inner_What::INNER_READ_OFFLINE_MSG);
        message.setobject(&msg,sizeof(msg));
     
        Thread::ssend_message_to_thread("io_read1",message);

        Log::Print(__func__, "RequestMsg is read from DB!");
    }

    THREAD_CALLBACK_FUNC void DbHost::onRequestMsg(const void* param,Message message) {
        if(nullptr == param) {
            return ;
        }

        struct Msg msg;
        void* temp = message.getobject(&msg,sizeof(msg));
        if(nullptr == temp) {
            return ;
        }

        if(!msg.rid || !msg.rrid) {
            return ;
        }
        
        MysqlSlave* mysqlSlave = (MysqlSlave*)param;
        
        inner_ipc::ResOfflineMsgIPC response;
        response.set_rid(msg.rid);
        response.set_requestid(msg.requestId);

        bool status = mysqlSlave->read_role_msg(msg.rid,msg.rrid,&response,false);
        if(!status) {
            return ;
        }

        string data;
        response.SerializeToString(&data);

        char _data[data.length() + 1];
        memset(_data,0,data.length() + 1);
        memcpy(_data,data.c_str(),data.length());

        NetworkManage::getNetworkManage()->SendByNetId(msg.netId,
            _MsgType_Gameserver_Db::SS_REQUEST_MSG,
            _data,data.length());
    }
}