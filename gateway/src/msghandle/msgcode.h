#ifndef H_GSMSGCODE_H_
#define H_GSMSGCODE_H_

namespace gateway {
	enum {
		CS_ROLE_TO_GATEWAY_BEGIN			=	500								//	role和gateway开始的通信协议
	};

	enum S_MsgType {
		CS_ROLE_TO_GATEWAY					=	CS_ROLE_TO_GATEWAY_BEGIN,			// 	玩家登录协议头
		
		CS_ROLE_TOGATEWAY_CREATEROLE												//	玩家创建角色协议头
	};
}

#endif