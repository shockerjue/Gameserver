#ifndef H_ROLE_H_
#define H_ROLE_H_

#include <iostream>
#include "./../../../common/engine/engine.h"

using namespace std;
using namespace engine;

namespace role {
	typedef int		RoleId;
	typedef int 	GuildId;

	class Role {
	public:
		Role();
		~Role();
		Role(const Role& role);
		Role &operator=(const Role& role);

		void	SetRoleId(RoleId rid);
		void 	SetNetId(NetId netid);
		void 	SetGuildId(GuildId guildid);
		void 	SetName(string name);
		void	SetLogin(bool is);

		RoleId  GetRoleId(void);
		NetId 	GetNetId(void);
		GuildId GetGuilId(void);
		string 	GetName(void);
		bool 	GetLogin(void);

		void	Save(void);
	private:
		RoleId		m_rid;
		NetId 		m_netid;
		GuildId		m_guildid;
		string 		m_name;
		bool 		m_islogin;
	};
}

#endif