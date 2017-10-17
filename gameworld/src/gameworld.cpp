#include "gameworld.h"

namespace gameworld {
	GameWorld*	GameWorld::s_pGameWorld = nullptr;

	GameWorld::GameWorld(void){
		
	}

	GameWorld::~GameWorld(void){
	}

	GameWorld*	GameWorld::getGameWorld(void){
		if(nullptr == s_pGameWorld){
			GameWorld::s_pGameWorld = new GameWorld();
		}

		return GameWorld::s_pGameWorld;
	}

	void GameWorld::registerHandler(void){
		EventManage::registerMsgHandle();
		EventManage::registerThreadMsgHandle();
	}
}