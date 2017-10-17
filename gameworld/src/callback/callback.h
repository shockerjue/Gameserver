#ifndef H_CALLBACK_H_
#define H_CALLBACK_H_

#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/include/thread.h"
#include "./../../../common/engine/callback.h"
#include "./../../../common/msgdefine/msghandle.h"

using namespace util;
using namespace engine;
using namespace msghandle;
using namespace engine::thread;


namespace gameworld {
	class Callback: public NetworkCallback {
	public:
		Callback(void);
		~Callback(void);
		
		virtual void onClose(Network network);
	
	private:
		Callback(const Callback&){}
		Callback& operator=(const Callback&){ return *this; }
	};
}

#endif