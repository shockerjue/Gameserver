#include "keymanage.h"

namespace gameserver {
	KeyManage* KeyManage::s_pKeyManage = nullptr;

	KeyManage::KeyManage(void){

	}

	KeyManage::~KeyManage(void){
		this->m_keys.clear();
	}

	KeyManage*	KeyManage::getKeyManage(void){
		if(nullptr == KeyManage::s_pKeyManage){
			KeyManage::s_pKeyManage = new KeyManage();
		}

		return KeyManage::s_pKeyManage;
	}

	void KeyManage::appendkey(int rid,string key){
		if(!rid || 0 == key.size()){
			return ;
		}

		if(this->isexists(rid)){
			this->m_keys.erase(rid);
		}

		KeyItem kitem;
		kitem.key = key;
		kitem.createtime = Datetime::getTimestamp();

		this->m_keys[rid] = kitem;

		Log::Print(__func__,kitem.toString());
		
		return ;
	}

	void KeyManage::updateKey(int rid,string key){
		if(!rid || 0 == key.size()){
			return ;
		}

		map<int,KeyItem>::iterator it = this->m_keys.find(rid);
		if(it == this->m_keys.end()){
			return ;
		}

		it->second.key = key;
		it->second.createtime = Datetime::getTimestamp();

		return ;
	}

	bool KeyManage::isexists(int rid){
		if(!rid){
			return false;
		}

		map<int,KeyItem>::iterator it = this->m_keys.find(rid);
		if(it == this->m_keys.end()){
			return false;
		}

		return true;
	}

	int KeyManage::isexists(string key){
		if(0 == key.size()){
			return 0;
		}

		map<int,KeyItem>::iterator it = this->m_keys.begin();
		while(it != this->m_keys.end()){
			if(0 == key.compare(it->second.key)){
				return it->first;
			}

			it++;
		}

		return 0;
	}

	bool KeyManage::istimeout(int rid){
		if(!rid){
			return true;
		}

		map<int,KeyItem>::iterator it = this->m_keys.find(rid);
		if(it == this->m_keys.end()){
			return true;
		}

		long times = it->second.createtime;
		long curtimes = Datetime::getTimestamp();
		if((times + 300) < curtimes){
			return true;
		}

		return false;
	}

	bool KeyManage::istimeout(string key){
		if(0 == key.size()){
			return true;
		}

		map<int,KeyItem>::iterator it = this->m_keys.begin();
		while(it != this->m_keys.end()){
			if(0 == key.compare(it->second.key)){
				break;
			}

			it++;
		}

		long times = it->second.createtime;
		long curtimes = Datetime::getTimestamp();
		if((times + 300) < curtimes){
			return true;
		}

		return false;
	}

	void KeyManage::removekey(int rid){
		if(!rid){
			return ;
		}

		if(this->isexists(rid)){
			this->m_keys.erase(rid);
		}

		return ;
	}

	void KeyManage::removekey(string key){
		if(0 == key.size()){
			return ;
		}

		int rid = this->isexists(key);
		if(!rid){
			return ;
		}

		this->m_keys.erase(rid);

		return ;
	}

	KeyItem KeyManage::getkey(int rid){
		if(!rid){
			return KeyItem();
		}

		map<int,KeyItem>::iterator it = this->m_keys.find(rid);
		if(it == this->m_keys.end()){
			return KeyItem();
		}

		return it->second;
	}
}