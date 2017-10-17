#include "login.h"

namespace login {
	Login* Login::s_pLogin = nullptr;

	Login::Login(void){
		
	}

	Login::~Login(void){
    
	}

	Login* Login::getLogin(){
		if(nullptr == Login::s_pLogin){
			Login::s_pLogin = new Login();
		}

		return Login::s_pLogin;
	}

	void Login::registerHandler(void){
		EventManage::registerMsgHandle();
		EventManage::registerThreadMsgHandle();
	}
}