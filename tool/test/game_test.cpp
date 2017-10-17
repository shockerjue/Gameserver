#include "game_test.h"

GameTest::GameTest(){

}

GameTest::~GameTest(void) {

}

bool GameTest::authri(const char* ip,const int port,const char * name,
    RoleLoginResponse* response) {
    if(nullptr == ip || 0 >= port) {
        Log::Perror(__func__, "Server address is error!");

        return false;
    }

    if(nullptr == name) {
        Log::Perror(__func__,"Role name is empty!");

        return false;
    }

    if(nullptr == response) {
        Log::Perror(__func__,"Response is empty!");

        return false;
    }

    NetworkAddress address(ip,4);
	Network network(network::Protocol::TCP);
	bool ret = network.Connect(address,port);
	if(!ret){
		Log::Perror(__func__,"Connect fail!");

		return false;
    }

    RoleLoginRequest role;
	role.set_platomid("1");
	role.set_username(string(name));

	string data;
	role.SerializeToString(&data);

	struct MessageHeader header;
	header.msg_type = 500;
	header.msg_size = data.length();
	Log::Print(__func__,header.toString());
	
    char _data[sizeof(struct MessageHeader) + data.length()];
    memset(_data,0,sizeof(struct MessageHeader) + data.length());
	memcpy(_data,(const char*)&header,sizeof(struct MessageHeader));
	memcpy(_data +sizeof(struct MessageHeader),data.c_str(),data.length());

	int size = network.Send(_data,sizeof(_data));

	sleep(2);
    Log::Print(__func__,size);
    
    char buf[1024] = {0};
	size = network.Recv(buf,1024);
	if(0 == size){
		Log::Perror(__func__,"Authori recv fail!");
        network.Disconnect();

		return false;
    }
    
	string data1 = buf + sizeof(struct MessageHeader);
	response->ParseFromString(data1);

	Log::Print(__func__,response->address());
	Log::Print(__func__,response->port());
	Log::Print(__func__,response->rid());
    Log::Print(__func__,response->key());
    Log::Print(__func__,response->status());
    Log::Print(__func__,response->msg());

    if(0 != response->status()) {
        bool ret = this->createRole(network,name,"",response);

        if(!ret) {
            network.Disconnect();
        }

        return ret;
    }else {
        network.Disconnect();
        this->m_rolename = string(name);

        return true;
    }
}

bool GameTest::createRole(Network network,const char* name,const char* pwd,
    RoleLoginResponse* response) {
    if(nullptr == name || nullptr == pwd) {
        Log::Print(__func__,"Name or pwd is empty");

        return false;
    }

    if(!network.isConnect()) {
        Log::Print(__func__,"Network is hav't connect!");

        return false;
    }

    if(nullptr == response) {
        Log::Print(__func__, "Reponse is empty");

        return false;
    }

    CreateRoleRequest request;
    request.set_platomid("1");
    request.set_username(string(name));
    
    string data;
    request.SerializeToString(&data);

    struct MessageHeader header;
	header.msg_type = 501;
	header.msg_size = data.length();
    Log::Print(__func__,header.toString());
    
    char _data[sizeof(struct MessageHeader) + data.length()];
    memset(_data,0,sizeof(struct MessageHeader) + data.length());
    memcpy(_data,(const char*)&header,sizeof(struct MessageHeader));
    memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());

    int size = network.Send(_data,sizeof(_data));
    
    sleep(2);
    Log::Print(__func__,size);
    
    char buf[1024] = {0};
    size = network.Recv(buf,1024);
    if(0 == size){
        Log::Perror(__func__,"Authori recv fail!");

        return false;
    }
    
    string data1 = buf + sizeof(struct MessageHeader);
    response->ParseFromString(data1);

    Log::Print(__func__,response->address());
    Log::Print(__func__,response->port());
    Log::Print(__func__,response->rid());
    Log::Print(__func__,response->key());
    Log::Print(__func__,response->status());
    Log::Print(__func__,response->msg());

    if(0 != response->status()) {
        return false;
    }

    network.Disconnect();
    this->m_rolename = string(name);

    return true;
}

