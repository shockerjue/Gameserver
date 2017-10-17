#include "mysqlslave.h"

namespace dbrw {
	MysqlSlave* 	MysqlSlave::s_pDbrw = nullptr;

	MysqlSlave::MysqlSlave(void){
		ConfigModule* config = Engine::getConfigModule();
		this->m_mysqlutil.SetHost(config->getStringByKey("dbslavehost"));
		this->m_mysqlutil.SetPort(config->getIntByKey("dbslaveport"));
		this->m_mysqlutil.SetDbName(config->getStringByKey("dbslavename"));
		this->m_mysqlutil.SetUser(config->getStringByKey("dbslaveuser"));
		this->m_mysqlutil.SetPass(config->getStringByKey("dbslavepass"));

		bool ret = this->m_mysqlutil.InitConnection();
		if(!ret){
			Log::Perror(__func__, this->m_mysqlutil.toString() + " Connect fail!");

			return ;
		}
	}

	MysqlSlave::~MysqlSlave(void){

	}

	MysqlSlave* 	MysqlSlave::getMysqlSlave(void){
		if(nullptr == MysqlSlave::s_pDbrw){
			MysqlSlave::s_pDbrw = new MysqlSlave();
		}

		return MysqlSlave::s_pDbrw;
	}

	bool MysqlSlave::hasRole(int rid){
		if(!rid){
			Log::Warning(__func__, "Rid is empty!");

			return false;
		}

		string sql("SELECT rid FROM player");
		sql = sql + " WHERE rid=" + Util::int2str(rid);

		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql);

		vector<map<string,Value> >::iterator it = rows.begin();
		if(it == rows.end()){
			return false;
		}

