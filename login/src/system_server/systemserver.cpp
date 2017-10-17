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

            if((ConnectType)(ntype->m_type) == ConnectType::IOSERVER){
                struct LightestIpInfo info;
                info.m_port = ntype->m_port;
                strncpy(info.m_ip,ntype->m_ip,16);
                strncpy(info.m_name,ntype->m_name,32);

                GameserverHandle::initCounter(network.GetNetId());
                GameserverHandle::setGameserverInfo(network.GetNetId(),info);
            }

            Log::Print(__func__, ntype->toString());
        }
            
    }
}