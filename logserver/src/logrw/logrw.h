#ifndef H_LOGRW_H_
#define H_LOGRW_H_

#include <vector>
#include <string>
#include "logread.h"
#include "logwrite.h"
#include "./../../../common/include/value.h"

using namespace std;

namespace logserver {
    class LogRW {
    public:
        static vector<map<string,Value> > read(string sql);
        static int write(int level,string proc,string func,string msg,string date);
    };
}

#endif