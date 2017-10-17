#ifndef H_DDOSTYPE_H_
#define H_DDOSTYPE_H_

#include "./../util/util.h"
#include "./../include/thread.h"
#include "./../msgdefine/msgcode.h"
#include "./../engine/networkmanage.h"


using namespace util;
using namespace engine;
using namespace msghandle;
using namespace engine::thread;

namespace engine {

	namespace ddos {
		/**
		* ddos单项信息，代表了一个端的连接信息
		*
		* netid 		监听的netid
		* timeout 		超时时间
		* create_time 	创建连接的时间
		*/
		struct DDosItem {
			int 	create_time;
			int 	timeout;
			int 	netid;

			DDosItem():
				create_time(0),
				timeout(0),
				netid(0){

			}

			DDosItem(const DDosItem& item){
				this->create_time = item.create_time;
				this->timeout = item.timeout;
				this->netid = item.netid;
			}

			DDosItem& operator=(const DDosItem& item){
				if(this == &item){
					return *this;
				}

				this->create_time = item.create_time;
				this->timeout = item.timeout;
				this->netid = item.netid;

				return *this;
			}

			string toString(void){
				char buf[128] = {0};

				sprintf(buf,"{create_time: %d, timeout:%d , netid:%d}",
					this->create_time,
					this->timeout,
					this->netid);

				return string(buf);
			}
		};

		/**
		* DDOS操作的基础模型
		*/
		class DDosBase {
		public:
			virtual void check(Thread thread) = 0;
			virtual void new_msg(Message msg) = 0;
			virtual void new_msg(vector<Message> msgs) = 0;

		protected:
			map<int,struct DDosItem>        m_items;
		};
	}

}

#endif