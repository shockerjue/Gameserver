#include "./../include/configmodule.h"

namespace engine {
	
	namespace config {
		ConfigModule* 	ConfigModule::s_pConfigModule = nullptr;

		ConfigModule::ConfigModule(void){

		}

		ConfigModule::~ConfigModule(void){
			
		}

		ConfigModule::ConfigModule(const ConfigModule& config){
			this->m_config = config.m_config;

			return ;
		}

		ConfigModule& ConfigModule::operator=(const ConfigModule& config){
			if(this == &config){
				return *this;
			}

			this->m_config = config.m_config;

			return *this;
		}

		ConfigModule* ConfigModule::getConfigModule(void){
			if(nullptr == ConfigModule::s_pConfigModule){
				ConfigModule::s_pConfigModule = new ConfigModule();
			}

			return ConfigModule::s_pConfigModule;
		}

		bool ConfigModule::onLoad(const string filename){
			if(0 == filename.size()){
				return false;
			}

			ifstream fin(filename, std::ios::in);
			if(!fin){
				Log::Perror(__func__, filename + " opem fail!");

				exit(1);
			}

			char key[512] = {0};
			char line[512] = {0};
			char value[512] = {0};
			
			this->m_config.clear();

			//	读取一行数据
			while(fin.getline(line,512)){
				const char* p = line;
				int i = 0;

				//	解析出key的部分
				while(nullptr != p && (('a' <= *p && 'z' >= *p) ||
					('A' <= *p && 'Z' >= *p) || ' ' ==  *p || 
					('0' <= *p && '9' >= *p)) && 512 > i || 
					'_' == *p){
					if(' ' == *p){
						p++;

						continue;
					}

					key[i++] = *p;
					p++;
				}

				if('=' != *p){
					continue ;
				}

				i = 0;
				p++;

				//	解析出value的部分
				while(nullptr != p && (('a' <= *p && 'z' >= *p) || 
					('A' <= *p && 'Z' >= *p) || ('0' <= *p && '9' >= *p) 
					|| '.' == *p || ' ' == *p) && 512 > i){
					if(' ' == *p){
						p++;

						continue;
					}

					value[i++] = *p;
					p++;
				}
				

				//	将值放入容器中
				Value _value;
				_value.SetValue(string(value));

				this->m_config[string(key)] = _value;
				cout<<key<<" : "<<value<<endl;
				//	设置默认值
				memset(line,0,512);
				memset(key,0,512);
				memset(value,0,512);
			}

			fin.close();
			
			return true;
		}

		int ConfigModule::getIntByKey(const string key){
			if(0 == key.size()){
				return 0;
			}

			map<string,Value>::iterator it = this->m_config.find(key);
			if(it == this->m_config.end()){
				return 0;
			}

			return it->second.toInt();
		}

		string ConfigModule::getStringByKey(const string key){
			if(0 == key.size()){
				return string("");
			}

			map<string,Value>::iterator it = this->m_config.find(key);
			if(it == this->m_config.end()){
				return string("");
			}

			return it->second.toString();
		}

		bool ConfigModule::getBoolByKey(const string key){
			if(0 == key.size()){
				return false;
			}

			map<string,Value>::iterator it = this->m_config.find(key);
			if(it == this->m_config.end()){
				return false;
			}

			return it->second.toBool();
		}

		char ConfigModule::getCharByKey(const string key){
			if(0 == key.size()){
				return 0;
			}

			map<string,Value>::iterator it = this->m_config.find(key);
			if(it == this->m_config.end()){
				return 0;
			}

			return it->second.toChar();
		}

		float ConfigModule::getFloatByKey(const string key){
			if(0 == key.size()){
				return 0.0f;
			}

			map<string,Value>::iterator it = this->m_config.find(key);
			if(it == this->m_config.end()){
				return 0.0f;
			}

			return it->second.toFloat();
		}

		Value ConfigModule::getValue(const string key){
			if(0 == key.size()){
				return Value();
			}
			
			map<string,Value>::iterator it = this->m_config.find(key);
			if(it == this->m_config.end()){
				return Value();
			}

			return it->second;
		}
	}

}