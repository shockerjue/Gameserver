#include "logserver.h"

namespace logserver {
	LogServer* LogServer::s_pLogServer = nullptr;

	LogServer::LogServer(void){
		
	}

	LogServer::~LogServer(void){
    	
	}

	LogServer* LogServer::getLogServer(void){
		if(nullptr == LogServer::s_pLogServer){
			LogServer::s_pLogServer = new LogServer();
		}

		return LogServer::s_pLogServer;
	}

	void LogServer::registerHandler(void){
		EventManage::registerMsgHandle();
		EventManage::registerThreadMsgHandle();
	}
}