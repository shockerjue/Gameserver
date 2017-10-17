#include "chatserver.h"


namespace chatserver {
	ChatServer* ChatServer::s_pChatServer = nullptr;

	ChatServer::ChatServer(void){
		
	}

	ChatServer::~ChatServer(void){
    	
	}

	ChatServer* ChatServer::getChatServer(void){
		if(nullptr == ChatServer::s_pChatServer){
			ChatServer::s_pChatServer = new ChatServer();
		}

		return ChatServer::s_pChatServer;
	}

	void ChatServer::registerHandler(void){
		EventManage::registerMsgHandle();
		EventManage::registerThreadMsgHandle();
	}
}