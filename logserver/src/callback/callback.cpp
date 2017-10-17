#include "callback.h"

namespace logserver {
	Callback::Callback(void){

	}

	Callback::~Callback(void){

	}

	void Callback::onClose(Network network){

	}

	void Callback::onConnect(Network network){
		Log::Print(__func__, "New connect to here!");
	}
}