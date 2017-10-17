#include "role.h"

namespace role {
	Role::Role(void):
	m_rid(0),
	m_netid(0){

	}

	Role::~Role(void){
	}

	Role::Role(const Role& role){
		if(this == &role){
			return ;
		}

		this->m_rid = role.m_rid;
		this->m_netid = role.m_netid;
		this->m_guildid = role.m_guildid;
		this->m_name = role.m_name;

		return ;
	}

	Role& Role::operator=(const Role& role){
		if(this == &role){
			return *this;
		}

		this->m_rid = role.m_rid;
		this->m_netid = role.m_netid;
		this->m_guildid = role.m_guildid;
		this->m_name = role.m_name;

		return *this;
	}

	void Role::SetRoleId(RoleId rid){
		if(!rid){
			return ;
		}

		this->m_rid = rid;
	}

	void Role::SetNetId(NetId netid){
		if(!netid){
			return ;
		}

		this->m_netid = netid;
	}

	void Role::SetGuildId(GuildId guildid){
		if(!guildid){
			return ;
		}

		this->m_guildid = guildid;
	}

	void Role::SetName(string name){
		if(0 == name.size()){
			return ;
		}
		
		this->m_name = name;
	}

	RoleId  Role::GetRoleId(void){
		return this->m_rid;
	}

	NetId Role::GetNetId(void){
		return this->m_netid;
	}

	GuildId Role::GetGuilId(void){
		return this->m_guildid;
	}

	string 	Role::GetName(void){
		return this->m_name;
	}

	void	Role::Save(void){
		
	}
}