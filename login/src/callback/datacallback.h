#ifndef H_DATACALLBACK_H_
#define H_DATACALLBACK_H_

#include "./../gameserver/gameserverhandle.h"
#include "./../../../common/engine/callback.h"

using namespace engine;
using namespace loginhost;

namespace login {
	class DataCallback: public NetworkCallback {
	public:
		DataCallback(void);
		~DataCallback(void);

		virtual void onClose(Network network);

	private:
		DataCallback(const DataCallback&){}
		DataCallback& operator=(const DataCallback&){ return *this; }
	};
}

#endif