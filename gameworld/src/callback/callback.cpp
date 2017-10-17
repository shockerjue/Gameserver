#include <iostream>

#include "callback.h"

namespace gameworld{
	Callback::Callback(void){

	}

	Callback::~Callback(void){
		
	}

	void Callback::onClose(Network network){
		cout<<"disconnected "<< network.getAddress().getHostAddress()<<endl;
	}
}