#include "mysqlmaster.h"


namespace dbrw {
	MysqlMaster::MysqlMaster(void){
		ConfigModule* config = Engine::getConfigModule();
		this->m_mysqlutil.SetHost(config->getStringByKey("dbmasterhost"));
		this->m_mysqlutil.SetPort(config->getIntByKey("dbmasterport"));
		this->m_mysqlutil.SetDbName(config->getStringByKey("dbmastername"));
		this->m_mysqlutil.SetUser(config->getStringByKey("dbmasteruser"));
		this->m_mysqlutil.SetPass(config->getStringByKey("dbmasterpass"));

		bool ret = this->m_mysqlutil.InitConnection();
		if(!ret){
			Log::Perror(__func__, this->m_mysqlutil.toString() + " Connect fail!");

			return ;
		}
	}

	MysqlMaster::~MysqlMaster(void){

	}

	bool MysqlMaster::role_is_exists(int rid){
		if(!rid) {
			return false;
		}

		char _sql[256] = {0};
		sprintf(_sql, "SELECT rid FROM player WHERE rid=%d", rid);

		vector<map<string,Value> >	rows = this->m_mysqlutil.Selection(_sql);
		if(0 == rows.size()) {
			return false ;
		}

		return true;
	}

	bool MysqlMaster::role_is_exists(string username) {
		if(0 == username.size()){
			return false;
		}

		char _sql[256] = {0};
		sprintf(_sql, "SELECT rid FROM player WHERE rolename='%s'",username.c_str());

		vector<map<string,Value> > rows = this->m_mysqlutil.Selection(_sql);
		if(0 == rows.size()) {
			return false ;
		}

		return true;
	}

	int MysqlMaster::create_role_info(inner_ipc::RoleInfo* role,bool is_cache){
		if(nullptr == role) {
			return -1;
		}

		if(this->role_is_exists(role->name())) {
			return 0;
		}

		char _sql[512] = {0};
		sprintf(_sql,"INSERT INTO player(rolename) VALUE('%s')",role->name().c_str());

		string sql = _sql;
		int ret = this->m_mysqlutil.Exec(sql);
		if(0 >= ret) {
			Log::Warning(__func__, sql);

			return -1;
		}

		role->set_rid(ret);
		
		if(is_cache) {
			struct Role _role;
			_role.rid = role->rid();
			strcpy(_role.name,role->name().c_str());

			ShareCache* share = ShareCache::getShareCache();
			int r = share->write_role_info(role->rid(),&_role);

			Log::Print(__func__, "create_role_info from name " + Util::int2str(r));
		}

		return ret;
	}

	void MysqlMaster::sync_role_info(inner_ipc::RoleInfo* role) {
		if(nullptr == role) {
			return ;
		}

		char _sql[512] = {0};
		sprintf(_sql,"UPDATE player SET rolename='%s' WHERE rid=%d",role->name().c_str(),role->rid());
		string sql = _sql;
		int ret = this->m_mysqlutil.Exec(sql);
		if(0 >= ret) {
			Log::Warning(__func__, "sync role info fail! " + sql);

			return ;
		}

		return ;
	}

	void MysqlMaster::add_friend(int rid,int arid) {
		if(!rid || !arid) {
			return ;
		}

		char _sql[256] = {0};
		sprintf(_sql,"INSERT INTO friends(rid1,rid2,createtime) VALUE(%d,%d,%ld)",rid,
			arid,Datetime::getTimestamp());

		string sql = _sql;
		int ret = this->m_mysqlutil.Exec(sql);
		if(0 >= ret) {
			Log::Warning(__func__, "Add friend fail!");

			return ;
		}

		return ;
	}

	void MysqlMaster::add_offline_msg(inner_ipc::OfflineMsgIPC chat) {
		if(0 == chat.srid() || 0 == chat.did()) {
			return ;
		} 

		char _sql[4096] ={0};
		sprintf(_sql,"INSERT INTO offline_msg(srid,did,type,msg_type,tid,size,stime,msg,sname) VALUE( \
			%d,%d,%d,%d,%d,%d,%d,'%s','%s')",chat.srid(),chat.did(),chat.type(),chat.msg_type(),chat.tid(), \
			chat.size(),chat.stime(),chat.msg().c_str(),chat.sname().c_str());

		string sql(_sql);
		int ret = this->m_mysqlutil.Exec(sql);
		if(0 >= ret) {
			Log::Warning(__func__, "add offline msg fail! " + sql);

			return ;
		}

		Log::Print(__func__,"-------------------add_offline_msg:" + sql);
	}
}