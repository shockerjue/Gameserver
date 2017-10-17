#include "chathandle.h"

namespace chatserver {
	void ChatHandle::send2World(int msgheader,inner_ipc::ChatMsgIPC chat){
		if(0 >= chat.srid()){
			return ;
		}

		map<int,PItem> worlds = PlayerPool::getPlayerPool()->getAllPlayer();
		map<int,PItem>::iterator it = worlds.begin();
		while(it != worlds.end()){
			PItem item = it->second;

			chat.set_did(item.rid);
			string data;
			chat.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length() + 1);
			memcpy(_data,data.c_str(),data.length());
			
			ChatHandle::send2(item.netid,msgheader,_data,data.length());

			it++;
		}

		return ;
	}

	void ChatHandle::send2Guild(int gid,int msgheader,inner_ipc::ChatMsgIPC chat){
		if(!gid || 0 >= chat.srid()){
			return ;
		}

		map<int,PItem> guilds = PlayerPool::getPlayerPool()->getAllPlayerByGid(gid);
		map<int,PItem>::iterator it = guilds.begin();
		while(it != guilds.end()){
			PItem item = it->second;

			chat.set_did(item.rid);
			string data;
			chat.SerializeToString(&data);
			char _data[data.length() + 1];
			memset(_data,0,data.length() + 1);
			memcpy(_data,data.c_str(),data.length());
			
			ChatHandle::send2(item.netid,msgheader,_data,data.length());

			it++;
		}

		return ;
	}

	void ChatHandle::send2Player(int rid,int msgheader,const char* buf,int size){
		if(!rid || nullptr == buf){
			return ;
		}

		PItem item = PlayerPool::getPlayerPool()->getItemByRid(rid);
		ChatHandle::send2(item.netid,msgheader,buf,size);

		Log::Print(__func__,item.toString());

		return ;
	}

	void ChatHandle::send2(NetId netid,int msgheader,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		NetworkManage::getNetworkManage()->SendByNetId(netid,
			msgheader,buf,size);

		return ;
	}
}