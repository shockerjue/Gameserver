#ifndef H_CHATSERVER_H_
#define H_CHATSERVER_H_

#include "eventhandle/event_manage.h"
#include "./../../common/include/servermodule.h"

using namespace engine;
using namespace chatserver::event;

namespace chatserver {
	class ChatServer : public ServerModule {
	public:
		~ChatServer(void);

		virtual void registerHandler(void);

		static ChatServer* getChatServer(void);

	private:
		ChatServer(void);
		ChatServer(const ChatServer&){}
		ChatServer& operator=(const ChatServer&){ return *this; }

	private:
		static ChatServer* 	s_pChatServer;
	};
}

#endif 