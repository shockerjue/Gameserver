#include "networkmanage.h"

namespace engine {
	
	map<NetId,Network>	NetworkManage::s_manager;
	NetworkManage* NetworkManage::s_networkManage = nullptr;

	NetworkManage::NetworkManage(void){
		this->initNetType();
	}

	NetworkManage::~NetworkManage(void){
		this->freeNetType();
	}

	void NetworkManage::addNetwork(Network network){
		map<NetId,Network>::iterator it = NetworkManage::s_manager.find(network.GetNetId());
		if(it != NetworkManage::s_manager.end()){
			return ;
		}

		NetworkManage::s_manager[network.GetNetId()] = network;
	}

	void NetworkManage::removeNetwork(Network network){
		map<NetId,Network>::iterator it = NetworkManage::s_manager.find(network.GetNetId());
		if(it == NetworkManage::s_manager.end()){
			return ;
		}

		network.Disconnect();
		NetworkManage::s_manager.erase(network.GetNetId());
	}

	void NetworkManage::removeNetwork(NetId netid){
		map<NetId,Network>::iterator it = NetworkManage::s_manager.find(netid);
		if(it == NetworkManage::s_manager.end()){
			return ;
		}

		it->second.Disconnect();
		NetworkManage::s_manager.erase(netid);
	}

	NetworkManage* NetworkManage::getNetworkManage(void){
		if(nullptr == s_networkManage){
			s_networkManage = new NetworkManage();
		}

		return s_networkManage;
	}

	Network NetworkManage::getNetwork(NetId netid){
		if(!netid){
			return this->m_network;
		}

		map<NetId,Network>::iterator it = NetworkManage::s_manager.find(netid);
		if(it != NetworkManage::s_manager.end()){
			return it->second;
		}

		return this->m_network;
	}

	bool NetworkManage::isHavNetwork(NetId netid){
		if(!netid){
			return true;
		}

		map<NetId,Network>::iterator it = NetworkManage::s_manager.find(netid);
		if(it != NetworkManage::s_manager.end()){
			return true;
		}

		return false;
	}

	Network NetworkManage::getNetworkByType(ConnectType type){
		map<NetId,Network>::iterator it = NetworkManage::s_manager.begin();

		while(it != NetworkManage::s_manager.end()){
			if(it->second.GetType() == type){
				return it->second;

				break;
			}

			it++;
		}

		return this->m_network;
	}

	void NetworkManage::updateConnectType(Network network,ConnectType type){
		map<NetId,Network>::iterator it = NetworkManage::s_manager.find(network.GetNetId());
		if(it == NetworkManage::s_manager.end()){
			return ;
		}

		it->second.SetType(type);
	}

	void NetworkManage::SendByNetworkType(ConnectType type,int msgheader,const char* buf,int size){
		if(!msgheader || nullptr == buf){
			return ;
		}
		
		Network network = this->getNetworkByType(type);
		if(0 == network.isConnect()){
			return ;
		}

		char* p = new char[sizeof(struct MessageHeader) + size + 1];
		if(nullptr == p){
			return ;
		}

		struct MessageHeader header;
		header.msg_type = msgheader;
		header.msg_size = size;
		memcpy(p,&header,sizeof(struct MessageHeader));
		memcpy(p + sizeof(struct MessageHeader),buf,size);

		network.Send(p,sizeof(struct MessageHeader) + size);

		delete[] p;
		p = nullptr;

		return ;
	}

	void NetworkManage::SendByNetwork(Network network,int msgheader,const char* buf,int size){
		if(!msgheader || nullptr == buf){
			return ;
		}

		if(0 == network.isConnect()){
			return ;
		}

		char* p = new char[sizeof(struct MessageHeader) + size + 1];
		if(nullptr == p){
			return ;
		}

		struct MessageHeader header;
		header.msg_type = msgheader;
		header.msg_size = size;
		memcpy(p,&header,sizeof(struct MessageHeader));
		memcpy(p + sizeof(struct MessageHeader),buf,size);

		network.Send(p,sizeof(struct MessageHeader) + size);

		delete[] p;
		p = nullptr;

		return ;
	}