bool GameTest::login(RoleLoginResponse* reponse) {
    NetworkAddress address(reponse->address().c_str(),4);
    bool ret = this->m_network.Create(network::Protocol::TCP);
    if(!ret) {
        Log::Print(__func__, "Create network error!");

        return false;
    }

    ret = this->m_network.Connect(address,reponse->port());
    if(!ret){
		Log::Perror(__func__, "Connect gameserver fail!");

		return false;
	}

    sleep(2);
    
    LoginRequest login;
	login.set_rid(reponse->rid());
	login.set_key(reponse->key());

	string data;
    login.SerializeToString(&data);
    
    struct MessageHeader header;
	header.msg_type = 10000;
	header.msg_size = data.length();

    char _data[sizeof(struct MessageHeader) + data.length()];
    memset(_data,0,sizeof(struct MessageHeader) + data.length());
	memcpy(_data,&header,sizeof(MessageHeader));
	memcpy(_data + sizeof(MessageHeader), data.c_str(),data.length());

	int size = this->m_network.Send(_data,sizeof(struct MessageHeader) + data.length());
	if(!size) {
		Log::Perror(__func__, "Login fail!");
        this->m_network.Disconnect();

        return false;
	}

	Log::Print(__func__,login.rid());
    Log::Print(__func__,size);
    
    char buf1[256] = {0};
	size = this->m_network.Recv(buf1,256);
	if(0 == size){
        this->m_network.Disconnect();

        return false;
	}

	string _rec = buf1 + sizeof(struct MessageHeader);

    LoginResponse retlogin;
    retlogin.ParseFromString(_rec);
    if(0 != retlogin.status()){
        Log::Print(__func__,"Login fail!");
        this->m_network.Disconnect();

        return false;
    }

	cout<<"rid:"<<retlogin.rid()<<endl;
    cout<<"{status:"<<retlogin.status()<<", msg:"<<retlogin.msg()<<"}"<<endl;
    
    if(!retlogin.rid()){
		Log::Print(__func__,"Login fail!");
        this->m_network.Disconnect();

		return false;
    }
    
    this->m_rid = retlogin.rid();

    return true;
}

void GameTest::menu(void) {
    cout<<"Please Input cmd:"<<endl;
}

void GameTest::game_run(Thread* thread) {
    if(0 == this->m_network.isConnect()){
        Log::Perror(__func__, "Didn't create network connect!");

        return ;
    }

    int fd = this->m_network.GetNetId();
    thread->run("game",Thread_Call(game),&fd);
    sleep(2);

    this->requestRoleInfo();
    this->requestFriends();
    this->sendAddFriend(this->m_network,9004);
    this->sendRequestMsg(this->m_network,9000);

    while(1) {
        this->menu();

        int rid;
        string msg;
        cout<<"rid:";
        cin>>rid;

        cout<<"msg:";
        cin>>msg;

        if(0 >= rid || 0 == msg.size()) {
            Log::Print(__func__," Input error! Again Input!");

            cin.clear();
            cin.sync(); //  清空流

            continue;
        } 
    
        GameTest::sendChat(this->m_network,this->m_rid,rid,msg);
    }
}

void GameTest::requestRoleInfo(void) {
    if(0 == this->m_network.isConnect()){
        Log::Perror(__func__, "Didn't create network connect!");

        return ;
    }

    RequestRoleInfo req;
    req.set_rid(this->m_rid);

    string data;
    req.SerializeToString(&data);

    struct MessageHeader header;
    header.msg_type = 10001;
    header.msg_size = data.length();

    char _data[sizeof(struct MessageHeader) + data.length()];
    memset(_data,0,sizeof(struct MessageHeader) + data.length());
    memcpy(_data,&header,sizeof(struct MessageHeader));
    memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());

    this->m_network.Send(_data,sizeof(struct MessageHeader) + data.length());

    Log::Print(__func__, "Request RoleInfo ");
}

