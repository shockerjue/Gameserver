#ifndef H_LOGREAD_H_
#define H_LOGREAD_H_

#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "./../../../common/ipc/ipc.pb.h"
#include "./../../../common/db/mysqlutil.h"
#include "./../../../common/engine/engine.h"
#include "./../../../common/include/value.h"

using namespace std;
using namespace engine;
using namespace engine::db;

namespace logserver {
    class LogRead {
    public:
        ~LogRead(void);

        vector<map<string,Value> > read(string sql);
    public:
        static LogRead*     getLogRead(void);
        
    private:
        MysqlUtil 	m_mysqlutil;

    private:
        LogRead(void);

        static  LogRead*    s_pLogRead;
    };
}

#endif