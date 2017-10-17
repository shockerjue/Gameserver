#include "engine.h"

#ifdef _cplusplus
    extern "C"{
#endif
	#include <sys/types.h>
	#include <string.h>
	#include <fcntl.h>
	#include <errno.h>
    #include <sys/socket.h>

    #ifdef __linux
		#include <sys/epoll.h>

    #elif __APPLE__
    	#include <sys/event.h>

    #endif
#ifdef _cplusplus
    }
#endif
    
namespace engine {
	
	
    Engine* 		Engine::s_pEngine = nullptr;
	unsigned short 	Engine::s_bindport = 0;
	char 			Engine::s_bindip[16] = {0};
	ServerType 		Engine::s_servertype = ServerType::ST_DEFAULT;

    Engine* Engine::getEngine(void){
    	if(nullptr == Engine::s_pEngine){
    		Engine::s_pEngine = new Engine();
    	}

    	return Engine::s_pEngine;
    }

	Engine::Engine(void){
        this->m_eventfd = EPOLL_ERROR;
		this->m_pNetCallback = nullptr;
		this->m_pEngineCallback = nullptr;
		this->m_pBaseCallback = new BaseCallback();
	}

	Engine::~Engine(void){
		if(this->m_eventfd){
			close(this->m_eventfd);
		}

		ConfigModule* config = Engine::getConfigModule();
		if(nullptr != config){
			delete config;
			
			config = nullptr;
		}

		if(nullptr != this->m_pBaseCallback){
			delete this->m_pBaseCallback;

			this->m_pBaseCallback = nullptr;
		}
	}

	ConfigModule* Engine::getConfigModule(void){
		return ConfigModule::getConfigModule();
	}

	string Engine::getName(void){
		string name = Engine::getConfigModule()->getStringByKey("name");

		return name;
	}

	void Engine::onLoadConfig(const string file){
		ConfigModule* pConfigModule = Engine::getConfigModule();
		if(!pConfigModule->onLoad(file)){
			Log::Perror(__func__, "load config file Error!");

			return ;
		}

		Log::s_proc_name = pConfigModule->getStringByKey("name");

		if(nullptr != this->m_pEngineCallback){
			this->m_pEngineCallback->onLoadConfig(file);
		}
	}

	void Engine::onInit(void){
		signal(SIGPIPE, Engine::onSigpipe); 
		
		#ifdef __linux
			this->m_eventfd = epoll_create(100);
		#elif __APPLE__ 
			this->m_eventfd = kqueue();
		#else
			Log::Print(__func__,"Didn't support platom!");

			exit(1);
		#endif

	    if(this->m_eventfd == EPOLL_ERROR){
	        Log::Perror(__func__,"Events manage create fail!\n");

	        exit(1);
	    }

		if(!this->m_network.Create(network::Protocol::TCP)){
			Log::Print(__func__,"Server Socket create fail!");

			exit(1);
		}

		int port = Engine::getConfigModule()->getIntByKey("port");
		if(!port){
			Log::Perror(__func__,"Config file didn't loaded or Config error!");

			exit(1);
		}

		string host = Engine::getConfigModule()->getStringByKey("host");
		if(0 == host.size()){
			Log::Perror(__func__, "Config file didn.t loaded or Config error!");

			exit(1);
		}

		int op = 1;
	    this->m_network.Bind(host.c_str(),port);
		this->m_network.Listen(5);
		this->m_network.SetType(ConnectType::GATEWAY);
		this->m_network.SetMode(network::fdmode::NBLK);
		this->m_network.SetOpt(SOL_SOCKET,SO_REUSEADDR,(const void*)&op,sizeof(op));

		this->addEvent(this->m_network);

		string address = Engine::getConfigModule()->getStringByKey("address");
		strcpy(Engine::s_bindip,address.c_str());
		Engine::s_bindport = port;

		if(nullptr != this->m_pEngineCallback){
			this->m_pEngineCallback->onInit();
		}
	}

	void Engine::onStart(){
		if(EPOLL_ERROR == this->m_eventfd){
        	Log::Perror(__func__,"Didn;t call onInit function!");

        	return ;
        }

        if(nullptr != this->m_pEngineCallback){
			this->m_pEngineCallback->onStart();
		}
	}