		return true;
	}

	bool MysqlSlave::hasRole(string username){
		if(0 == username.size()){
			Log::Warning(__func__, "Username is empty!");

			return false;
		}

		string sql("SELECT rid FROM player");
		sql = sql + " WHERE rolename='" + username + "'";

		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql);

		vector<map<string,Value> >::iterator it = rows.begin();
		if(it == rows.end()){
			return false;
		}

		return true;
	}

	bool MysqlSlave::isFriend(int rid,int arid) {
		if(!rid || !arid) {
			return false;
		}

		char temp[256] = {0};
		sprintf(temp,"SELECT id FROM friends WHERE (rid1=%d AND rid2=%d) or (rid1=%d AND rid2=%d)",
			rid,arid,arid,rid);

		string sql(temp);
		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql);
		
		vector<map<string,Value> >::iterator it = rows.begin();
		if(it == rows.end()){
			return false;
		}

		return true;
	}

	bool MysqlSlave::getLoginInfo(string table,string column_name,
		Value value,inner_ipc::ResponseLoginIPC* login){
		if(0 == table.size() || nullptr == login ){
			Log::Warning(__func__, "Query table name is empty!");

			return false;
		}

		if(0 == column_name.size()){
			Log::Warning(__func__, "Query column_name is empty!");

			return false;
		}

		if(value.isEmpty()){
			Log::Warning(__func__, " Query pattern value is empty!");

			return false;
		}

		string sql("SELECT * FROM ");
		sql = sql + table + " WHERE " + column_name + "='" + value.toString() + "'";

		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql);

		vector<map<string,Value> >::iterator it = rows.begin();
		if(it == rows.end()){
			return false;
		}

		login->set_rid((*it)["rid"].toInt());
		login->set_username(value.toString());

		return true;
	}

	bool MysqlSlave::getLoginInfo(string table,string column_name,
			int value,inner_ipc::RoleInfo* role) {
		if(0 == table.size() || nullptr == role ){
			Log::Warning(__func__, "Query table name is empty!");

			return false;
		}

		if(0 == column_name.size()){
			Log::Warning(__func__, "Query column_name is empty!");

			return false;
		}

		string sql("SELECT * FROM ");
		sql = sql + table + " WHERE " + column_name + "=" + Util::int2str(value) + "";

		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql);

		vector<map<string,Value> >::iterator it = rows.begin();
		if(it == rows.end()){
			return false;
		}

		role->set_rid((*it)["rid"].toInt());
		role->set_name((*it)["rolename"].toString());

		return true;
	}


	bool MysqlSlave::read_role_info(int rid,inner_ipc::RoleInfo* role, bool is_cache) {
		if(!rid || nullptr == role) {
			return false ;
		}

		string sql("SELECT rid,rolename FROM player WHERE rid=");
		sql = sql +  Util::int2str(rid);

		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql);

		vector<map<string,Value> >::iterator it = rows.begin();
		if(it == rows.end()) {
			return false ;
		}

		role->set_rid((*it)["rid"].toInt());
		role->set_name((*it)["rolename"].toString());

		if(is_cache) {
			// TO DO write to cache

			struct Role role_;
			role_.rid = rid;
			strcpy(role_.name,role->name().c_str());

			ShareCache* share = ShareCache::getShareCache();
			int r = share->write_role_info(rid,&role_);

			Log::Print(__func__, "read_role_info from rid " + Util::int2str(r));
		}

		return true ;
	}

	bool MysqlSlave::read_role_info(string name,inner_ipc::RoleInfo* role,bool is_cache) {
		if(0 == name.size() || nullptr == role) {
			return false ;
		}

		string sql("SELECT rid,rolename FROM player WHERE rolename='");
		sql = sql + name + "'" ;

		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql);

		vector<map<string,Value> >::iterator it = rows.begin();
		if(it == rows.end()) {
			return false ;
		}

		role->set_rid((*it)["rid"].toInt());
		role->set_name((*it)["rolename"].toString());

		if(is_cache) {
			// TO DO write to cache

			struct Role role_;
			role_.rid = role->rid();
			strcpy(role_.name,role->name().c_str());

			ShareCache* share = ShareCache::getShareCache();
			int r = share->write_role_info(role->rid(),&role_);

			Log::Print(__func__, "read_role_info from name " + Util::int2str(r));
		}

		return true ;
	}

	bool MysqlSlave::read_role_friends(int rid,inner_ipc::ResFriendsIPC* friends,bool is_cache) {
		if(!rid || nullptr == friends) {
			return false ;
		}

		char temp[256] = {0};
		sprintf(temp,"SELECT id,rid1,rid2,createtime FROM friends WHERE rid1=%d or rid2=%d",rid,rid);

		string sql(temp);
		Log::Print(__func__,sql);
		
		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql);
		vector<map<string,Value> >::iterator it = rows.begin();
		if(it == rows.end()) {
			return false;
		}

		int count = 0;
		string sql_infos("SELECT rid,rolename FROM player WHERE rid IN(");
		while(it != rows.end()) {
			if(count > 0) {
				sql_infos = sql_infos + ",";
			}
			if(rid != (*it)["rid1"].toInt()) {
				sql_infos = sql_infos + Util::int2str((*it)["rid1"].toInt());
			}else {
				sql_infos = sql_infos + Util::int2str((*it)["rid2"].toInt());
			}

			it++;
			count++;
		}

		sql_infos = sql_infos + ")";
		
		if(count > 0) {
			vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql_infos);
			vector<map<string,Value> >::iterator it = rows.begin();
			if(it == rows.end()) {
				return false;
			}

			while(it != rows.end()) {
				inner_ipc::FriendItem* item = friends->add_friends();
				item->set_rid((*it)["rid"].toInt());
				item->set_username((*it)["rolename"].toString());

				it++;
			}
		}

		return true;
	}

	bool MysqlSlave::read_role_msg(int rid,int srid,inner_ipc::ResOfflineMsgIPC* msgs,bool is_cache) {
		if(!rid || nullptr == msgs) {
			return false;
		}

		char c_sql[256] = {0};
		sprintf(c_sql,"SELECT `id`,srid,did,`type`,msg_type,tid,size,stime,msg,sname FROM offline_msg \
			WHERE srid=%d AND did=%d",srid,rid);

		string sql = c_sql;
		Log::Print(__func__,sql);

		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(sql);
		vector<map<string,Value> >::iterator it = rows.begin();
		if(it == rows.end()) {
			return false;
		}

		int len = 0;
		while(it != rows.end()) {
			inner_ipc::OfflineMsgItem* item = msgs->add_msgs();
			item->set_srid((*it)["srid"].toInt());
			item->set_did((*it)["did"].toInt());
			item->set_type((*it)["type"].toInt());
			item->set_msg_type((*it)["msg_type"].toInt());
			item->set_tid((*it)["tid"].toInt());
			item->set_msg_size((*it)["size"].toInt());
			item->set_stime((*it)["stime"].toInt());
			item->set_msg((*it)["msg"].toString());
			item->set_sname((*it)["sname"].toString());

			it++;
			len++;
		}

		msgs->set_count(len);

		if(is_cache) {
			;
		}

		return true;
	}
}