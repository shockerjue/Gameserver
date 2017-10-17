#include "write.h"

namespace io_thread {
    Write::Write(void){

    }

    Write::~Write(void) {

    }

    Write::Write(const Write& write) {

    }

    Write& Write::operator=(const Write& write){
        if(this == &write) {
            return *this;
        }

        return *this;
    }

    void Write::new_msg(const void* param,Message message) {
        if(0 == message.getsize() || 
            !message.getwhat()){
            return ;
        }

        int what =  message.getwhat();
        Common::Excute(what,param,message);

        return ;
    }

    void Write::new_msg(const void* param,vector<Message> msgs) {
        vector<Message>::iterator it = msgs.begin();

        while(it != msgs.end()){
            Message msg = *it;
            this->new_msg(param,msg);

            it++;
        }

        return ;
    }
}