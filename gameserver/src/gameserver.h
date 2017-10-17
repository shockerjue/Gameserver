#ifndef H_GAMESERVER_H_
#define H_GAMESERVER_H_

#include "keymanage.h"
#include "eventhandle/event_manage.h"
#include "./../../common/include/servermodule.h"

using namespace engine;
using namespace gameserver::event;

namespace gameserver {
	class GameServer : public ServerModule {
	public:
		~GameServer(void);

		static GameServer*	getGameServer(void);
		
		virtual void registerHandler(void);

	private:
		GameServer(void);
		GameServer(const GameServer& game){}
		GameServer& operator=(const GameServer& game){ return *this; }

	private:
		static GameServer* 		s_pGameServer;
	};
}

#endif