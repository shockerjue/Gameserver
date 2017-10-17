#ifndef H_CALLBACK_H_
#define H_CALLBACK_H_

#include "./../role/rolemanage.h"
#include "./../chathost/chathost.h"
#include "./../loginhost/loginhost.h"
#include "./../../../common/ddos/ddos.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/callback.h"
#include "./../../../common/msgdefine/msghandle.h"

using namespace role;
using namespace util;
using namespace engine;
using namespace chathost;
using namespace loginhost;
using namespace msghandle;
using namespace engine::ddos;


namespace gameserver {
	class Callback: public NetworkCallback {
	public:
		Callback(void);
		~Callback(void);
		
		virtual void onClose(Network network);
		virtual void onTimeout(Network network);
		virtual void onConnect(Network network);

	private:
		Callback(const Message&){}
		Callback& operator=(const Message&){ return *this; }
	};
}

#endif