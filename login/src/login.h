#ifndef H_LOGIN_H_
#define H_LOGIN_H_

#include "eventhandle/event_manage.h"
#include "./../../common/include/servermodule.h"

using namespace engine;
using namespace login::event;


namespace login{
	class Login: public ServerModule {
	public:
		~Login(void);

		static Login* getLogin(void);

		virtual void registerHandler(void);
	private:
		Login(void);
		Login(const Login&){}
		Login& operator=(const Login&){ return *this; }

	private:
		static Login* 		s_pLogin;
	};
}

#endif