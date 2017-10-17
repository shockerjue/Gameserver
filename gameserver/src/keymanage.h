/**
* 登陆缓存信息管理，主要管理从Login服务器发送过来的登陆key
*/

#ifndef H_KEYMANAGE_H_
#define H_KEYMANAGE_H_

#include <map>
#include <iostream>

#include "./../../common/util/util.h"

using namespace std;
using namespace util;

namespace gameserver {
	struct KeyItem {
		long 		createtime;
		string 		key;

		KeyItem(void):
			createtime(0),
			key(""){

		}

		KeyItem(const KeyItem& key){
			if(this == &key){
				return ;
			}

			this->createtime = key.createtime;
			this->key = key.key;
		}

		KeyItem& operator=(const KeyItem& key){
			if(this == &key){
				return *this;
			}

			this->key = key.key;
			this->createtime = key.createtime;

			return *this;
		}

		string toString(void){
			char buf[256] = {0};

			sprintf(buf, "{createtime: %ld, key: %s}",
				this->createtime,
				this->key.c_str());

			return string(buf);
		}
	};

	class KeyManage {
	public:
		~KeyManage(void);

		void appendkey(int rid,string key);
		void updateKey(int rid,string key);
		void removekey(int rid);
		void removekey(string key);

		bool isexists(int rid);
		int isexists(string key);

		bool istimeout(int rid);
		bool istimeout(string key);

		KeyItem getkey(int rid);

		static KeyManage* getKeyManage(void);
	private:
		KeyManage(void);
		KeyManage(const KeyManage&){}
		KeyManage& operator=(const KeyManage&){ return *this; }


	private:
		map<int,KeyItem>		m_keys;

		static KeyManage* s_pKeyManage;
	};
}


#endif