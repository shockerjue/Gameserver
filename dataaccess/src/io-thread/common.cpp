#include "common.h"

namespace io_thread {
    map<int,Message_Func> Common::s_what_call;

    void Common::RegisterCall(void) {
        /**
        * 下面注册读线程消息回调函数
        */
        Common::s_what_call[Inner_What::INNER_READ_ROLE_FOR_LOGIN]      = DbHost::onLoginRequest;
        Common::s_what_call[Inner_What::INNER_READ_ROLE_FOR_REQ]        = DbHost::onRequestLoginInfo;
        Common::s_what_call[Inner_What::INNER_READ_ROLE_FRIENDS]        = DbHost::onRequestFriends;
        Common::s_what_call[Inner_What::INNER_READ_ADD_FRIEND]          = DbHost::onRequestAddFriend;
        Common::s_what_call[Inner_What::INNER_READ_OFFLINE_MSG]         = DbHost::onRequestMsg;

        /**
        * 下面注册写线程消息回调函数
        */
        Common::s_what_call[Inner_What::INNER_WRITE_ROLE_FOR_CREATE]    = DbHost::onCreateRole;
        Common::s_what_call[Inner_What::INEER_WRITE_ADD_FRIEND]         = DbHost::onAddFriendWrite;

        Common::s_what_call[Inner_What::INNER_WRITE_OFFLINE_MSG]        = ChatHost::onSaveOfflineMsg;
    }

    void Common::Excute(int what,const void* param,Message message) {
        if(!what) {
            return ;
        }

        map<int,Message_Func>::iterator it = Common::s_what_call.find(what);
        if(it == Common::s_what_call.end()){
            return ;
        }

        Common::s_what_call[what](param,message);
    }
}