	void Engine::onStop(){
		if(nullptr != this->m_pEngineCallback){
			this->m_pEngineCallback->onStop();
		}
	}

	void Engine::SetMsgCallback(NetworkCallback* callback){
		if(nullptr == callback){
			return ;
		}

		this->m_pNetCallback = callback;
	}

	void Engine::SetEngineCallback(EngineCallback* callback){
		if(nullptr == callback){
			return ;
		}

		this->m_pEngineCallback = callback;
	}

	void Engine::addEvent(Network network){
		if (EPOLL_ERROR == this->m_eventfd){
			return ;
		}

		if(!network.GetNetId()){
			return ;
		}

		#ifdef __linux
			struct epoll_event event;
		    event.data.fd = network.GetNetId();
		    event.events = EPOLLIN;

		    epoll_ctl(this->m_eventfd,EPOLL_CTL_ADD,network.GetNetId(),&event);
		#elif __APPLE__
		    struct kevent event;
		    EV_SET(&event,	network.GetNetId(), EVFILT_READ, EV_ADD, 0,0,nullptr);
		    int ret = kevent(this->m_eventfd, &event, 1, nullptr, 0, nullptr);
			if (-1 == ret){
				perror("kevent register");

				exit(1);
			}
			if (event.flags & EV_ERROR){
				perror("Event error: ");

				exit(1);
			}
		#else
			Log::Print(__func__, "Didn't support platom!");
		#endif
		
	}

	bool Engine::ConnectByIp(const char* ip,unsigned short port,ConnectType type,ConnectType peerType){
		if(nullptr == ip || !port){
			return false;
		}

		NetworkAddress address(ip,4);
		Network network(network::Protocol::TCP);

		if(!network.Connect(address,port)){
			Log::Perror(__func__,string("Connect fail to ") + string(ip));

			return false;
		}

		network.SetType(type);
		network.SetMode(network::fdmode::NBLK);

		NetworkManage::addNetwork(network);

		this->addEvent(network);

		if(ConnectType::NONEDEFAULT == peerType){
			return true;
		}
		
		struct NetworkType peer;
		peer.m_type = peerType;
		peer.m_port = Engine::s_bindport;
		strcpy(peer.m_ip,Engine::s_bindip);
		strcpy(peer.m_name,this->getName().c_str());

		struct MessageHeader header;
		header.msg_type = S_SystemType::SS_SERVER_LOGIN;
		header.msg_size = sizeof(struct NetworkType);

		int len = sizeof(struct NetworkType) + sizeof(struct MessageHeader);
		char* p = new char[len + 1];
		if(nullptr == p){
			return false;
		}

		memcpy(p,&header,sizeof(struct MessageHeader));
		memcpy(p + sizeof(struct MessageHeader),&peer,sizeof(struct NetworkType));
		
		network.Send(p, len);

		delete[] p;
		p = nullptr;

		return true;
	}

	bool Engine::ConnectByName(const char* hostsign,const char* portsign,ConnectType type,ConnectType peerType){
		if(nullptr == hostsign || nullptr == portsign){
			return false;
		}

		int port = Engine::getConfigModule()->getIntByKey(string(portsign));
		if(!port){
			Log::Perror(__func__, string(portsign) + " is Error!");

			exit(1);
		}

		string host = Engine::getConfigModule()->getStringByKey(string(hostsign));
		if(0 == host.size()){
			Log::Perror(__func__,string(hostsign) + " is Error!");

			exit(1);
		}

		const char* p = host.c_str();
		const char pre = *p;
		switch(pre){
			case 'w':{
				return this->ConnectByDomain(host.c_str(),port,type,peerType);
			}break;
			default:{
				return this->ConnectByIp(host.c_str(),port,type,peerType);
			}break;
		}

		return true;
	}

