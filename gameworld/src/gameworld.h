#ifndef H_GAMEWORLD_H_
#define H_GAMEWORLD_H_

#include "eventhandle/event_manage.h"
#include "./../../common/include/servermodule.h"

using namespace engine;
using namespace gameworld::event;

namespace gameworld {
	class GameWorld : public ServerModule {
	public:
		~GameWorld(void);

		virtual void registerHandler(void);

		static GameWorld* getGameWorld(void);
		
	private:
		GameWorld(void);
		GameWorld(const GameWorld&){}
		GameWorld& operator=(const GameWorld&){ return *this; }

	private:

		static GameWorld* 	s_pGameWorld;
	};
}

#endif