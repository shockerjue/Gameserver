#include "mysqlutil.h"

namespace engine {

	namespace db {
		
		MysqlUtil::MysqlUtil(void){
			mysql_init(&this->m_mysql);
			this->m_isopen = false;
		}

		MysqlUtil::~MysqlUtil(void){
			mysql_close(&this->m_mysql);
			this->m_isopen = false;
		}

		bool MysqlUtil::InitConnection(void){
			 if (this->m_isopen){
			 	Log::Warning(__func__, "Is connected to server!");

	            return true;    
	        } 

	        if(!this->can_connect()){
	        	Log::Perror(__func__, "Can't connection databases, param error!");

	        	return false;
	        }

	        if(!mysql_real_connect( 
	        	&this->m_mysql, 
	        	this->GetHost().c_str(), 
	        	this->GetUser().c_str(), 
	        	this->GetPass().c_str(), 
	        	this->GetDbName().c_str(),
	        	this->GetPort(), nullptr, 0)){
	        	Log::Perror(__func__, "Error connection to database:" + 
	        		string(mysql_error(&this->m_mysql)));

	        	return false;
	        }

	        this->m_isopen = true;
			
			Log::Print(__func__, this->toString() + " Connect Success!");  

	        return true;
		}

		bool MysqlUtil::InitConnection(const string host,const int port,
				const string dbname,const string user,const string pass){
			if(this->m_isopen){
				Log::Warning(__func__, "Is connected to server!");

				return true;
			}

			this->SetHost(host);
			this->SetPort(port);
			this->SetDbName(dbname);
			this->SetUser(user);
			this->SetPass(pass);

			if(!this->can_connect()){
				Log::Perror(__func__, "Can't connection database ,param error!");
			}

			if(!mysql_real_connect(
				&this->m_mysql,
				this->GetHost().c_str(),
				this->GetUser().c_str(),
				this->GetPass().c_str(),
				this->GetDbName().c_str(),
				this->GetPort(),nullptr,0)){
				Log::Perror(__func__, "Error connection to database:" + 
					string(mysql_error(&this->m_mysql)));

				return false;
			}

			this->m_isopen = true;

			return true;
		}

		int MysqlUtil::Exec(const string sql){
			if(0 == sql.size()){
				return 0;
			}

			if(!this->m_isopen){
				Log::Perror(__func__, "Mysql connection isn't opened!");

				return 0;
			}

			int rows = 0;
		    int res = mysql_query(&this->m_mysql, sql.c_str());
		    if(res >= 0){
		        rows = mysql_affected_rows(&this->m_mysql);
		        if(-1 == rows){
		        	Log::Perror(__func__, "ExecSQL error :" + 
	        			string(mysql_error(&this->m_mysql)));
		        }

		        return rows;
		    } else {
		    	Log::Perror(__func__, "Update errpr :" + 
	        			string(mysql_error(&this->m_mysql)));

		        return 0;
		    }

			return rows;
		}

		vector<map<string,Value> >	MysqlUtil::Selection(const string sql){
			vector<map<string,Value> > results;
			if(0 == sql.size()){
				return results;
			}

			Log::Print(__func__ , sql);

			if(!this->m_isopen){
				Log::Perror(__func__, "Mysql connection isn't opened!");

				return results;
			}

			unsigned int i,j = 0;    

	        i = mysql_real_query(&this->m_mysql,sql.c_str(),sql.size());
	        if ( i != 0 ){ 
	        	Log::Perror(__func__, "Select Error!" + string(mysql_error(&this->m_mysql)));

	            return results;    
	        }    

	        MYSQL_RES *res;    
	        MYSQL_ROW row; 
	        MYSQL_FIELD *fd;

	        res = mysql_store_result(&this->m_mysql);
	        vector<string> fild_name;
	        for(i = 0; nullptr != (fd = mysql_fetch_field(res)); i++){
	            fild_name.push_back(string(fd->name));
	        }

	        map<string,Value> objectValue;    
	        while( (row = mysql_fetch_row(res)) ){
	            int fields =  mysql_num_fields(res);

				/**
				* 将查询到的值放入值匹配其中
				*/
	            for ( j = 0 ; j < fields ; j++ ){ 
	            	Value _value;
	            	_value.SetValue(row[j]); 
	                objectValue[fild_name[j]] = _value;    
	            }   

	            results.push_back(objectValue);  
	            objectValue.clear();
	        }    

	        mysql_free_result(res);

	        return results;
		}
	}

}