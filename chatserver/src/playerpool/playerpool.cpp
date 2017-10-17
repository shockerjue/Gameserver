#include "playerpool.h"

namespace player {
	Item::Item(void){
		this->netid = 0;
		this->gulid = 0;
	}

	Item::Item(const Item& item){
		if(this == &item){
			return ;
		}

		this->rid = item.rid;
		this->netid = item.netid;
		this->gulid = item.gulid;
		this->uname = item.uname;

		return ;
	}

	Item&	Item::operator=(const Item& item){
		if(this == &item){
			return *this;
		}

		this->rid = item.rid;
		this->netid = item.netid;
		this->gulid = item.gulid;
		this->uname = item.uname;


		return *this;
	}

	string Item::toString(void){
		char buf[128] = {0};

		sprintf(buf, "{rid: %d, netid: %d, gulid: %d, uname: %s}",
			this->rid,
			this->netid,
			this->gulid,
			this->uname.c_str());

		return string(buf);
	}

	PlayerPool*	PlayerPool::s_pPlayerPool = nullptr;

	PlayerPool::PlayerPool(void){

	}

	PlayerPool::~PlayerPool(void){

	}

	PlayerPool* PlayerPool::getPlayerPool(void){
		if(nullptr == PlayerPool::s_pPlayerPool){
			PlayerPool::s_pPlayerPool = new PlayerPool();
		}

		return PlayerPool::s_pPlayerPool;
	}

	bool PlayerPool::isExsits(int rid){
		if(!rid){
			return false;
		}

		map<int,PItem>::iterator it = this->m_ridnet.find(rid);
		if(it == this->m_ridnet.end()){
			return false;
		}

		return true;
	}

	int PlayerPool::size(void){
		return this->m_ridnet.size();
	}

	void PlayerPool::registerPItem(int rid,PItem value){
		if(!rid){
			return ;
		}

		if(this->isExsits(rid)){
			return ;
		}

		this->m_ridnet[rid] = value;
	}

	void PlayerPool::registerOrUpPItem(int rid,PItem value){
		if(!rid){
			return ;
		}

		map<int,PItem>::iterator it = this->m_ridnet.find(rid);
		if(it != this->m_ridnet.end()){
			it->second.netid = value.netid;
			it->second.gulid = value.gulid;
			it->second.rid = value.rid;

		}else {
			this->m_ridnet[rid] = value;
		}

	}

	void PlayerPool::unregisterPItem(int rid){
		if(!rid){
			return ;
		}		

		if(!this->isExsits(rid)){
			return ;
		}

		this->m_ridnet.erase(rid);
	}

	PItem PlayerPool::getItemByRid(int rid){
		if(!rid){
			return PItem();
		}

		map<int,PItem>::iterator it = this->m_ridnet.find(rid);
		if(it == this->m_ridnet.end()){
			return PItem();
		}

		return it->second;
	}

	map<int,PItem>	PlayerPool::getAllPlayer(void){
		return this->m_ridnet;
	}

	map<int,PItem>	PlayerPool::getAllPlayerByGid(int gid){
		map<int,PItem> temp;

		if(!gid){
			return temp;
		}

		map<int,PItem>::iterator it = this->m_ridnet.begin();
		while(it != this->m_ridnet.end()){
			PItem itm = it->second;
			if(gid == itm.gulid){
				temp[itm.rid] = it->second;
			}

			it++;
		}

		return temp;
	}
}