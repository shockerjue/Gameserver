#ifndef H_NCALLBACK_H_
#define H_NCALLBACK_H_

#include "./../engine/engine.h"
#include "./../include/type.h"

using namespace std;
using namespace engine;

class NCallback: public NetworkCallback {
public:
	virtual void onRecv(Socket socket,const char* buf,const int size);
	virtual void onSend(const int size);
	virtual void onClose(Socket socket);
};

#endif