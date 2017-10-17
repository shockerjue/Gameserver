#include "systemserver.h"

void SystemServer::onServerRegister(NetId netid,const char* buf,const int size){
	Network network = NetworkManage::getNetworkManage()->getNetwork(netid);
	ConnectType type = (ConnectType)network.GetType();
	
	if (type == ConnectType::NONEDEFAULT){
		struct NetworkType* ntype = (struct NetworkType*)buf;
		if(ConnectType::MIN > ntype->m_type || ConnectType::MAX < ntype->m_type){
			;
		}else{
			NetworkManage::getNetworkManage()->updateConnectType(network,(ConnectType)(ntype->m_type));

			Log::Print(__func__ , string("Set Network Type! ") + ntype->toString());
		}
	}
}