#include "datacallback.h"

namespace login {
    DataCallback::DataCallback(void){

    }

    DataCallback::~DataCallback(void){

    }

    void DataCallback::onClose(Network network){
        Log::Print(__func__, "onClose-----");

        if(ConnectType::IOSERVER == network.GetType()){
            GameserverHandle::removeCounter(network.GetNetId());
        }
    }
}