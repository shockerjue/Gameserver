#include "read.h"

namespace io_thread {
    Read::Read(void){

    }

    Read::~Read(void) {

    }

    Read::Read(const Read& read) {

    }

    Read& Read::operator=(const Read& read) {
        if(this == &read) {
            return *this;
        }

        return *this;
    }

    void Read::new_msg(const void* param,Message message) {
        if(0 == message.getsize() || 
            !message.getwhat()){
            return ;
        }

        int what =  message.getwhat();
        Common::Excute(what,param,message);

        return ;
    }

    void Read::new_msg(const void* param,vector<Message> msgs) {
        vector<Message>::iterator it = msgs.begin();

        while(it != msgs.end()){
            Message msg = *it;
            this->new_msg(param,msg);

            it++;
        }

        return ;
    }

}