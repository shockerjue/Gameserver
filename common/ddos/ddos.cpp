#include "ddos.h"


namespace engine {

	namespace ddos {
		
		DDos::DDos(void){

		}

		DDos::~DDos(void){
			this->m_items.clear();
		}

		void DDos::new_msg(Message message){
			if(0 == message.getsize()){
				return ;
			}

			struct DDosItem item;
			message.getobject(&item,sizeof(struct DDosItem));
			if(0 >= item.netid){
				return ;
			}

			if(message.getwhat() == S_WhatType::W_NOTI_NETID_NORMAL || 
				message.getwhat() == S_WhatType::W_NOTI_NETID_CLOSED){

				if(0 < item.netid){
					this->m_items.erase(item.netid);
				}
			}

			if(message.getwhat() == S_WhatType::W_NOTI_NETID_INSERT){
				map<int,struct DDosItem>::iterator it = this->m_items.find(item.netid);
				if(it != this->m_items.end()){
					this->m_items.erase(item.netid);
				}

				this->m_items[item.netid] = item;
			}

			Log::Print(__func__,item.toString() + message.toString());

			return ;
		}

		void DDos::new_msg(vector<Message> msgs){
			vector<Message>::iterator it = msgs.begin();

			while(it != msgs.end()){
				Message msg = *it;
				this->new_msg(msg);

				it++;
			}

			return ;
		}

		void DDos::check(Thread thread){
			int current_time = Datetime::getTimestamp();

			map<int,struct DDosItem>::iterator it = this->m_items.begin();
			while(it != this->m_items.end()){
				struct DDosItem item = it->second;
				int interval = current_time - item.create_time;

				if(interval > item.timeout){
					Message message;
					message.setwhat(S_WhatType::W_NOTI_NETID_TIMEOUT);
					message.setsize(sizeof(item.netid));
					message.setobject(&item.netid,sizeof(item.netid));

					thread.send_message_to_main(message);

					it = this->m_items.erase(it);

					continue ;
				}

				it++;
			} 

			return ;
		}

		void DDos::on_noti_timeout(Message message){
			if(0 == message.getsize()){
				return ;
			}

			int netid = 0;
			message.getobject(&netid,sizeof(int));
			if(!netid){
				return ;
			}

			NetworkManage* pNetworkManage = NetworkManage::getNetworkManage();
			pNetworkManage->removeNetwork(netid);

			Log::Print(__func__,"Timeout msg to !");
		}

		void DDos::noti_ddos_thread(Thread thread,const int netid,
			int timeout, S_WhatType type){
			if(!netid){
				return ;
			}

			if(0 >= timeout){
				timeout = 1;
			}

			struct DDosItem item;
			item.create_time = Datetime::getTimestamp();
			item.timeout = timeout;
			item.netid = netid;

			Message message;
			message.setwhat((int)type);
			message.setsize(sizeof(struct DDosItem));
			message.setobject(&item,sizeof(struct DDosItem));

			thread.send_message_to_thread(message);
		}

		void DDos::noti_ddos_thread(const string name,const int netid,
			int timeout,S_WhatType type){
			if(!netid || 0 == name.size()){
				return ;
			}

			if(0 >= timeout){
				timeout = 1;
			}

			struct DDosItem item;
			item.create_time = Datetime::getTimestamp();
			item.timeout = timeout;
			item.netid = netid;

			Message message;
			message.setwhat((int)type);
			message.setsize(sizeof(struct DDosItem));
			message.setobject(&item,sizeof(struct DDosItem));

			Thread::ssend_message_to_thread(name,message);
		}

	}

}