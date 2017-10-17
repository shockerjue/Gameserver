#include "gateway.h"

namespace gateway {
	Gateway*	Gateway::s_pGateway = nullptr;

	Gateway::Gateway(void){
		
	}

	Gateway::~Gateway(void){
    	
	}

	Gateway* Gateway::getGateway(){
		if(nullptr == Gateway::s_pGateway){
			Gateway::s_pGateway = new Gateway();
		}

		return Gateway::s_pGateway;
	}

	void Gateway::registerHandler(void){
		EventManage::registerMsgHandle();
		EventManage::registerThreadMsgHandle();
	}
}