#include "dbmodule.h"

namespace engine {
	
	namespace db {
		void DBModule::SetHost(const string host){
			this->m_host = host;
		}

		void DBModule::SetPort(const int port){
			this->m_port = port;
		}

		void DBModule::SetDbName(const string dbname){
			this->m_dbname = dbname;
		}

		void DBModule::SetUser(const string user){
			this->m_user = user;
		}

		void DBModule::SetPass(const string pass){
			this->m_pass = pass;
		}

		string	DBModule::GetHost(void){
			return this->m_host;
		}

		int 	DBModule::GetPort(void){
			return this->m_port;
		}

		string 	DBModule::GetDbName(void){
			return this->m_dbname;
		}

		string	DBModule::GetUser(void){
			return this->m_user;
		}

		string	DBModule::GetPass(void){
			return this->m_pass;
		}

		bool DBModule::can_connect(void){
			if(0 == this->m_host.size() ||
				 !this->m_port || 
				 0 == this->m_dbname.size() ||
				 0 == this->m_user.size() ||
				 0 == this->m_pass.size()){
				return false;
			}

			return true;
		}

		string	DBModule::toString(void){
			char	buf[512] = {0};

			sprintf(buf, "\n{\n\thost: %s, \n\tport: %d, \n\tdbname:%s, \n\tuser: %s, \n\tpass: %s\n}",
				this->m_host.c_str(),
				this->m_port,
				this->m_dbname.c_str(),
				this->m_user.c_str(),
				this->m_pass.c_str());

			return string(buf);
		}
	}

}