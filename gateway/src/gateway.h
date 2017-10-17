#ifndef H_GATEWAY_H_
#define H_GATEWAY_H_

#include "eventhandle/event_manage.h"
#include "./../../common/include/servermodule.h"

using namespace engine;
using namespace gateway::event;

namespace gateway {
	class Gateway : public ServerModule{
	public:
		~Gateway(void);

		static Gateway* getGateway(void);

		virtual void registerHandler(void);
	private:
		Gateway(void);
		Gateway(const Gateway&){}
		Gateway& operator=(const Gateway&){ return *this; }

	private:

		static Gateway*	s_pGateway;
	};
}

#endif