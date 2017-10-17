#ifndef H_LOGSERVER_H_
#define H_LOGSERVER_H_

#include "eventhandle/event_manage.h"
#include "./../../common/include/servermodule.h"


using namespace engine;
using namespace logserver::event;

namespace logserver {
	class LogServer : public ServerModule {
	public:
    	~LogServer(void);

    	virtual void registerHandler(void);

    	static LogServer* getLogServer(void);
    	
	private:
		LogServer(void);
		LogServer(const LogServer&){}
		LogServer& operator=(const LogServer&){ return *this; }

	private:
		static LogServer* 	s_pLogServer;
	};
}

#endif
