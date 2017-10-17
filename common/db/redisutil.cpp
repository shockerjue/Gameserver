#include "redisutil.h"

namespace engine {

	namespace db {
		RedisUtil::RedisUtil(void){
			this->m_pConnect = nullptr;
			this->m_isopen = false;
		}

		RedisUtil::~RedisUtil(void){
			if(nullptr != this->m_pConnect){
				redisFree(this->m_pConnect);

				this->m_pConnect = nullptr;
			}

			this->m_isopen = false;
		}

		bool RedisUtil::InitConnection(void){
			if(nullptr != this->m_pConnect){
				Log::Warning(__func__, "Redis already create instance!");

				return true;
			}

			if (this->m_isopen){
			 	Log::Warning(__func__, "Is connected to Redis!");

	            return true;    
	        } 

	        if(!this->can_connect()){
	        	Log::Perror(__func__, "Can't connection Redis, param error!");

	        	return false;
	        }

			this->m_pConnect = redisConnect(this->GetHost().c_str(), 
				this->GetPort());

	        if(this->m_pConnect != nullptr && this->m_pConnect->err)
	        {
	            Log::Perror(__func__, "Connect Redis error! msg:" + 
	            	string(this->m_pConnect->errstr));

	            return false;
	        }

	        this->m_isopen = true;

	        return true;
		}

		bool RedisUtil::InitConnection(const string host,const int port){
			if(nullptr != this->m_pConnect){
				Log::Warning(__func__, "Redis already create instance!");

				return true;
			}

			if (this->m_isopen){
			 	Log::Warning(__func__, "Is connected to Redis!");

	            return true;    
	        } 

	        this->SetHost(host);
	        this->SetPort(port);

	        if(!this->can_connect()){
	        	Log::Perror(__func__, "Can't connection Redis, param error!");

	        	return false;
	        }

	        this->m_pConnect = redisConnect(this->GetHost().c_str(), 
				this->GetPort());

	        if(this->m_pConnect != nullptr && this->m_pConnect->err)
	        {
	            Log::Perror(__func__, "Connect Redis error! msg:" + 
	            	string(this->m_pConnect->errstr));

	            return false;
	        }

	        this->m_isopen = true;

	        return true;
		}

		Value 	RedisUtil::Get(string key){
			Value _value;
			
			if(0 == key.size()){
				return _value;
			}

			if(!this->m_isopen){
				Log::Perror(__func__, "Redis connection isn't opened!");

				return _value;
			}

			redisReply* 	_reply;	

			_reply = (redisReply*)redisCommand(this->m_pConnect, "GET %s", key.c_str());
	        string str = _reply->str;

	        freeReplyObject(_reply);
	        
	        _value.SetValue(str);

	        return _value;
		}

		void  RedisUtil::Set(string key, string value){
			if(0 == key.size() || 0 == value.size()){
				return ;
			}

			if(!this->m_isopen){
				Log::Perror(__func__, "Redis connection isn't opened!");

				return ;
			}

			redisCommand(this->m_pConnect, "SET %s %s", key.c_str(), value.c_str());
		}

		bool RedisUtil::can_connect(void){
			if(0 == this->GetHost().size() ||
				!this->GetPort()){
					
				return false;
			}

			return true;
		}
	}

}