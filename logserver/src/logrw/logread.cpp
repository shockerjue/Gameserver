#include "logread.h"

namespace logserver {
    LogRead*    LogRead::s_pLogRead = nullptr;

    LogRead::LogRead(void){
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

    LogRead* LogRead::getLogRead(void){
        if(nullptr == LogRead::s_pLogRead){
            LogRead::s_pLogRead = new LogRead();
        }

        return LogRead::s_pLogRead;
    }

    vector<map<string,Value> > LogRead::read(string sql){
        vector<map<string,Value> > temp;
        if(0 == sql.size()){
            return temp;
        }

        temp = this->m_mysqlutil.Selection(sql);

        return temp;
    }
}