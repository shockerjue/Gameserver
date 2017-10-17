#include "./../include/servermodule.h"

namespace engine {

	ServerModule::ServerModule(void){
		this->m_pNetworkCallback = nullptr;
		this->m_pEngineCallback = nullptr;
		this->m_pEngine = Engine::getEngine();
	}

	ServerModule::~ServerModule(void){
		if(nullptr != this->m_pEngine){
			delete this->m_pEngine;

			this->m_pEngine = nullptr;
		}
	}

	void ServerModule::onInit(void){
		this->m_pEngine->onInit();
	}

	void ServerModule::onStart(void){
		this->m_pEngine->onStart();
		
		this->m_pEngine->SetMsgCallback(this->m_pNetworkCallback);
		this->m_pEngine->SetEngineCallback(this->m_pEngineCallback);
	}

	void ServerModule::onStop(void){
		this->m_pEngine->onStop();
	}

	void ServerModule::Run(void){
		this->m_pEngine->Run();
	}

	void ServerModule::onLoadConfig(const string file){
		this->m_pEngine->onLoadConfig(file);
	}

	void ServerModule::ConnectHostByIp(const char* ip,unsigned short port,
		ConnectType type,ConnectType peerType){
		if(nullptr == ip || !port){
			return ;
		}

		NetworkAddress address(ip,4);
		this->m_pEngine->ConnectByAddress(address,port,type,peerType);
	}

	void ServerModule::ConnectHostBySign(const char* hostsign,const char* portsign,
		ConnectType type,ConnectType peerType){
		if(nullptr == hostsign || nullptr == portsign){
			return ;
		}

		this->m_pEngine->ConnectByName(hostsign,portsign,type,peerType);
	}

}