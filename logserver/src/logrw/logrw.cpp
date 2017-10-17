#include "logrw.h"

namespace logserver {
    vector<map<string,Value> > LogRW::read(string sql){
        vector<map<string,Value> > temp;
        if(0 == sql.size()){
            return temp;
        }

        temp = LogRead::getLogRead()->read(sql);

        return temp;
    }

    int LogRW::write(int level,string proc,string func,string msg,string date){
        if(0 == proc.size() || 0 == func.size() ||
            0 == msg.size()){
            return -1;
        }

        int ret = LogWrite::getLogWrite()->write(level,proc,func,msg,date);

        return ret;
    }
}