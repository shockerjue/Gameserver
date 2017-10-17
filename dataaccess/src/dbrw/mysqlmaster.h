/***********************************************************
* 对数据库进行读写操作（操作主数据库）
* author  shockerjue
************************************************************/

#ifndef H_MYSQLMASTER_H_
#define H_MYSQLMASTER_H_

#include "./../../../common/ipc/ipc.h"
#include "./../sharecache/sharecache.h"
#include "./../../../common/util/util.h"
#include "./../../../common/db/mysqlutil.h"
#include "./../../../common/engine/engine.h"

using namespace util;
using namespace engine;
using namespace sharecache;
using namespace engine::db;

namespace dbrw {
	class MysqlMaster {
	public:
		MysqlMaster(void);
		~MysqlMaster(void);

		/**
		* 创建玩家信息
		*
		* @param role 		玩家初始默认信息
		* @param is_cache	是否缓存
		*/
		int create_role_info(inner_ipc::RoleInfo* role,bool is_cacahe);
		
		/**
		* 同步角色信息到数据库中
		* 
		* @param role
		*/
		void sync_role_info(inner_ipc::RoleInfo* role);

		/**
		* 添加好友
		*
		* @param rid
		* @param arid
		*/
		void add_friend(int rid,int arid);

		/**
		* 保存离线消息
		*
		* @param chat
		*/
		void add_offline_msg(inner_ipc::OfflineMsgIPC chat);

	private:
		bool role_is_exists(int rid);
		bool role_is_exists(string username);

	private:
		MysqlUtil 	m_mysqlutil;
	};
}

#endif