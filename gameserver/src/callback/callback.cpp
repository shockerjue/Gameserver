#include "callback.h"

namespace gameserver {
	Callback::Callback(void){

	}

	Callback::~Callback(void){

	}

	void Callback::onClose(Network network){
		Log::Print(__func__, "Client close!");

		if(ConnectType::NONEDEFAULT == network.GetType()){

			LoginHost::notify2loginforclose(network);
			ChatHost::notify2chatserverforlogout(network);

			RoleManage::getRoleManage()->removeRole(network.GetNetId());

			DDos::noti_ddos_thread("ddos",network.GetNetId(),
				5,S_WhatType::W_NOTI_NETID_CLOSED);
		}

		return ;
	}

	void Callback::onTimeout(Network network){
		Log::Print(__func__, "onTimeout");
		if(ConnectType::NONEDEFAULT == network.GetType()){
			LoginHost::notify2loginforclose(network);
			ChatHost::notify2chatserverforlogout(network);

			Log::Print(__func__, "onTimeout---------------------->");
		}
	}

	void Callback::onConnect(Network network){
		Log::Print(__func__, "New connect to here!");

		LoginHost::notify2loginforconnect(network);

		DDos::noti_ddos_thread("ddos",network.GetNetId(),
			5,S_WhatType::W_NOTI_NETID_INSERT);

		return ;
	}
}