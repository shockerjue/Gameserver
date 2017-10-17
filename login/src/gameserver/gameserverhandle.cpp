#include "gameserverhandle.h"

namespace loginhost {
	map<NetId,int> GameserverHandle::s_counter;
	map<NetId,struct LightestIpInfo> GameserverHandle::s_ipinfo;

	void GameserverHandle::onHasIOServer(NetId netid,const char* buf,int size){
		if(nullptr == buf || !netid){
			return ;
		}

		string data;
		data.assign(buf,size);

		inner_ipc::IoServerLoginIPC to_login;
		to_login.ParseFromString(data);

		switch(to_login.op()){
			case 0:{
				GameserverHandle::subCounter(netid);
			}break;
			case 1:{
				GameserverHandle::addCounter(netid);
			}break;
			default:break;
		}

		Log::Print(__func__,GameserverHandle::getGameserverInfo(netid).toString());
		Log::Print(__func__,GameserverHandle::getCounter(netid));
	}

	void GameserverHandle::onGetOnlineInfo(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		int total_count = 0;
		map<NetId,int>::iterator it = GameserverHandle::s_counter.begin();
		while(it != GameserverHandle::s_counter.end()){
			struct LightestIpInfo info = GameserverHandle::s_ipinfo.find(it->first)->second;

			total_count = total_count + it->second;
			
			it++;
		}
	}

	void GameserverHandle::notifyToIoServer(NetId netid,const char* buf,int size,char key[]){
		if(nullptr == buf || !netid){
			return ;
		}

		string to_data;
		to_data.assign(buf,size);
		
		inner_ipc::ResponseLoginIPC db;
		db.ParseFromString(to_data);

		if(db.status() == 400){
			return ;
		}

		inner_ipc::LoginIoServerIPC io;
		io.set_rid(db.rid());
		io.set_key(string(key));

		string data;
		io.SerializeToString(&data);
		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetId(netid,
			_MsgType_Login_Gameserver::SS_LOGIN_NOTI_GAMESERVER,
			_data,data.length());

		Log::Print(__func__, io.key());
	}

	void GameserverHandle::initCounter(NetId netid){
		if(!netid){
			return ;
		}

		GameserverHandle::s_counter[netid] = 0;
	}

	void GameserverHandle::addCounter(NetId netid){
		if(!netid){
			return ;
		}

		map<NetId,int>::iterator it = GameserverHandle::s_counter.find(netid);
		if(it == GameserverHandle::s_counter.end()){
			GameserverHandle::s_counter[netid] = 1;

			return ;
		}

		it->second = it->second + 1;
	}

	int  GameserverHandle::getCounter(NetId netid){
		if(!netid){
			return -1;
		}

		map<NetId,int>::iterator it = GameserverHandle::s_counter.find(netid);
		if(it == GameserverHandle::s_counter.end()){
			return -1;
		}

		return it->second;
	}

	void GameserverHandle::subCounter(NetId netid){
		if(!netid){
			return ;
		}

		map<NetId,int>::iterator it = GameserverHandle::s_counter.find(netid);
		if(it == GameserverHandle::s_counter.end()){
			return ;
		}

		if(!it->second){
			return ;
		}

		it->second = it->second - 1;
	}

	void GameserverHandle::removeCounter(NetId netid){
		if(!netid){
			return ;
		}

		GameserverHandle::s_counter.erase(netid);
	}

	void GameserverHandle::setGameserverInfo(NetId netid,struct LightestIpInfo info){
		if(!netid){
			return ;
		}

		GameserverHandle::s_ipinfo[netid] = info;
	}

	struct LightestIpInfo GameserverHandle::getGameserverInfo(NetId netid){
		if(!netid){
			return LightestIpInfo();
		}

		map<NetId,struct LightestIpInfo>::iterator it = GameserverHandle::s_ipinfo.find(netid);
		if(it == GameserverHandle::s_ipinfo.end()){
			return LightestIpInfo();
		}

		return it->second;
	}

	NetId GameserverHandle::getLightest(void){
		NetId netid = 0;
		int counter = 9999;

		map<NetId,int>::iterator it = GameserverHandle::s_counter.begin();
		while(it != GameserverHandle::s_counter.end()){
			if(it->second < counter){
				netid = it->first;
				counter = it->second;
			}
			

			it++;
		}

		if(0 == netid){
			netid = NetworkManage::getNetworkManage()->getNetworkByType(ConnectType::IOSERVER).GetNetId();
			
			GameserverHandle::s_counter[netid] = 1;
		}

		return netid;
	}

	struct LightestIpInfo GameserverHandle::getLightestByIpAPort(const char* ip,const int port){
		struct LightestIpInfo lighInfo;
		lighInfo.m_port = 0;

		if(nullptr == ip || !port){
			return lighInfo;
		}

		string ip_str(ip);
		map<NetId,struct LightestIpInfo>::iterator it = GameserverHandle::s_ipinfo.begin();
		while(it != GameserverHandle::s_ipinfo.end()){
			if(0 == ip_str.compare(string(it->second.m_ip))){
				if(it->second.m_port == port){
					return it->second;
				}
			}

			it++;
		}

		return lighInfo;
	}

	NetId GameserverHandle::getNetIdByIpAPort(const char* ip,const int port){
		if(nullptr == ip || !port){
			return 0;
		}

		string ip_str(ip);
		map<NetId,struct LightestIpInfo>::iterator it = GameserverHandle::s_ipinfo.begin();
		while(it != GameserverHandle::s_ipinfo.end()){
			if(0 == ip_str.compare(string(it->second.m_ip))){
				if(it->second.m_port == port){
					return it->first;
				}
			}

			it++;
		}

		return 0;
	}

}