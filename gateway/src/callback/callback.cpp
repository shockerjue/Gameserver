#include <iostream>

#include "callback.h"

using namespace std;

namespace gateway{
	Callback::Callback(void){

	}

	Callback::~Callback(void){
		
	}

	void Callback::onConnect(Network network){
		DDos::noti_ddos_thread("ddos",network.GetNetId(),
			5,S_WhatType::W_NOTI_NETID_INSERT);
	}

	void Callback::onClose(Network network){
		Log::Print(__func__, network.getAddress().getHostAddress());
		
		DDos::noti_ddos_thread("ddos",network.GetNetId(),
			5,S_WhatType::W_NOTI_NETID_CLOSED);
	}
}