	bool Engine::ConnectByDomain(const char* domain,unsigned short port,ConnectType type,ConnectType peerType){
		if(nullptr == domain || !port){
			return false;
		}

		NetworkAddress address(domain);
		Network network(network::Protocol::TCP);

		if(!network.Connect(address,port)){
			Log::Perror(__func__,string("Connect fail to ") + string(domain));

			return false;
		}

		network.SetType(type);
		network.SetMode(network::fdmode::NBLK);

		NetworkManage::addNetwork(network);

		this->addEvent(network);

		struct NetworkType peer;
		peer.m_type = peerType;
		peer.m_port = Engine::s_bindport;
		strcpy(peer.m_ip,Engine::s_bindip);
		strcpy(peer.m_name,this->getName().c_str());

		struct MessageHeader header;
		header.msg_type = S_SystemType::SS_SERVER_LOGIN;
		header.msg_size = sizeof(struct NetworkType);

		int len = sizeof(struct NetworkType) + sizeof(struct MessageHeader);
		char* p = new char[len + 1];
		if(nullptr == p){
			return false;
		}

		memcpy(p,&header,sizeof(struct MessageHeader));
		memcpy(p + sizeof(struct MessageHeader),&peer,sizeof(struct NetworkType));
		
		network.Send(p, len);

		delete[] p;
		p = nullptr;

		return true;
	}

	bool Engine::ConnectByAddress(NetworkAddress address,unsigned short port,ConnectType type,ConnectType peerType){
		Network network(network::Protocol::TCP);
		
		if(!network.Connect(address,port)){
			Log::Perror(__func__,string("Connect fail to ") + address.getHostAddress());

			return false;
		}

		network.SetType(type);
		network.SetMode(network::fdmode::NBLK);

		NetworkManage::addNetwork(network);

		this->addEvent(network);

		struct NetworkType peer;
		peer.m_type = peerType;
		peer.m_port = Engine::s_bindport;
		strcpy(peer.m_ip,Engine::s_bindip);
		strcpy(peer.m_name,this->getName().c_str());

		struct MessageHeader header;
		header.msg_type = S_SystemType::SS_SERVER_LOGIN;
		header.msg_size = sizeof(struct NetworkType);

		int len = sizeof(struct NetworkType) + sizeof(struct MessageHeader);
		char* p = new char[len + 1];
		if(nullptr == p){
			return false;
		}

		memcpy(p,&header,sizeof(struct MessageHeader));
		memcpy(p + sizeof(struct MessageHeader),&peer,sizeof(struct NetworkType));
		
		network.Send(p, len);

		delete[] p;
		p = nullptr;

		return true;
	}

	void Engine::AddThreadEvent(Thread thread){
    	if(EPOLL_ERROR == this->m_eventfd){
    		Log::Perror(__func__, "Didn't call onInit function!");

    		return ;
    	}

		if(0 >= thread.getrecvfd()){
			Log::Perror(__func__, "Didn't create thread!");

			return ;
		}

		if(NetworkManage::getNetworkManage()->isHavNetwork(thread.getrecvfd())){
			Log::Warning(__func__, "Thread event already added!");

			return ;
		}

		Network network(thread.getrecvfd());
		network.SetType(ConnectType::THREAD_TYPE);
		network.SetMode(network::fdmode::NBLK);


		NetworkManage::addNetwork(network);

		this->addEvent(network);

		Log::Print(__func__, "Add thread event!");
	}

	void Engine::onAccept(void){
    	if(EPOLL_ERROR == this->m_eventfd){
    		Log::Perror(__func__, "Didn't call onInit function!");

    		return ;
    	}

		while(1){
			Network network;
			bool ret = this->m_network.Accept(&network);
			if(!ret){
				break ;
			}

			network.SetMode(network::fdmode::NBLK);
			network.SetType(ConnectType::NONEDEFAULT);

			#ifdef __linux
				struct epoll_event ev;
				ev.data.fd = network.GetNetId();
		        ev.events = EPOLLIN | EPOLLRDHUP;

		        epoll_ctl(this->m_eventfd,EPOLL_CTL_ADD,network.GetNetId(),&ev);
		        
	        #elif __APPLE__
		        struct kevent event;
			    EV_SET(&event,	network.GetNetId(), EVFILT_READ, EV_ADD, 0,0,nullptr);
			    int _ret = kevent(this->m_eventfd, &event, 1, nullptr, 0, nullptr);
				if (-1 == _ret){
					perror("kevent register");

					exit(1);
				}

				if (event.flags & EV_ERROR){
					perror("Event error");

					exit(1);
				}
				
	        #endif
				
			if(nullptr != this->m_pNetCallback){
				this->m_pNetCallback->onConnect(network);
			}

	        NetworkManage::addNetwork(network);

	        Log::Print(__func__,"Connect from :" + network.getAddress().getHostAddress());
		}
	}

