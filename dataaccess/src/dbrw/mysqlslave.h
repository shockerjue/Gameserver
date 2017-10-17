/***********************************************************
* 对数据库进行读操作（操作从数据库）
* author  shockerjue
************************************************************/

#ifndef H_DBRW_H_
#define H_DBRW_H_

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
	class MysqlSlave {
	public:
		MysqlSlave(void);
		~MysqlSlave(void);

		/**
		* 检查对应的rid是否存在
		*
		* @param rid
		*/
		bool hasRole(int rid);

		/**
		* 检查对应用户名是否已经存在
		*
		* @param username 			用户名
		*/
		bool hasRole(string username);

		/**
		* 检查两个玩家是否是好友关系
		*
		* @param rid
		* @param arid
		*/
		bool isFriend(int rid,int arid);

		/**
		* 获取玩家登陆信息
		*
		* @param 	table 		查询表
		* @param 	column_name	查询匹配字段
		* @param 	value 		查询匹配值
		*/
		bool getLoginInfo(string table,string column_name,
			Value value,inner_ipc::ResponseLoginIPC* login);

		/**
		* 获取玩家登陆信息
		*
		* @param table
		* @param column_name
		* @param value
		* @param *role
		*/
		bool getLoginInfo(string table,string column_name,
			int value,inner_ipc::RoleInfo* role);

		/**
		* 读取角色信息，并根据参数确定是否需要写入到缓存中
		*
		* @param rid		玩家rid
		* @param role		玩家信息
		* @param is_cache	是否缓存
		*/
		bool read_role_info(int rid,inner_ipc::RoleInfo* role, bool is_cache);
		bool read_role_info(string name,inner_ipc::RoleInfo* role,bool is_cache);

		/**
		* 读取角色好友信息
		* 
		* @param rid
		* @param friends	存储好友的信息
		* @param is_cache
		*/
		bool read_role_friends(int rid,inner_ipc::ResFriendsIPC* friends,bool is_cache);

		/**
		* 读取角色消息
		*
		* @param rid
		* @param srid
		* @param msgs
		* @param is_cache
		*/
		bool read_role_msg(int rid,int srid,inner_ipc::ResOfflineMsgIPC* msgs,bool is_cache);

		static MysqlSlave* 	getMysqlSlave(void);
	private:
		MysqlSlave(const MysqlSlave& ){ /* To Do*/ }
		MysqlSlave& operator=(const MysqlSlave& ){ return *this; }

	private:
		MysqlUtil 	m_mysqlutil;

		static MysqlSlave*	s_pDbrw;
	};
}

#endif