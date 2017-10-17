#include "gameserver.h"

namespace gameserver {
	GameServer*	GameServer::s_pGameServer = nullptr;

	GameServer::GameServer(void){
	}

	GameServer::~GameServer(void){
		KeyManage* ptemp = KeyManage::getKeyManage();
		
		delete ptemp;
		ptemp = nullptr;
	}

	GameServer* GameServer::getGameServer(void){
		if(nullptr == GameServer::s_pGameServer){
			GameServer::s_pGameServer = new GameServer();
		}

		return GameServer::s_pGameServer;
	}

	void GameServer::registerHandler(void){
		EventManage::registerMsgHandle();
		EventManage::registerThreadMsgHandle();
	}
}