	void Engine::onRecv(NetId netid){
		if(!netid){
			return ;
		}

		if(!NetworkManage::getNetworkManage()->isHavNetwork(netid)) {
			return ;
		}

		Network network = NetworkManage::getNetworkManage()->getNetwork(netid);
		unsigned short type = network.GetType();
		
		char buf[16384] = {0};
		int len = 0;
		while(len < 16384){
			int ret = network.Recv(buf,2048);
			if(0 >= ret){
				
				break;
			}

			len = len + ret;
		}

		if(0 >= len){
			this->onClose(netid);
			
			return ;
		}

		const char* p = buf;
		switch(type) {
			case ConnectType::THREAD_TYPE:{
				if(nullptr != this->m_pBaseCallback) {
					this->m_pBaseCallback->onThreadMsg(p,len,this->m_pNetCallback);
				}
			}break;
			default:{
				if(nullptr != this->m_pBaseCallback) {
					this->m_pBaseCallback->onRecv(network,p,len);
				}
			}break;
		}
	}

	void Engine::onClose(NetId netid){
		if(!netid){
			return ;
		}

		if(!NetworkManage::getNetworkManage()->isHavNetwork(netid)) {
			return ;
		}

		Network network = NetworkManage::getNetworkManage()->getNetwork(netid);

		if(nullptr != this->m_pNetCallback){
			this->m_pNetCallback->onClose(network);
		}

		if(network.GetType() == ConnectType::NONEDEFAULT){
			Timeout::noti_ddos_thread("timeout",netid,10,
				S_WhatType::W_NOTI_RECV_CLOSED);
		}

		NetworkManage::removeNetwork(network);
	}

	void Engine::Run(void){
    	if(EPOLL_ERROR == this->m_eventfd){
    		Log::Perror(__func__, "Didn't call onInit function!");

    		return ;
    	}

		Log::Print(__func__,"Server start running .....");

		while(1){
			#ifdef __linux 
				struct epoll_event events[512];
		        int eret = epoll_wait(this->m_eventfd , events , 512 , -1);

		        if(EPOLL_ERROR == eret){
		        	Log::Perror(__func__,strerror(errno));
		        	
		            break;
		        }

		        int i = 0;
		        for (int i = 0; i < eret; ++i){
		        	if(events[i].data.fd == this->m_network.GetNetId() &&
		        	 events[i].events & EPOLLIN){
		        		this->onAccept();
		        	
			        }else if(events[i].events & EPOLLRDHUP || events[i].events & EPOLLERR){
			        	epoll_ctl(this->m_eventfd,EPOLL_CTL_DEL,
			        		events[i].data.fd,events);

			        	this->onClose(events[i].data.fd);
			        	
			        	Log::Print(__func__,"Close connect !");

			        }else if(events[i].events & EPOLLIN) {
			        	this->onRecv(events[i].data.fd);
			        }else {
			        	Log::Warning(__func__,"Don't known Error " + string(strerror(errno)));
			        }
		        }
	        #elif __APPLE__
	        	struct kevent events[1024];
	        	int eret = kevent(this->m_eventfd, nullptr, 0, events,	1024, nullptr);
	        	if(EPOLL_ERROR == eret){
	        		Log::Perror(__func__,"epoll_wait call error!");

	        		break;
	        	}

	        	int i = 0;
	        	for(i = 0; i < eret; i++){
	        		int netid = events[i].ident;
	        		if(netid == this->m_network.GetNetId()){
	        			this->onAccept();
	        		}else {
	        			this->onRecv(netid);
	        		}
	        	}
	        #else
	        	break;
	        #endif
		}
	}

	ServerType Engine::getServerType(void){
		return Engine::s_servertype;
	}

    void Engine::setServerType(ServerType type){
    	Engine::s_servertype = type;
    }

	void Engine::onSigpipe(int sig){
		Log::Print(__func__, "Engine::onSigpipe");
	}

	map<ProtocolHeader,EngineFunc> 	EventHandle::g_msg_call;
	map<ThreadWhat,ThreadMsgFunc>	EventHandle::g_thread_msg_call;
}