void GameTest::requestFriends(void) {
    if(0 == this->m_network.isConnect()){
        Log::Perror(__func__, "Didn't create network connect!");

        return ;
    }

    RequestFrineds req;
    req.set_type(2);

    string data;
    req.SerializeToString(&data);

    struct MessageHeader header;
    header.msg_type = 10002;
    header.msg_size = data.length();

    char _data[sizeof(struct MessageHeader) + data.length()];
    memset(_data,0,sizeof(struct MessageHeader) + data.length());
    memcpy(_data,&header,sizeof(struct MessageHeader));
    memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());

    this->m_network.Send(_data,sizeof(struct MessageHeader) + data.length());

    Log::Print(__func__, "Request Friends  ");
}

void GameTest::sendChat(Network network,int rid,int did,string msg) {
    if(!network.isConnect()) {
        return ;
    }

    if(!rid || 0 == msg.length() || !did) {
        return ;
    }

    SendChat chat;
    chat.set_rid(rid);
    chat.set_did(did);
    chat.set_msg_type(engine::MsgType::TEXT);
    chat.set_msg_size(msg.length());
    chat.set_msg(msg);

    string data;
    chat.SerializeToString(&data);

    struct MessageHeader header;
    header.msg_type = 10100;
    header.msg_size = data.length();

    char _data[sizeof(struct MessageHeader) + data.length()];
    memset(_data,0,sizeof(struct MessageHeader) + data.length());
    memcpy(_data,&header,sizeof(struct MessageHeader));
    memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());
    
    network.Send(_data,sizeof(struct MessageHeader) + data.length());

    Log::Print(__func__,msg);
}

void GameTest::sendAddFriend(Network network,int arid) {
    if(!arid) {
        return ;
    }

    RequestAddFriend req;
    req.set_arid(arid);

    string data;
    req.SerializeToString(&data);

    struct MessageHeader header;
    header.msg_type = 10003;
    header.msg_size = data.length();

    char _data[sizeof(struct MessageHeader) + data.length()];
    memset(_data,0,sizeof(struct MessageHeader) + data.length());
    memcpy(_data,&header,sizeof(struct MessageHeader));
    memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());
    
    network.Send(_data,sizeof(struct MessageHeader) + data.length());

    Log::Print(__func__,"Send add friend");
}

void GameTest::sendRequestMsg(Network network,int rrid) {
    if(!rrid) {
        return ;
    }

    RequestOfflineMsgs req;
    req.set_rrid(rrid);
    req.set_type(0);
    req.set_count(0);

    string data;
    req.SerializeToString(&data);

    struct MessageHeader header;
    header.msg_type = 10103;
    header.msg_size = data.length();

    char _data[sizeof(struct MessageHeader) + data.length()];
    memset(_data,0,sizeof(struct MessageHeader) + data.length());
    memcpy(_data,&header,sizeof(struct MessageHeader));
    memcpy(_data + sizeof(struct MessageHeader),data.c_str(),data.length());

    network.Send(_data,sizeof(struct MessageHeader) + data.length());

    Log::Print(__func__, "Send request Msg");
}

void GameTest::onRecvChat(Network network,const char* msg,int size) {
    if(nullptr == msg || !size) {
        return ;
    }

    string data;
    data.assign(msg,size);

    RecvChat rchat;
    bool ret = rchat.ParseFromString(data);
    if(ret) {
        int did = rchat.rid();
        // GameTest::sendChat(network,rchat.rid(),rchat.sid(),rchat.msg());
    }

    Log::Print(__func__,rchat.msg());
}

void GameTest::onAddFriend(Network network,const char* msg,int size) {
    if(nullptr == msg || !size) {
        return ;
    }

    string data;
    data.assign(msg,size);

    ResponseAddFriend response;
    response.ParseFromString(data);

    Log::Print(__func__,"AddFriend response! name:" + response.friend_().username()  
        + " status:" + Util::int2str(response.status()));
}

