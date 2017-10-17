/**
* 主要是定义消息的头部
*/

#ifndef H_MSGHEADER_H_
#define H_MSGHEADER_H_

#ifdef _cplusplus
    extern "C"{
#endif
#include <string.h>
#ifdef _cplusplus
    }
#endif
    
#include <string>
#include <iostream>


using namespace std;

namespace msghandle {
	enum{
		MT_INVALID = 0,
	};

	/**
	* 网络事件消息头部
	*/
	struct MessageHeader{
		int 	msg_type; 		//	消息类型
		int 	msg_size;		//	消息数据大小

		MessageHeader(){}
		MessageHeader(const MessageHeader& messageHeader){
			this->msg_type = messageHeader.msg_type;
			this->msg_size = messageHeader.msg_size;

			return ;
		}

		MessageHeader& operator=(const MessageHeader& messageHeader){
			if(this == &messageHeader){
				return *this;
			}

			this->msg_type = messageHeader.msg_type;
			this->msg_size = messageHeader.msg_size;

			return *this;
		}

		string toString(void){
			char buf[32] = {0};

			sprintf(buf,"{msg_type: %d, msg_size: %d}",
				this->msg_type,
				this->msg_size);

			return string(buf);
		}
	};
}

#endif