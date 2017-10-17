#include "dbhost.h"

namespace dbhost {
	void DbHost::onRequestInfo(NetId netid,const char* buf,int size){
		if(!netid || nullptr == buf || 0 > size){
			return ;
		}

		string data;
		data.assign(buf,size);
		
		inner_ipc::ResRoleInfoIPC info;
		info.ParseFromString(data);

		int rid = info.rid();
		if(!rid){
			return ;
		}

		Role* role = RoleManage::getRoleManage()->getRoleByRid(rid);
		if(nullptr == role){
			return ;
		}

		role->SetName(info.name());
		Network network = NetworkManage::getNetworkManage()->getNetwork(role->GetNetId());

		ResponseRoleInfo resinfo;
		resinfo.set_rid(role->GetRoleId());
		resinfo.set_reqrid(info.reqrid());

		string res_data;
		resinfo.SerializeToString(&res_data);

		char _res_data[res_data.length() + 1];
		memset(_res_data,0,res_data.length() + 1);
		memcpy(_res_data,res_data.c_str(),res_data.length());

		NetworkManage::getNetworkManage()->SendByNetId(role->GetNetId(),
			S_MsgType::CS_REQUEST_ROLE_INFO,_res_data,res_data.length());

		ChatHost::notify2chatserverforlogin(network);
		KeyManage::getKeyManage()->removekey(rid);

		Log::Print(__func__, info.name());
	}

	void DbHost::onRequestBag(NetId netid,const char* buf,int size) {

	}

	void DbHost::onRequestFriends(NetId netid,const char* buf,int size) {
		if(!netid || nullptr == buf || 0 > size){
			return ;
		}

		string data;
		data.assign(buf,size);

		inner_ipc::ResFriendsIPC friends;
		friends.ParseFromString(data);
		
		int rid = friends.rid();
		if(!rid) {
			return ;
		}

		Role* role = RoleManage::getRoleManage()->getRoleByRid(rid);
		if(nullptr == role) {
			return ;
		}

		ResponseFriends res_friends;
		::google::protobuf::RepeatedPtrField< ::inner_ipc::FriendItem >* friendsitems = friends.mutable_friends();
		::google::protobuf::RepeatedPtrField< ::inner_ipc::FriendItem >::iterator it = friendsitems->begin();
		while(it != friendsitems->end()) {
			gameserver_proto::FriendItem* item = res_friends.add_friends();
			item->set_rid(it->rid());
			item->set_username(it->username());

			it++;
		}

		string temp;
		res_friends.SerializeToString(&temp);

		char _temp[temp.length() + 1];
		memset(_temp,0,temp.length() + 1);
		memcpy(_temp,temp.c_str(),temp.length());

		NetworkManage::getNetworkManage()->SendByNetId(role->GetNetId(),
			S_MsgType::CS_REQUEST_FRIENDS_LIST,_temp,temp.length());
	}

	void DbHost::onRequestAddFriend(NetId netid,const char* buf,int size) {
		if(!netid || nullptr == buf || !size) {
			return ;
		}

		string data;
		data.assign(buf,size);

		inner_ipc::ResAddFriendIPC res;
		res.ParseFromString(data);

		int rid = res.rid();
		if(!rid) {
			return ;
		}

		Role* role = RoleManage::getRoleManage()->getRoleByRid(rid);
		if(nullptr == role) {
			return ;
		}

		// 响应玩家
		ResponseAddFriend response;
		response.set_status(res.status());

		inner_ipc::FriendItem _item = res.friend_();
		gameserver_proto::FriendItem* item = response.mutable_friend_();
		item->set_rid(_item.rid());
		item->set_username(_item.username());

		string temp;
		response.SerializeToString(&temp);
		char _temp[temp.length() + 1];
		memset(_temp,0,temp.length() + 1);
		memcpy(_temp,temp.c_str(),temp.length());

		NetworkManage::getNetworkManage()->SendByNetId(role->GetNetId(),
			S_MsgType::CS_REQUEST_ADD_FRIEND,_temp,temp.length());

		// 通知玩家,借助chatserver通知

		Log::Print(__func__, "Addfriend success from dbserver!");

		return ;
	}

	void DbHost::onResponseOfflineMsg(NetId netid,const char* buf,int size) {
		if(!netid || nullptr == buf || !size) {
			return ;
		}

		string data;
		data.assign(buf,size);
		
		inner_ipc::ResOfflineMsgIPC res;
		res.ParseFromString(data);

		int rid = res.rid();
		if(!rid) {
			return ;
		}

		Role* role = RoleManage::getRoleManage()->getRoleByRid(rid);
		if(nullptr == role) {
			return ;
		}

		ResponseOfflineMsgs response;
		response.set_count(res.count());

		::google::protobuf::RepeatedPtrField< ::inner_ipc::OfflineMsgItem >* msgitems = res.mutable_msgs();
		::google::protobuf::RepeatedPtrField< ::inner_ipc::OfflineMsgItem >::iterator it = msgitems->begin();
		while(it != msgitems->end()) {
			gameserver_proto::MsgItem* item = response.add_msgs();
			item->set_srid(it->srid());
			item->set_rid(it->srid());
			item->set_type(it->type());
			item->set_msg_type(it->msg_type());
			item->set_tid(it->tid());
			item->set_msg_size(it->msg_size());
			item->set_sname(it->sname());
			item->set_stime(it->stime());
			item->set_msg(it->msg());

			it++;
		}

		string temp;
		response.SerializeToString(&temp);

		char _temp[temp.length() + 1];
		memset(_temp,0,temp.length() + 1);
		memcpy(_temp,temp.c_str(),temp.length());

		NetworkManage::getNetworkManage()->SendByNetId(role->GetNetId(),
			S_MsgType::CS_ROLE_TO_GAMESERVER_CHAT_SEND_MSG,_temp,temp.length());


		Log::Print(__func__, "RequestMsg success from dbserver!" + Util::int2str(temp.length()) + " rid:" + Util::int2str(rid));
	}
}