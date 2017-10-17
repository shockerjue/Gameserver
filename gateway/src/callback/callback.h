/**
* Network event callback
*
*/

#ifndef H_CALLBACK_H_
#define H_CALLBACK_H_

#include "./../msghandle/msgcode.h"
#include "./../../../common/ddos/ddos.h"
#include "./../../../common/util/util.h"
#include "./../../../common/include/type.h"
#include "./../../../common/engine/callback.h"

using namespace util;
using namespace engine;
using namespace engine::ddos;


namespace gateway {
	class Callback: public NetworkCallback {
	public:
		Callback(void);
		~Callback(void);

		virtual void onConnect(Network network);
		virtual void onClose(Network network);

	private:
		Callback(const Callback&){}
		Callback& operator=(const Callback&){ return *this; }
	};
}

#endif