#ifndef H_LOGWRITE_H_
#define H_LOGWRITE_H_


#ifdef _cplusplus
extern "C"{
#endif
	#include <stdio.h>
#ifdef _cplusplus
}
#endif

#include <map>
#include <string>
#include <iostream>

#include "./../../../common/ipc/ipc.h"
#include "./../../../common/db/mysqlutil.h"
#include "./../../../common/engine/engine.h"

using namespace std;
using namespace engine;
using namespace engine::db;

namespace logserver {
    class LogWrite {
    public:
        ~LogWrite(void);
        int write(int level,string proc,string func,string msg,string date);

    public:
        static LogWrite* getLogWrite(void);

    private:
        MysqlUtil 	m_mysqlutil;

    private:
        LogWrite(void);

        static LogWrite* s_pLogWrite;
    };
}

#endif