	void NetworkManage::SendByNetId(NetId netid,int msgheader,const char* buf,int size){
		if(!msgheader || nullptr == buf){
			return ;
		}

		Network network = this->getNetwork(netid);
		if(!network.isConnect()){
			return ;
		}

		char* p = new char[sizeof(struct MessageHeader) + size + 1];
		if(nullptr == p){
			return ;
		}

		struct MessageHeader header;
		header.msg_type = msgheader;
		header.msg_size = size;
		memcpy(p,&header,sizeof(struct MessageHeader));
		memcpy(p + sizeof(struct MessageHeader),buf,size);

		network.Send(p,sizeof(struct MessageHeader) + size);

		delete[] p;
		p = nullptr;

		return ;
	}

	void NetworkManage::Send2LogServer(ConnectType type,LogLevel level,string _func,string msg){
		if(0 == msg.size()){
			return ;
		}

		inner_ipc::SystemLogIPC _log;
		_log.set_level(level);
		_log.set_proc(Log::s_proc_name);
		_log.set_func(_func);
		_log.set_date(Datetime::getCurrentDate());
		_log.set_msg(msg);

		string data;
		_log.SerializeToString(&data);

		char* p = new char[sizeof(struct MessageHeader) + data.length() + 1];
		if(nullptr == p){
			return ;
		}

		struct MessageHeader header;
		header.msg_type = S_SystemType::SS_SERVER_LOG;
		header.msg_size = data.length();
		memcpy(p,&header,sizeof(struct MessageHeader));
		memcpy(p + sizeof(struct MessageHeader),data.c_str(),data.length());

		Network network = this->getNetworkByType(type);
		if(0 == network.isConnect()){
			delete[] p;
			p = nullptr;

			return ;
		}

		network.Send(p,sizeof(struct MessageHeader) + data.length());

		delete[] p;
		p = nullptr;

		return ;
	}

	void NetworkManage::addNetType(NetId netid,ConnectType type){
		if(!netid || 0 > type){
			return ;
		}

		TypeToNetwork* head = this->m_t2network[type];
		TypeToNetwork* p = head;

		TypeToNetwork* node = new TypeToNetwork();
		node->m_netid = netid;
		node->m_pNext = nullptr;

		while(nullptr != p->m_pNext && nullptr != p){
			p = p->m_pNext;
		}

		if(nullptr == p){
			return ;
		}

		p->m_pNext = node;
	}

	void NetworkManage::remvoeNetType(NetId netid,ConnectType type){
		if(!netid || 0 > type){
			return ;
		}

		TypeToNetwork* head = this->m_t2network[type];
		TypeToNetwork* p = head;
		TypeToNetwork* fore = head;

		while(p != nullptr){
			TypeToNetwork* q = p->m_pNext;
			if(p->m_netid == netid){
				delete p;
				p = nullptr;

				fore->m_pNext = q;

				break;
			}

			fore = p;
			p = q;
		}

		return ;
	}

	void NetworkManage::freeNetType(void){
		int i = 0;

		for (int i = 0; i < TypeLen; ++i){

			TypeToNetwork* head = this->m_t2network[i];
			TypeToNetwork* p = head;
			TypeToNetwork* q = p;
			while(nullptr != p){
				q = p->m_pNext;

				delete p;
				p = nullptr;

				p = q;
			}
		}
	}

	void NetworkManage::initNetType(void){
		int i = 0;

		for (int i = 0; i < TypeLen; ++i){
			TypeToNetwork* head = new TypeToNetwork();
			head->m_netid = 0;
			head->m_pNext = nullptr;

			this->m_t2network[i] = head;
		}
	}

	NetId NetworkManage::getNetworkId(ConnectType type){
		if(0 > type){
			return 0;
		}

		TypeToNetwork* head = this->m_t2network[type];
		TypeToNetwork* p = head;
		while(nullptr != p){
			if(p->m_netid){
				break;
			}

			p = p->m_pNext;
		}

		if(nullptr != p){
			return p->m_netid;
		}

		return 0;
	}
}