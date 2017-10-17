#include "dboperator.h"

namespace dbhost {
	void DbOperator::sendRequestRoleInfo(NetId netid,int rid,int reqrid) {
		if(!netid || !rid) {
			return ;
		}

		inner_ipc::ReqRoleInfoIPC req;
		req.set_rid(rid);
		req.set_reqrid(reqrid);
		req.set_requestid(netid);

		string data;
		req.SerializeToString(&data);

		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::DB,
			_MsgType_Gameserver_Db::SS_REQUEST_LOGIN_INFO,_data,data.length());
	}

	void DbOperator::sendRequestBag(NetId netid,int rid) {

	}

	void DbOperator::sendRequestFriends(NetId netid,int rid,int type) {
		if(!netid || !rid) {
			return ;
		}

		inner_ipc::ReqFriendsIPC request;
		request.set_rid(rid);
		request.set_requestid(netid);
		request.set_type(type);

		string data;
		request.SerializeToString(&data);

		char _data[data.length() + 1];
		memset(_data,0,data.length() + 1);
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::DB,
			_MsgType_Gameserver_Db::SS_REQUEST_FRIENDS_LIST,_data,data.length());
	}

	void DbOperator::sendRequestAddFriends(NetId netid,int rid,int arid) {
		if(!netid || !rid) {
			return ;
		}

		inner_ipc::ReqAddFriendIPC req;
		req.set_rid(rid);
		req.set_arid(arid);
		req.set_requestid(netid);

		string data;
		req.SerializeToString(&data);

		char _data[data.length() + 1];
		memset(_data,0,data.length());
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::DB,
			_MsgType_Gameserver_Db::SS_REQUEST_ADD_FRIENDS,_data,data.length());
	}

	void DbOperator::sendRequestOfflineMsgs(NetId netid,int rid,RequestOfflineMsgs request) {
		if(!netid || !rid) {
			return ;
		}

		inner_ipc::ReqOfflineMsgIPC req;
		req.set_rid(rid);
		req.set_type(request.type());
		req.set_rrid(request.rrid());
		req.set_count(request.count());
		req.set_requestid(netid);

		string data;
		req.SerializeToString(&data);

		char _data[data.length() + 1];
		memset(_data,0,data.length());
		memcpy(_data,data.c_str(),data.length());

		NetworkManage::getNetworkManage()->SendByNetworkType(ConnectType::DB,
			_MsgType_Gameserver_Db::SS_REQUEST_MSG,_data,data.length());
	}
}