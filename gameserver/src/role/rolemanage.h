#ifndef H_ROLEMANAGE_H_
#define H_ROLEMANAGE_H_

#include "role.h"
#include "./../../../common/engine/engine.h"

using namespace engine;

/**
* 管理玩家对象
*/
namespace role {
	typedef void (*QueryFunc)(NetId,RoleId); 

	class RoleManage {
	public:
		~RoleManage(void);

		/**
		* 通过NetId获取对应的玩家对象指针
		*
		* @param netid 	
		* @return Role*
		*/
		Role* 	getRoleByNetId(NetId netid);

		/**
		* 通过RoleId获取玩家对象指针
		* 
		* @param rid
		* @return Role*
		*/
		Role* 	getRoleByRid(RoleId rid);

		/**
		* 将玩家指针对象添加到管理器中
		* 
		* @param netid
		* @param role*
		*/
		void	 appendRole(NetId netid,Role* role);

		/**
		* 从管理其中移除Role指针对象
		* 
		* @param netid
		*/
		void	 removeRole(NetId netid);

		void	 queryAllRole(QueryFunc func);

		static RoleManage* getRoleManage(void);

		/**
		* 获取对象数
		*/
		static unsigned short 	getCounter(void);
	private:
		RoleManage(void);
		RoleManage(const RoleManage&){}
		RoleManage& operator=(const RoleManage&){ return *this; }

	private:
		map<NetId,Role*> 	m_roles;
		Role* 				m_default;

		static unsigned  short 	s_counter;
		static RoleManage*	s_pRoleManage;
	};
}

#endif 