void GameTest::onRecvRoleInfo(Network network,const char* msg,int size) {
    if(nullptr == msg || !size) {
        return ;
    }

    string data;
    data.assign(msg,size);

    ResponseRoleInfo res;
    res.ParseFromString(data);

    Log::Print(__func__, "recv role info! " + Util::int2str(res.reqrid()));
}

void GameTest::onRecvFriends(Network network,const char* msg,int size) {
    if(nullptr == msg) {
        return ;
    }

    string data;
    data.assign(msg,size);

    ResponseFriends response;
    response.ParseFromString(data);
    ::google::protobuf::RepeatedPtrField< ::gameserver_proto::FriendItem >* friendsitems = response.mutable_friends();
    ::google::protobuf::RepeatedPtrField< ::gameserver_proto::FriendItem >::iterator it = friendsitems->begin();
    while(it != friendsitems->end()) {
       Log::Print(__func__,"{rolename: " + it->username() + ", rid: " + Util::int2str(it->rid()) + " }");

       it++;
    }

    Log::Print(__func__, "recv friends info!");
}

void GameTest::onRequestMsg(Network network,const char* msg,int size) {
    string data;
    data.assign(msg,size);

    ResponseOfflineMsgs response;
    response.ParseFromString(data);
    ::google::protobuf::RepeatedPtrField< ::gameserver_proto::MsgItem>* msgitems = response.mutable_msgs();
    ::google::protobuf::RepeatedPtrField< ::gameserver_proto::MsgItem>::iterator it = msgitems->begin();
    while(it != msgitems->end()) {
        Log::Print(__func__,"rid:" + Util::int2str(it->srid()) + "  msg:" + it->msg());

        it++;
    }

    Log::Print(__func__,"Recv Msg len: " + Util::int2str(response.count()));
}

Thread_Func(game) {
    int netid = *((int*)_thread);
    if(0 >= netid) {
        Log::Perror(__func__, "Thread start error!");

        return nullptr;
    }

    Network network(netid);

    fd_set readfds;
    FD_ZERO(&readfds);

    map<int,CALLBACK> callback;
    callback[10100]   = GameTest::onRecvChat;
    callback[10001]   = GameTest::onRecvRoleInfo;
    callback[10002]   = GameTest::onRecvFriends;
    callback[10003]   = GameTest::onAddFriend;
    callback[10103]   = GameTest::onRequestMsg;

    while(1){
        FD_SET(netid,&readfds);

        int ret = select(netid + 1,&readfds,nullptr,nullptr,nullptr);
        if(ret < 0){
            Log::Perror(__func__,"Thread already over!");

            break ;
        }

        if(0 < ret && FD_ISSET(netid,&readfds)){
            char rmsg[16384] = {0};
            ret = network.Recv(rmsg,16384);
            if(!ret){
                network.Disconnect();
                
                break;
            }
        
            int pos = 0;
            const char* p = rmsg;
            while(nullptr != p && pos < ret) {
                struct MessageHeader* header = (struct MessageHeader*)p;
            
                const int msg_type = header->msg_type;
                const int msg_size = header->msg_size;
                char msg[msg_size];
                memset(msg,0,msg_size);
                memcpy(msg,p + sizeof(struct MessageHeader),msg_size);
                
                map<int,CALLBACK>::iterator it = callback.find(msg_type);
                if(it == callback.end()){
                    Log::Warning(__func__, "Can't prase code " + Util::int2str(msg_type));
                }else{
                    callback[msg_type](network,msg,msg_size);
                }

                pos = pos + sizeof(struct MessageHeader) + msg_size;
                p = p + sizeof(struct MessageHeader) + msg_size;
            }
        }
    }

    return nullptr;
}

void game_test(const char* ip,const int port,const char * name) {
    GameTest game;

    RoleLoginResponse response;
    bool ret = game.authri(ip,port,name,&response);
    if(!ret) {
        Log::Print(__func__, "game.authri error!");

        return ;
    }

    ret = game.login(&response);
    if(!ret) {
        return ;
    }

    Thread thread;
    game.game_run(&thread);
}