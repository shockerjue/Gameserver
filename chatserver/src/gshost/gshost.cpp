#include "gshost.h"

namespace gshost {
	void GsHost::onPlayerOnline(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);
		inner_ipc::PlayerOnlineIPC online;
		online.ParseFromString(data);

		if(!online.rid()){
			return ;
		}

		PItem value;
		value.rid = online.rid();
		value.netid = netid;
		value.gulid = online.gulid();
		value.uname = online.uname();

		PlayerPool::getPlayerPool()->registerPItem(online.rid(),value);

		Log::Print(__func__,value.toString());

		return ;
	}

	void GsHost::onPlayerOffline(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);
		inner_ipc::PlayerOfflineIPC offline;
		offline.ParseFromString(data);

		if(!offline.rid()){
			return ;
		}

		PlayerPool::getPlayerPool()->unregisterPItem(offline.rid());

		Log::Print(__func__,offline.rid());

		return ;
	}

	void GsHost::onSendToRole(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);
		inner_ipc::ChatMsgIPC chat;
		chat.ParseFromString(data);

		GsHost::sendToGameserver(chat,_MsgType_Gameserver_Chatserver::SS_GS_SEND_ROLE_MSG_TO_CS);

		Log::Print(__func__,chat.msg());

		return ;
	}

	void GsHost::onSendToGuild(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);
		inner_ipc::ChatMsgIPC chat;
		chat.ParseFromString(data);

		GsHost::sendToGameserver(chat,_MsgType_Gameserver_Chatserver::SS_GS_SEND_GUILD_MSG_TO_CS);

		return ;
	}

	void GsHost::onSendToWorld(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf){
			return ;
		}

		string data;
		data.assign(buf,size);
		inner_ipc::ChatMsgIPC chat;
		chat.ParseFromString(data);

		GsHost::sendToGameserver(chat,_MsgType_Gameserver_Chatserver::SS_GS_SEND_WORLD_MSG_TO_CS);

		return ;
	}

	void GsHost::sendToGameserver(inner_ipc::ChatMsgIPC chat,HEADER header){
		if(0 >= chat.size()){
			return ;
		}

		int srid = chat.srid();
		int did = chat.did();
		engine::ChatType type = (engine::ChatType)chat.type();

		bool is = PlayerPool::getPlayerPool()->isExsits(srid);
		if(!is){	
			Log::Print(__func__,"Role isn't exsits!");

			return ;
		}

		PItem pitem = PlayerPool::getPlayerPool()->getItemByRid(chat.srid());
		if(0 >= chat.size()){
			return ;
		}

		inner_ipc::ChatMsgIPC ssmsg;
		ssmsg.set_srid(chat.srid());
		ssmsg.set_did(did);
		ssmsg.set_tid(0);
		ssmsg.set_status(1);
		ssmsg.set_type((int)type);
		ssmsg.set_size(chat.size());
		ssmsg.set_stime(chat.stime());
		ssmsg.set_msg_type(chat.msg_type());
		ssmsg.set_msg(chat.msg());
		ssmsg.set_sname(string(pitem.uname));

		// 处理目标玩家是否在线
		is = PlayerPool::getPlayerPool()->isExsits(did);
		if(!is) {
			chat.set_sname(ssmsg.sname());
			DbHost::saveOfflineMsgIPC(chat);

			Log::Print(__func__,"DSTRole isn't exsits!");

			return ;
		}

		switch(chat.type()){
			case engine::ChatType::PEER2PEER:{
				string data;
				ssmsg.SerializeToString(&data);
				char _data[data.length() + 1];
				memset(_data,0,data.length() + 1);
				ChatHandle::send2Player(chat.did(),header,data.c_str(),data.length());
			}break;
			case engine::ChatType::PEER2GUILD:{
				ChatHandle::send2Guild(chat.did(),header,ssmsg);
			}break;
			case engine::ChatType::PEER2WORLD:{
				ChatHandle::send2World(header,ssmsg);
			}break;
			default:break;
		}

		Log::Print(__func__,ssmsg.msg());

		return ;
	}
}