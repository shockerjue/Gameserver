#include "rolemanage.h"

namespace role {
	unsigned  short 	RoleManage::s_counter = 0;
	RoleManage*	RoleManage::s_pRoleManage = nullptr;

	RoleManage::RoleManage(void){
		this->m_default = nullptr;

		RoleManage::s_counter = RoleManage::s_counter + 1;
	}

	RoleManage::~RoleManage(void){
		map<NetId,Role*>::iterator iter = this->m_roles.begin();
		while(iter != this->m_roles.end()) 
		{
			 delete iter->second;
			 iter->second = NULL;

			 this->m_roles.erase(iter++); 
		}

		RoleManage::s_counter = 0;
	}

	RoleManage* RoleManage::getRoleManage(void){
		if(nullptr == RoleManage::s_pRoleManage){
			RoleManage::s_pRoleManage = new RoleManage();
		}

		return RoleManage::s_pRoleManage;
	}

	unsigned short 	RoleManage::getCounter(void){
		return RoleManage::s_counter;
	}

	void RoleManage::appendRole(NetId netid,Role* role){
		if(!netid || nullptr == role){
			return ;
		}

		map<NetId,Role*>::iterator it = this->m_roles.find(netid);
		if(it != this->m_roles.end()){
			return ;
		}

		this->m_roles[netid] = role;
	}

	void RoleManage::removeRole(NetId netid){
		if(!netid){
			return ;
		}

		map<NetId,Role*>::iterator it = this->m_roles.find(netid);
		if(it == this->m_roles.end()){
			return ;
		}

		delete it->second;
		it->second = NULL;

		RoleManage::s_counter = RoleManage::s_counter - 1;
		this->m_roles.erase(it); 
	}

	Role* RoleManage::getRoleByNetId(NetId netid){
		if(!netid){
			return this->m_default;
		}

		map<NetId,Role*>::iterator it = this->m_roles.find(netid);
		if(it == this->m_roles.end()){
			return this->m_default;
		}

		return it->second;
	}

	Role* RoleManage::getRoleByRid(RoleId rid){
		if(!rid){
			return this->m_default;
		}

		map<NetId,Role*>::iterator it = this->m_roles.begin();
		while(it != this->m_roles.end()){
			if(rid == it->second->GetRoleId()){
				return it->second;
			}

			it++;
		}

		return this->m_default;
	}

	void RoleManage::queryAllRole(QueryFunc func){
		map<NetId,Role*>::iterator it = this->m_roles.begin();
		while(it != this->m_roles.end()){
			NetId netid = it->second->GetNetId();
			RoleId roleid = it->second->GetRoleId();

			func(netid,roleid);
		}

		return ;
	}
}