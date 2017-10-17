#include "logwrite.h"

namespace logserver {
    LogWrite* LogWrite::s_pLogWrite = nullptr;

    LogWrite::LogWrite(void){
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

    LogWrite::~LogWrite(void){

    }

    LogWrite* LogWrite::getLogWrite(void){
        if(nullptr == LogWrite::s_pLogWrite){
            LogWrite::s_pLogWrite = new LogWrite();
        }

        return LogWrite::s_pLogWrite;
    }

     int LogWrite::write(int level,string proc,string func,string msg,string date){
        if(0 == proc.size() || 0 == func.size() ||
            0 == msg.size()){
            return -1;
        }
        
        char _sql[1024] = {0};
        sprintf(_sql,"INSERT INTO log(`level`,procname,func,`datetime`,msg) VALUE(%d,'%s','%s','%s','%s')",
            level,proc.c_str(),func.c_str(),date.c_str(),msg.c_str());
        
        string sql = _sql;

        int ret = this->m_mysqlutil.Exec(sql);

        return ret;
     }
}