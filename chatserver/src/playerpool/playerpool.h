/**
* 存储已经连接登录的关于聊天通信的玩家信息
*/

#ifndef H_PLAYERPOOL_H_
#define H_PLAYERPOOL_H_

#include <map>
#include <string>
#include <iostream>
#include "./../../../common/engine/engine.h"

using namespace std;
using namespace engine;

namespace player{
	/**
	* 对应玩家的信息
	*/
	struct Item {
		int 	rid;		//	玩家rid
		NetId	netid;		//	玩家对应与gameserver连接的网络id
		int 	gulid;		//	玩家社团id
		string 	uname;		//	玩家名

		Item();
		Item(const Item& item);
		Item& operator=(const Item& item);

		string toString(void);
	};

	typedef struct Item 	PItem;

	class PlayerPool {
	public:
		~PlayerPool(void);

		/**
		* 检验rid对应的玩家信息是否已经存在
		*/
		bool isExsits(int rid);

		int size(void);

		/**
		* 从管理池中获取rid相关的通信信息
		*
		* @param rid
		* @return PItem
		*/
		PItem getItemByRid(int rid);

		/**
		* 从管理池中移除玩家信息
		*
		* @param rid
		*/
		void unregisterPItem(int rid);

		/**
		* 获取管理池中所有的玩家信息，主要用于广播数据或者是发送世界公告
		*
		* @return map<int,PItem>
		*/
		map<int,PItem>	getAllPlayer(void);

		/**
		* 获取所有gid相同的玩家信息
		* 
		* @param gid 	公会id/..
		* @return map<int,PItem>
		*/
		map<int,PItem>	getAllPlayerByGid(int gid);

		/**
		* 将玩家信息注册到管理池中
		*
		* @param rid
		* @param value
		*/
		void registerPItem(int rid,PItem value);

		/**
		* 更新或者添加玩家信息到管理池中
		* 
		* @param rid
		* @param value
		*/
		void registerOrUpPItem(int rid,PItem value);

		static PlayerPool*	getPlayerPool(void);
	private:
		PlayerPool(void);
		PlayerPool(const PlayerPool&){}
		PlayerPool& operator=(const PlayerPool&){ return *this; }

	private:
		map<int,PItem>	m_ridnet;

		static PlayerPool*	s_pPlayerPool;
	};
}

#endif